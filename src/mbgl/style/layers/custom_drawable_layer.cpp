#include <mbgl/style/layers/custom_drawable_layer.hpp>
#include <mbgl/style/layers/custom_drawable_layer_impl.hpp>

#include <mbgl/renderer/layers/render_custom_drawable_layer.hpp>
#include <mbgl/style/layer_observer.hpp>
#include <mbgl/gfx/context.hpp>
#include <mbgl/renderer/change_request.hpp>
#include <mbgl/renderer/layer_group.hpp>
#include <mbgl/gfx/cull_face_mode.hpp>
#include <mbgl/shaders/shader_program_base.hpp>
#include <mbgl/renderer/layer_tweaker.hpp>
#include <mbgl/gfx/drawable.hpp>
#include <mbgl/gfx/drawable_tweaker.hpp>
#include <mbgl/shaders/line_layer_ubo.hpp>
#include <mbgl/util/string_indexer.hpp>
#include <mbgl/util/convert.hpp>
#include <mbgl/util/geometry.hpp>
#include <mbgl/programs/fill_program.hpp>
#include <mbgl/shaders/fill_layer_ubo.hpp>
#include <mbgl/util/math.hpp>
#include <mbgl/tile/geometry_tile_data.hpp>
#include <mbgl/util/containers.hpp>
#include <mbgl/gfx/fill_generator.hpp>
#include <mbgl/shaders/custom_drawable_layer_ubo.hpp>

namespace mbgl {

namespace style {

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
    ~LineDrawableTweaker() override = default;

    void init(gfx::Drawable&) override{};

    void execute(gfx::Drawable& drawable, const PaintParameters& parameters) override {
        if (!drawable.getTileID().has_value()) {
            return;
        }

        const UnwrappedTileID tileID = drawable.getTileID()->toUnwrapped();
        const auto zoom = parameters.state.getZoom();
        mat4 tileMatrix;
        parameters.state.matrixFor(/*out*/ tileMatrix, tileID);

        const auto matrix = LayerTweaker::getTileMatrix(
            tileID, parameters, {{0, 0}}, style::TranslateAnchorType::Viewport, false, false, false);

        static const StringIdentity idLineDynamicUBOName = stringIndexer().get("LineDynamicUBO");
        const shaders::LineDynamicUBO dynamicUBO = {
            /*units_to_pixels = */ {1.0f / parameters.pixelsToGLUnits[0], 1.0f / parameters.pixelsToGLUnits[1]}, 0, 0};

        static const StringIdentity idLineUBOName = stringIndexer().get("LineUBO");
        const shaders::LineUBO lineUBO{/*matrix = */ util::cast<float>(matrix),
                                       /*ratio = */ 1.0f / tileID.pixelsToTileUnits(1.0f, zoom),
                                       0,
                                       0,
                                       0};

        static const StringIdentity idLinePropertiesUBOName = stringIndexer().get("LinePropertiesUBO");

        static const StringIdentity idLineInterpolationUBOName = stringIndexer().get("LineInterpolationUBO");
        const shaders::LineInterpolationUBO lineInterpolationUBO{/*color_t =*/0.f,
                                                                 /*blur_t =*/0.f,
                                                                 /*opacity_t =*/0.f,
                                                                 /*gapwidth_t =*/0.f,
                                                                 /*offset_t =*/0.f,
                                                                 /*width_t =*/0.f,
                                                                 0,
                                                                 0};
        auto& uniforms = drawable.mutableUniformBuffers();
        uniforms.createOrUpdate(idLineDynamicUBOName, &dynamicUBO, parameters.context);
        uniforms.createOrUpdate(idLineUBOName, &lineUBO, parameters.context);
        uniforms.createOrUpdate(idLinePropertiesUBOName, &linePropertiesUBO, parameters.context);
        uniforms.createOrUpdate(idLineInterpolationUBOName, &lineInterpolationUBO, parameters.context);
    };

private:
    shaders::LinePropertiesUBO linePropertiesUBO;
};

class FillDrawableTweaker : public gfx::DrawableTweaker {
public:
    FillDrawableTweaker(const Color& color_, float opacity_)
        : color(color_),
          opacity(opacity_) {}
    ~FillDrawableTweaker() override = default;

    void init(gfx::Drawable&) override{};

