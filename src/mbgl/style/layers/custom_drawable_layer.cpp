#include <mbgl/style/layers/custom_drawable_layer.hpp>
#include <mbgl/style/layers/custom_drawable_layer_impl.hpp>

#include <mbgl/renderer/layers/render_custom_drawable_layer.hpp>
#include <mbgl/style/layer_observer.hpp>
#include <mbgl/gfx/context.hpp>
#include <mbgl/renderer/paint_parameters.hpp>
#include <mbgl/renderer/change_request.hpp>
#include <mbgl/renderer/layer_group.hpp>
#include <mbgl/gfx/cull_face_mode.hpp>
#include <mbgl/shaders/shader_program_base.hpp>
#include <mbgl/renderer/layer_tweaker.hpp>
#include <mbgl/gfx/drawable.hpp>
#include <mbgl/gfx/drawable_tweaker.hpp>
#include <mbgl/shaders/line_layer_ubo.hpp>
#include <mbgl/util/convert.hpp>
#include <mbgl/util/geometry.hpp>
#include <mbgl/programs/fill_program.hpp>
#include <mbgl/shaders/fill_layer_ubo.hpp>
#include <mbgl/util/math.hpp>
#include <mbgl/tile/geometry_tile_data.hpp>
#include <mbgl/util/containers.hpp>
#include <mbgl/gfx/fill_generator.hpp>
#include <mbgl/shaders/custom_drawable_layer_ubo.hpp>
#include <mbgl/shaders/shader_source.hpp>
#include <mbgl/gfx/renderable.hpp>
#include <mbgl/gfx/renderer_backend.hpp>
#include <mbgl/shaders/widevector_ubo.hpp>

#include <cmath>

namespace mbgl {

namespace style {

using namespace shaders;

namespace {
const LayerTypeInfo typeInfoCustomDrawable{"custom-drawable",
                                           LayerTypeInfo::Source::NotRequired,
                                           LayerTypeInfo::Pass3D::NotRequired,
                                           LayerTypeInfo::Layout::NotRequired,
                                           LayerTypeInfo::FadingTiles::NotRequired,
                                           LayerTypeInfo::CrossTileIndex::NotRequired,
                                           LayerTypeInfo::TileKind::NotRequired};
} // namespace

CustomDrawableLayer::CustomDrawableLayer(const std::string& layerID, std::unique_ptr<CustomDrawableLayerHost> host)
    : Layer(makeMutable<Impl>(layerID, std::move(host))) {}

CustomDrawableLayer::~CustomDrawableLayer() = default;

const CustomDrawableLayer::Impl& CustomDrawableLayer::impl() const {
    return static_cast<const Impl&>(*baseImpl);
}

Mutable<CustomDrawableLayer::Impl> CustomDrawableLayer::mutableImpl() const {
    return makeMutable<Impl>(impl());
}

std::unique_ptr<Layer> CustomDrawableLayer::cloneRef(const std::string&) const {
    assert(false);
    return nullptr;
}

using namespace conversion;

std::optional<Error> CustomDrawableLayer::setPropertyInternal(const std::string&, const Convertible&) {
    return Error{"layer doesn't support this property"};
}

StyleProperty CustomDrawableLayer::getProperty(const std::string&) const {
    return {};
}

Mutable<Layer::Impl> CustomDrawableLayer::mutableBaseImpl() const {
    return staticMutableCast<Layer::Impl>(mutableImpl());
}

// static
const LayerTypeInfo* CustomDrawableLayer::Impl::staticTypeInfo() noexcept {
    return &typeInfoCustomDrawable;
}

// CustomDrawableLayerHost::Interface

class LineDrawableTweaker : public gfx::DrawableTweaker {
public:
    LineDrawableTweaker(const shaders::LinePropertiesUBO& properties)
        : linePropertiesUBO(properties) {}

    void init(gfx::Drawable&) override {};