    void execute(gfx::Drawable& drawable, const PaintParameters& parameters) override {
        if (!drawable.getTileID().has_value()) {
            return;
        }

        const UnwrappedTileID tileID = drawable.getTileID()->toUnwrapped();
        mat4 tileMatrix;
        parameters.state.matrixFor(/*out*/ tileMatrix, tileID);

        const auto matrix = LayerTweaker::getTileMatrix(
            tileID, parameters, {{0, 0}}, style::TranslateAnchorType::Viewport, false, false, false);

        static const StringIdentity idFillDrawableUBOName = stringIndexer().get("FillDrawableUBO");
        const shaders::FillDrawableUBO fillUBO{/*matrix = */ util::cast<float>(matrix)};

        static const StringIdentity idFillEvaluatedPropsUBOName = stringIndexer().get("FillEvaluatedPropsUBO");
        const shaders::FillEvaluatedPropsUBO fillPropertiesUBO{
            /* .color = */ color,
            /* .opacity = */ opacity,
            0,
            0,
            0,
        };

        static const StringIdentity idFillInterpolateUBOName = stringIndexer().get("FillInterpolateUBO");
        const shaders::FillInterpolateUBO fillInterpolateUBO{
            /* .color_t = */ 0.f,
            /* .opacity_t = */ 0.f,
            0,
            0,
        };
        auto& uniforms = drawable.mutableUniformBuffers();
        uniforms.createOrUpdate(idFillDrawableUBOName, &fillUBO, parameters.context);
        uniforms.createOrUpdate(idFillEvaluatedPropsUBOName, &fillPropertiesUBO, parameters.context);
        uniforms.createOrUpdate(idFillInterpolateUBOName, &fillInterpolateUBO, parameters.context);
    };

private:
    Color color;
    float opacity;
};

class SymbolDrawableTweaker : public gfx::DrawableTweaker {
public:
    SymbolDrawableTweaker(const CustomDrawableLayerHost::Interface::SymbolOptions& options_)
        : options(options_) {}
    ~SymbolDrawableTweaker() override = default;

    void init(gfx::Drawable&) override{};

    void execute(gfx::Drawable& drawable, const PaintParameters& parameters) override {
        if (!drawable.getTileID().has_value()) {
            return;
        }

        const UnwrappedTileID tileID = drawable.getTileID()->toUnwrapped();
        mat4 tileMatrix;
        parameters.state.matrixFor(/*out*/ tileMatrix, tileID);

        const auto matrix = LayerTweaker::getTileMatrix(
            tileID, parameters, {{0, 0}}, style::TranslateAnchorType::Viewport, false, false, false);

        static const StringIdentity idDrawableUBOName = stringIndexer().get("CustomSymbolIconDrawableUBO");
        const shaders::CustomSymbolIconDrawableUBO drawableUBO{/*matrix = */ util::cast<float>(matrix)};

        const auto pixelsToTileUnits = tileID.pixelsToTileUnits(1.0f, options.scaleWithMap ? tileID.canonical.z : parameters.state.getZoom());
        const auto f = options.scaleWithMap ? std::pow(2.f, parameters.state.getZoom() - tileID.canonical.z) : 1.0f;
        const auto extrudeScale = options.pitchWithMap ? std::array<float, 2>{pixelsToTileUnits, pixelsToTileUnits}
            : std::array<float, 2>{parameters.pixelsToGLUnits[0] * f, parameters.pixelsToGLUnits[1] * f};
        
        static const StringIdentity idParametersUBOName = stringIndexer().get("CustomSymbolIconParametersUBO");
        const shaders::CustomSymbolIconParametersUBO parametersUBO{
            /*extrude_scale*/               {extrudeScale[0] * options.size.width, extrudeScale[1] * options.size.height},
            /*anchor*/                      options.anchor,
            /*angle_degrees*/               options.angleDegrees,
            /*scale_with_map*/              options.scaleWithMap,
            /*pitch_with_map*/              options.pitchWithMap,
            /*camera_to_center_distance*/   parameters.state.getCameraToCenterDistance()};

        // set UBOs
        auto& uniforms = drawable.mutableUniformBuffers();
        uniforms.createOrUpdate(idDrawableUBOName, &drawableUBO, parameters.context);
        uniforms.createOrUpdate(idParametersUBOName, &parametersUBO, parameters.context);
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

void CustomDrawableLayerHost::Interface::addPolyline(const GeometryCoordinates& coordinates) {
    if (!lineShader) lineShader = lineShaderDefault();
    assert(lineShader);
    if (!builder || builder->getShader() != lineShader) {
        finish();
        builder = createBuilder("lines", lineShader);
    }
    assert(builder);
    assert(builder->getShader() == lineShader);
    builder->addPolyline(coordinates, lineOptions.geometry);
}

void CustomDrawableLayerHost::Interface::addFill(const GeometryCollection& geometry) {
    if (!fillShader) fillShader = fillShaderDefault();
    assert(fillShader);
    if (!builder || builder->getShader() != fillShader) {
        finish();
        builder = createBuilder("fill", fillShader);
    }
    assert(builder);
    assert(builder->getShader() == fillShader);

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
    static const StringIdentity idVertexAttribName = stringIndexer().get("a_pos");
    builder->setVertexAttrNameId(idVertexAttribName);

    auto attrs = context.createVertexAttributeArray();
    if (const auto& attr = attrs->add(idVertexAttribName)) {
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
    if (!symbolShader) symbolShader = symbolShaderDefault();
    assert(symbolShader);
    if (!builder || builder->getShader() != symbolShader) {
        finish();
        builder = createBuilder("symbol", symbolShader);
    }
    assert(builder);
    assert(builder->getShader() == symbolShader);

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
    static const StringIdentity idPositionAttribName = stringIndexer().get("a_pos");
    static const StringIdentity idTextureAttribName = stringIndexer().get("a_tex");
    builder->setVertexAttrNameId(idPositionAttribName);

    auto attrs = context.createVertexAttributeArray();
    if (const auto& attr = attrs->add(idPositionAttribName)) {
        attr->setSharedRawData(sharedVertices,
                               offsetof(CustomSymbolIcon, a_pos),
                               /*vertexOffset=*/0,
                               sizeof(CustomSymbolIcon),
                               gfx::AttributeDataType::Float2);
    }
    if (const auto& attr = attrs->add(idTextureAttribName)) {
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
        static const StringIdentity idTextureUniformName = stringIndexer().get("u_texture");
        if (auto textureLocation = builder->getShader()->getSamplerLocation(idTextureUniformName);
            textureLocation.has_value()) {
            builder->setTexture(symbolOptions.texture, *textureLocation);
        }
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
                drawable->setTileID(tileID.value());
                if (tweaker) {
                    drawable->addTweaker(tweaker);
                }

                TileLayerGroup* tileLayerGroup = static_cast<TileLayerGroup*>(layerGroup.get());
                tileLayerGroup->addDrawable(RenderPass::Translucent, tileID.value(), std::move(drawable));
            }
        };

        if (builder->getShader() == lineShader) {
            // finish building lines

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
        } else if (builder->getShader() == fillShader) {
            // finish building fills

            // create fill tweaker
            auto tweaker = std::make_shared<FillDrawableTweaker>(fillOptions.color, fillOptions.opacity);

            // finish drawables
            finish_(tweaker);
        } else if (builder->getShader() == symbolShader) {
            // finish building symbols

            // finish drawables
            finish_(nullptr);
        }
    }
}

gfx::ShaderPtr CustomDrawableLayerHost::Interface::lineShaderDefault() const {
    gfx::ShaderGroupPtr shaderGroup = shaders.getShaderGroup("LineShader");

    const mbgl::unordered_set<StringIdentity> propertiesAsUniforms{
        stringIndexer().get("a_color"),
        stringIndexer().get("a_blur"),
        stringIndexer().get("a_opacity"),
        stringIndexer().get("a_gapwidth"),
        stringIndexer().get("a_offset"),
        stringIndexer().get("a_width"),
    };

    return shaderGroup->getOrCreateShader(context, propertiesAsUniforms);
}

gfx::ShaderPtr CustomDrawableLayerHost::Interface::fillShaderDefault() const {
    gfx::ShaderGroupPtr shaderGroup = shaders.getShaderGroup("FillShader");

    const mbgl::unordered_set<StringIdentity> propertiesAsUniforms{
        stringIndexer().get("a_color"),
        stringIndexer().get("a_opacity"),
    };

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