    void execute(gfx::Drawable& drawable, const PaintParameters& parameters) override {
        if (!drawable.getTileID().has_value()) {
            return;
        }

        const UnwrappedTileID tileID = drawable.getTileID()->toUnwrapped();
        const auto zoom = parameters.state.getZoom();
        mat4 tileMatrix;
        parameters.state.matrixFor(/*out*/ tileMatrix, tileID);

        const auto matrix = LayerTweaker::getTileMatrix(
            tileID, parameters, {{0, 0}}, style::TranslateAnchorType::Viewport, false, false, drawable, false);

        const shaders::LineDynamicUBO dynamicUBO = {
            /*units_to_pixels = */ {1.0f / parameters.pixelsToGLUnits[0], 1.0f / parameters.pixelsToGLUnits[1]}, 0, 0};

        const shaders::LineUBO lineUBO{/*matrix = */ util::cast<float>(matrix),
                                       /*ratio = */ 1.0f / tileID.pixelsToTileUnits(1.0f, zoom),
                                       0,
                                       0,
                                       0};

        const shaders::LineInterpolationUBO lineInterpolationUBO{/*color_t =*/0.f,
                                                                 /*blur_t =*/0.f,
                                                                 /*opacity_t =*/0.f,
                                                                 /*gapwidth_t =*/0.f,
                                                                 /*offset_t =*/0.f,
                                                                 /*width_t =*/0.f,
                                                                 0,
                                                                 0};
        auto& uniforms = drawable.mutableUniformBuffers();
        uniforms.createOrUpdate(idLineDynamicUBO, &dynamicUBO, parameters.context);
        uniforms.createOrUpdate(idLineUBO, &lineUBO, parameters.context);
        uniforms.createOrUpdate(idLinePropertiesUBO, &linePropertiesUBO, parameters.context);
        uniforms.createOrUpdate(idLineInterpolationUBO, &lineInterpolationUBO, parameters.context);
    };

private:
    shaders::LinePropertiesUBO linePropertiesUBO;
};

class WideVectorDrawableTweaker : public gfx::DrawableTweaker {
public:
    void init(gfx::Drawable&) override {};

    void execute(gfx::Drawable& drawable, const PaintParameters& parameters) override {
        const auto renderableSize = parameters.backend.getDefaultRenderable().getSize();
        shaders::WideVectorUniformsUBO uniform{
            /*mvpMatrix     */ mbgl::matrix::identity4f(),
            /*mvpMatrixDiff */ {0.0f},
            /*mvMatrix      */ mbgl::matrix::identity4f(),
            /*mvMatrixDiff  */ {0.0f},
            /*pMatrix       */ mbgl::matrix::identity4f(),
            /*pMatrixDiff   */ {0.0f},
            /*frameSize     */ {(float)renderableSize.width, (float)renderableSize.height}};

        enum class WKSVertexLineJoinType {
            WKSVertexLineJoinMiter = 0,
            WKSVertexLineJoinMiterClip = 1,
            WKSVertexLineJoinMiterSimple = 2,
            WKSVertexLineJoinRound = 3,
            WKSVertexLineJoinBevel = 4,
            WKSVertexLineJoinNone = 5,
        };
        enum class WKSVertexLineCapType {
            WKSVertexLineCapButt = 0,
            WKSVertexLineCapRound = 1,
            WKSVertexLineCapSquare = 2,
        };
        style::WideVectorUniformWideVecUBO wideVec{
            /*w2            */ 16.0f,
            /*offset        */ 0.0f,
            /*edge          */ 1.0f,
            /*texRepeat     */ 0.0f,
            /*texOffset     */ {},
            /*miterLimit    */ 1.0f,
            /*join          */ static_cast<int32_t>(WKSVertexLineJoinType::WKSVertexLineJoinMiter),
            /*cap           */ static_cast<int32_t>(WKSVertexLineCapType::WKSVertexLineCapButt),
            /*hasExp        */ false,
            /*interClipLimit*/ 0.0f};

        auto& uniforms = drawable.mutableUniformBuffers();
        uniforms.createOrUpdate(idWideVectorUniformsUBO, &uniform, parameters.context);
        uniforms.createOrUpdate(idWideVectorUniformWideVecUBO, &wideVec, parameters.context);
    };

private:
    shaders::LinePropertiesUBO linePropertiesUBO;
};

class FillDrawableTweaker : public gfx::DrawableTweaker {
public:
    FillDrawableTweaker(const Color& color_, float opacity_)
        : color(color_),
          opacity(opacity_) {}

    void init(gfx::Drawable&) override {};

    void execute(gfx::Drawable& drawable, const PaintParameters& parameters) override {
        if (!drawable.getTileID().has_value()) {
            return;
        }

        const UnwrappedTileID tileID = drawable.getTileID()->toUnwrapped();
        mat4 tileMatrix;
        parameters.state.matrixFor(/*out*/ tileMatrix, tileID);

        const auto matrix = LayerTweaker::getTileMatrix(
            tileID, parameters, {{0, 0}}, style::TranslateAnchorType::Viewport, false, false, drawable, false);

        const shaders::FillDrawableUBO fillUBO{/*matrix = */ util::cast<float>(matrix)};

        const shaders::FillEvaluatedPropsUBO fillPropertiesUBO{
            /* .color = */ color,
            /* .opacity = */ opacity,
            0,
            0,
            0,
        };

        const shaders::FillInterpolateUBO fillInterpolateUBO{
            /* .color_t = */ 0.f,
            /* .opacity_t = */ 0.f,
            0,
            0,
        };
        auto& uniforms = drawable.mutableUniformBuffers();
        uniforms.createOrUpdate(idFillDrawableUBO, &fillUBO, parameters.context);
        uniforms.createOrUpdate(idFillEvaluatedPropsUBO, &fillPropertiesUBO, parameters.context);
        uniforms.createOrUpdate(idFillInterpolateUBO, &fillInterpolateUBO, parameters.context);
    };

private:
    Color color;
    float opacity;
};

class SymbolDrawableTweaker : public gfx::DrawableTweaker {
public:
    SymbolDrawableTweaker(const CustomDrawableLayerHost::Interface::SymbolOptions& options_)
        : options(options_) {}

    void init(gfx::Drawable&) override {};

    void execute(gfx::Drawable& drawable, const PaintParameters& parameters) override {
        if (!drawable.getTileID().has_value()) {
            return;
        }

        const UnwrappedTileID tileID = drawable.getTileID()->toUnwrapped();
        mat4 tileMatrix;
        parameters.state.matrixFor(/*out*/ tileMatrix, tileID);

        const auto matrix = LayerTweaker::getTileMatrix(
            tileID, parameters, {{0, 0}}, style::TranslateAnchorType::Viewport, false, false, drawable, false);

        const shaders::CustomSymbolIconDrawableUBO drawableUBO{/*matrix = */ util::cast<float>(matrix)};

        const auto pixelsToTileUnits = tileID.pixelsToTileUnits(
            1.0f, options.scaleWithMap ? tileID.canonical.z : parameters.state.getZoom());
        const float factor = options.scaleWithMap
                                 ? static_cast<float>(std::pow(2.f, parameters.state.getZoom() - tileID.canonical.z))
                                 : 1.0f;
        const auto extrudeScale = options.pitchWithMap ? std::array<float, 2>{pixelsToTileUnits, pixelsToTileUnits}
                                                       : std::array<float, 2>{parameters.pixelsToGLUnits[0] * factor,
                                                                              parameters.pixelsToGLUnits[1] * factor};

        const shaders::CustomSymbolIconParametersUBO parametersUBO{
            /*extrude_scale*/ {extrudeScale[0] * options.size.width, extrudeScale[1] * options.size.height},
            /*anchor*/ options.anchor,
            /*angle_degrees*/ options.angleDegrees,
            /*scale_with_map*/ options.scaleWithMap,
            /*pitch_with_map*/ options.pitchWithMap,
            /*camera_to_center_distance*/ parameters.state.getCameraToCenterDistance(),
            /*aspect_ratio*/ parameters.pixelsToGLUnits[0] / parameters.pixelsToGLUnits[1],
            0,
            0,
            0};

        // set UBOs
        auto& uniforms = drawable.mutableUniformBuffers();
        uniforms.createOrUpdate(idCustomSymbolDrawableUBO, &drawableUBO, parameters.context);
        uniforms.createOrUpdate(idCustomSymbolParametersUBO, &parametersUBO, parameters.context);
    };

private:
    CustomDrawableLayerHost::Interface::SymbolOptions options;
};

CustomDrawableLayerHost::Interface::Interface(RenderLayer& layer_,
                                              LayerGroupBasePtr& layerGroup_,
                                              gfx::ShaderRegistry& shaders_,
                                              gfx::Context& context_,
                                              const TransformState& state_,
                                              const std::shared_ptr<UpdateParameters>& updateParameters_,
                                              const RenderTree& renderTree_,
                                              UniqueChangeRequestVec& changes_)
    : layer(layer_),
      layerGroup(layerGroup_),
      shaders(shaders_),
      context(context_),
      state(state_),
      updateParameters(updateParameters_),
      renderTree(renderTree_),
      changes(changes_) {
    // ensure we have a default layer group set up
    if (!layerGroup) {
        if (auto aLayerGroup = context.createTileLayerGroup(
                /*layerIndex*/ layer.getLayerIndex(), /*initialCapacity=*/64, layer.getID())) {
            changes.emplace_back(std::make_unique<AddLayerGroupRequest>(aLayerGroup));
            layerGroup = std::move(aLayerGroup);
        }
    }
}

std::size_t CustomDrawableLayerHost::Interface::getDrawableCount() const {
    return layerGroup->getDrawableCount();
}

void CustomDrawableLayerHost::Interface::setTileID(OverscaledTileID tileID_) {
    tileID = tileID_;
}

void CustomDrawableLayerHost::Interface::setLineOptions(const LineOptions& options) {
    finish();
    lineOptions = options;
}

void CustomDrawableLayerHost::Interface::setFillOptions(const FillOptions& options) {
    finish();
    fillOptions = options;
}

void CustomDrawableLayerHost::Interface::setSymbolOptions(const SymbolOptions& options) {
    finish();
    symbolOptions = options;
}

void CustomDrawableLayerHost::Interface::updateBuilder(BuilderType type,
                                                       const std::string& name,
                                                       gfx::ShaderPtr shader) {
    assert(shader);
    if (type != builderType || !builder || builder->getShader() != shader) {
        finish();
        builder = createBuilder(name, shader);
        builderType = type;
    }
    assert(builder);
    assert(builder->getShader() == shader);
};

void CustomDrawableLayerHost::Interface::addPolyline(const GeometryCoordinates& coordinates) {
    switch (lineOptions.shaderType) {
        case LineShaderType::Classic: {
            // build classic polyline
            updateBuilder(BuilderType::LineClassic, "custom-lines", lineShaderDefault());
            builder->addPolyline(coordinates, lineOptions.geometry);
        } break;

        case LineShaderType::MetalWideVector: {
            // build wide vector polyline
            updateBuilder(BuilderType::LineWideVector, "custom-lines-widevector", lineShaderWideVector());

            // vertices
            struct VertexTriWideVecB {
                // x, y offset around the center
                std::array<float, 3> screenPos;
                std::array<float, 4> color;
                int index;
            };
            std::array<float, 4> color{1.0f, 0, 0, .5f};
            VertexTriWideVecB vertexBuffer[12]{
                {{0, 0, 0}, color, (0 << 16) + 0},
                {{0, 0, 0}, color, (1 << 16) + 0},
                {{0, 0, 0}, color, (2 << 16) + 0},
                {{0, 0, 0}, color, (3 << 16) + 0},

                {{0, 0, 0}, color, (4 << 16) + 1},
                {{0, 0, 0}, color, (5 << 16) + 1},
                {{0, 0, 0}, color, (6 << 16) + 1},
                {{0, 0, 0}, color, (7 << 16) + 1},

                {{0, 0, 0}, color, (8 << 16) + 2},
                {{0, 0, 0}, color, (9 << 16) + 2},
                {{0, 0, 0}, color, (10 << 16) + 2},
                {{0, 0, 0}, color, (11 << 16) + 2},
            };

            using VertexVector = gfx::VertexVector<VertexTriWideVecB>;
            const std::shared_ptr<VertexVector> sharedVertices = std::make_shared<VertexVector>();
            VertexVector& vertices = *sharedVertices;
            for (const auto& v : vertexBuffer) {
                vertices.emplace_back(v);
            }

            // instance data
            struct VertexTriWideVecInstance {
                std::array<float, 4> center;
                std::array<float, 4> color;
                int32_t prev;
                int32_t next;
                int64_t pad_;
            };
            static_assert(sizeof(VertexTriWideVecInstance) == 48);

            VertexTriWideVecInstance centerline[4]{
                {{0.5, -0.5, 0}, {3}, -1, 1, 0},
                {{-0.5, -0.5, 0}, {3}, 0, 2, 0},
                {{0.0, +0.5, 0}, {3}, 1, 3, 0},
                {{0.0, 0.0, 0}, {3}, 2, -1, 0},
            };

            using InstanceVector = gfx::VertexVector<VertexTriWideVecInstance>;
            const std::shared_ptr<InstanceVector> sharedInstanceData = std::make_shared<InstanceVector>();
            InstanceVector& instanceData = *sharedInstanceData;
            for (const auto& data : centerline) {
                instanceData.emplace_back(data);
            }

            // indexes
            using TriangleIndexVector = gfx::IndexVector<gfx::Triangles>;
            const std::shared_ptr<TriangleIndexVector> sharedTriangles = std::make_shared<TriangleIndexVector>();
            TriangleIndexVector& triangles = *sharedTriangles;
            triangles.emplace_back(
                0, 3, 1, 0, 2, 3, 4 + 0, 4 + 3, 4 + 1, 4 + 0, 4 + 2, 4 + 3, 8 + 0, 8 + 3, 8 + 1, 8 + 0, 8 + 2, 8 + 3);

            SegmentVector<VertexTriWideVecB> triangleSegments;
            triangleSegments.emplace_back(Segment<VertexTriWideVecB>{0, 0, 12, 18});

            // add to builder
            {
                // add vertex attributes
                auto vertexAttributes = context.createVertexAttributeArray();
                if (const auto& attr = vertexAttributes->set(idWideVectorScreenPos)) {
                    attr->setSharedRawData(sharedVertices,
                                           offsetof(VertexTriWideVecB, screenPos),
                                           /*vertexOffset=*/0,
                                           sizeof(VertexTriWideVecB),
                                           gfx::AttributeDataType::Float3);
                }
                if (const auto& attr = vertexAttributes->set(idWideVectorColor)) {
                    attr->setSharedRawData(sharedVertices,
                                           offsetof(VertexTriWideVecB, color),
                                           /*vertexOffset=*/0,
                                           sizeof(VertexTriWideVecB),
                                           gfx::AttributeDataType::Float4);
                }
                if (const auto& attr = vertexAttributes->set(idWideVectorIndex)) {
                    attr->setSharedRawData(sharedVertices,
                                           offsetof(VertexTriWideVecB, index),
                                           /*vertexOffset=*/0,
                                           sizeof(VertexTriWideVecB),
                                           gfx::AttributeDataType::Int);
                }
                builder->setVertexAttributes(std::move(vertexAttributes));
            }

            {
                // add instance attributes
                auto instanceAttributes = context.createVertexAttributeArray();
                if (const auto& attr = instanceAttributes->set(idWideVectorInstanceCenter)) {
                    attr->setSharedRawData(sharedInstanceData,
                                           offsetof(VertexTriWideVecInstance, center),
                                           /*vertexOffset=*/0,
                                           sizeof(VertexTriWideVecInstance),
                                           gfx::AttributeDataType::Float3);
                }
                if (const auto& attr = instanceAttributes->set(idWideVectorInstanceColor)) {
                    attr->setSharedRawData(sharedInstanceData,
                                           offsetof(VertexTriWideVecInstance, color),
                                           /*vertexOffset=*/0,
                                           sizeof(VertexTriWideVecInstance),
                                           gfx::AttributeDataType::Float4);
                }
                if (const auto& attr = instanceAttributes->set(idWideVectorInstancePrevious)) {
                    attr->setSharedRawData(sharedInstanceData,
                                           offsetof(VertexTriWideVecInstance, prev),
                                           /*vertexOffset=*/0,
                                           sizeof(VertexTriWideVecInstance),
                                           gfx::AttributeDataType::Int);
                }
                if (const auto& attr = instanceAttributes->set(idWideVectorInstanceNext)) {
                    attr->setSharedRawData(sharedInstanceData,
                                           offsetof(VertexTriWideVecInstance, next),
                                           /*vertexOffset=*/0,
                                           sizeof(VertexTriWideVecInstance),
                                           gfx::AttributeDataType::Int);
                }
                builder->setInstanceAttributes(std::move(instanceAttributes));
            }

            builder->setRawVertices({}, vertices.elements(), gfx::AttributeDataType::Float2);
            builder->setSegments(gfx::Triangles(), sharedTriangles, triangleSegments.data(), triangleSegments.size());

            // flush current builder drawable
            builder->flush(context);
        } break;
    }
}

void CustomDrawableLayerHost::Interface::addFill(const GeometryCollection& geometry) {
    // build fill
    updateBuilder(BuilderType::Fill, "custom-fill", fillShaderDefault());

    // provision buffers for fill vertices, indexes and segments
    using VertexVector = gfx::VertexVector<FillLayoutVertex>;
    const std::shared_ptr<VertexVector> sharedVertices = std::make_shared<VertexVector>();
    VertexVector& vertices = *sharedVertices;

    using TriangleIndexVector = gfx::IndexVector<gfx::Triangles>;
    const std::shared_ptr<TriangleIndexVector> sharedTriangles = std::make_shared<TriangleIndexVector>();
    TriangleIndexVector& triangles = *sharedTriangles;

    SegmentVector<FillAttributes> triangleSegments;

    // generate fill geometry into buffers
    gfx::generateFillBuffers(geometry, vertices, triangles, triangleSegments);

    // add to builder
    auto attrs = context.createVertexAttributeArray();
    if (const auto& attr = attrs->set(idFillPosVertexAttribute)) {
        attr->setSharedRawData(sharedVertices,
                               offsetof(FillLayoutVertex, a1),
                               /*vertexOffset=*/0,
                               sizeof(FillLayoutVertex),
                               gfx::AttributeDataType::Short2);
    }
    builder->setVertexAttributes(std::move(attrs));
    builder->setRawVertices({}, vertices.elements(), gfx::AttributeDataType::Short2);
    builder->setSegments(gfx::Triangles(), sharedTriangles, triangleSegments.data(), triangleSegments.size());

    // flush current builder drawable
    builder->flush(context);
}

void CustomDrawableLayerHost::Interface::addSymbol(const GeometryCoordinate& point) {
    // build symbol
    updateBuilder(BuilderType::Symbol, "custom-symbol", symbolShaderDefault());

    // temporary: buffers
    struct CustomSymbolIcon {
        std::array<float, 2> a_pos;
        std::array<float, 2> a_tex;
    };

    // vertices
    using VertexVector = gfx::VertexVector<CustomSymbolIcon>;
    const std::shared_ptr<VertexVector> sharedVertices = std::make_shared<VertexVector>();
    VertexVector& vertices = *sharedVertices;

    // encode center and extrude direction into vertices
    for (int y = 0; y <= 1; ++y) {
        for (int x = 0; x <= 1; ++x) {
            vertices.emplace_back(
                CustomSymbolIcon{{static_cast<float>(point.x * 2 + x), static_cast<float>(point.y * 2 + y)},
                                 {symbolOptions.textureCoordinates[x][0], symbolOptions.textureCoordinates[y][1]}});
        }
    }

    // indexes
    using TriangleIndexVector = gfx::IndexVector<gfx::Triangles>;
    const std::shared_ptr<TriangleIndexVector> sharedTriangles = std::make_shared<TriangleIndexVector>();
    TriangleIndexVector& triangles = *sharedTriangles;

    triangles.emplace_back(0, 1, 2, 1, 2, 3);

    SegmentVector<CustomSymbolIcon> triangleSegments;
    triangleSegments.emplace_back(Segment<CustomSymbolIcon>{0, 0, 4, 6});

    // add to builder
    auto attrs = context.createVertexAttributeArray();
    if (const auto& attr = attrs->set(idCustomSymbolPosVertexAttribute)) {
        attr->setSharedRawData(sharedVertices,
                               offsetof(CustomSymbolIcon, a_pos),
                               /*vertexOffset=*/0,
                               sizeof(CustomSymbolIcon),
                               gfx::AttributeDataType::Float2);
    }
    if (const auto& attr = attrs->set(idCustomSymbolTexVertexAttribute)) {
        attr->setSharedRawData(sharedVertices,
                               offsetof(CustomSymbolIcon, a_tex),
                               /*vertexOffset=*/0,
                               sizeof(CustomSymbolIcon),
                               gfx::AttributeDataType::Float2);
    }
    builder->setVertexAttributes(std::move(attrs));
    builder->setRawVertices({}, vertices.elements(), gfx::AttributeDataType::Float2);
    builder->setSegments(gfx::Triangles(), sharedTriangles, triangleSegments.data(), triangleSegments.size());

    // texture
    if (symbolOptions.texture) {
        builder->setTexture(symbolOptions.texture, idCustomSymbolImageTexture);
    }

    // create fill tweaker
    auto tweaker = std::make_shared<SymbolDrawableTweaker>(symbolOptions);
    builder->addTweaker(tweaker);

    // flush current builder drawable
    builder->flush(context);
}

void CustomDrawableLayerHost::Interface::finish() {
    if (builder && !builder->empty()) {
        // finish
        const auto finish_ = [this](gfx::DrawableTweakerPtr tweaker) {
            builder->flush(context);
            for (auto& drawable : builder->clearDrawables()) {
                assert(tileID.has_value());
                drawable->setTileID(tileID.value()); // TODO: fix usage of tileID
                if (tweaker) {
                    drawable->addTweaker(tweaker);
                }

                TileLayerGroup* tileLayerGroup = static_cast<TileLayerGroup*>(layerGroup.get());
                tileLayerGroup->addDrawable(RenderPass::Translucent, tileID.value(), std::move(drawable));
            }
        };

        // what were we building?
        switch (builderType) {
            case BuilderType::LineClassic: {
                // finish building classic lines

                // create line tweaker
                const shaders::LinePropertiesUBO linePropertiesUBO{lineOptions.color,
                                                                   lineOptions.blur,
                                                                   lineOptions.opacity,
                                                                   lineOptions.gapWidth,
                                                                   lineOptions.offset,
                                                                   lineOptions.width,
                                                                   0,
                                                                   0,
                                                                   0};
                auto tweaker = std::make_shared<LineDrawableTweaker>(linePropertiesUBO);

                // finish drawables
                finish_(tweaker);
            } break;
            case BuilderType::LineWideVector: {
                // finish building wide vector lines

                // create line tweaker
                auto tweaker = std::make_shared<WideVectorDrawableTweaker>();

                // finish drawables
                finish_(tweaker);
            } break;
            case BuilderType::Fill: {
                // finish building fills

                // create fill tweaker
                auto tweaker = std::make_shared<FillDrawableTweaker>(fillOptions.color, fillOptions.opacity);

                // finish drawables
                finish_(tweaker);
            } break;
            case BuilderType::Symbol:
                // finish building symbols
                finish_(nullptr);
                break;
            default:
                break;
        }
    }
}

gfx::ShaderPtr CustomDrawableLayerHost::Interface::lineShaderDefault() const {
    gfx::ShaderGroupPtr shaderGroup = shaders.getShaderGroup("LineShader");
    assert(shaderGroup);
    if (!shaderGroup) return gfx::ShaderPtr();

    const StringIDSetsPair propertiesAsUniforms{{"a_color", "a_blur", "a_opacity", "a_gapwidth", "a_offset", "a_width"},
                                                {idLineColorVertexAttribute,
                                                 idLineBlurVertexAttribute,
                                                 idLineOpacityVertexAttribute,
                                                 idLineGapWidthVertexAttribute,
                                                 idLineOffsetVertexAttribute,
                                                 idLineWidthVertexAttribute}};

    return shaderGroup->getOrCreateShader(context, propertiesAsUniforms);
}

gfx::ShaderPtr CustomDrawableLayerHost::Interface::lineShaderWideVector() const {
    gfx::ShaderGroupPtr shaderGroup = shaders.getShaderGroup("WideVectorShader");
    assert(shaderGroup);
    if (!shaderGroup) return gfx::ShaderPtr();

    return shaderGroup->getOrCreateShader(context, {});
}

gfx::ShaderPtr CustomDrawableLayerHost::Interface::fillShaderDefault() const {
    gfx::ShaderGroupPtr shaderGroup = shaders.getShaderGroup("FillShader");

    const StringIDSetsPair propertiesAsUniforms{{"a_color", "a_opacity"},
                                                {idFillColorVertexAttribute, idFillOpacityVertexAttribute}};

    return shaderGroup->getOrCreateShader(context, propertiesAsUniforms);
}

gfx::ShaderPtr CustomDrawableLayerHost::Interface::symbolShaderDefault() const {
    return context.getGenericShader(shaders, "CustomSymbolIconShader");
}

std::unique_ptr<gfx::DrawableBuilder> CustomDrawableLayerHost::Interface::createBuilder(const std::string& name,
                                                                                        gfx::ShaderPtr shader) const {
    std::unique_ptr<gfx::DrawableBuilder> builder_ = context.createDrawableBuilder(name);
    builder_->setShader(std::static_pointer_cast<gfx::ShaderProgramBase>(shader));
    builder_->setSubLayerIndex(0);
    builder_->setEnableDepth(false);
    builder_->setColorMode(gfx::ColorMode::alphaBlended());
    builder_->setCullFaceMode(gfx::CullFaceMode::disabled());
    builder_->setRenderPass(RenderPass::Translucent);

    return builder_;
}

} // namespace style
} // namespace mbgl
