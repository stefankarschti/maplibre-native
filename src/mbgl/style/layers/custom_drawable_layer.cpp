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
#include <mbgl/util/math.hpp>
#include <mbgl/tile/geometry_tile_data.hpp>

// fill_bucket.cpp
#include <mbgl/renderer/buckets/fill_bucket.hpp>
#include <mbgl/programs/fill_program.hpp>
#include <mbgl/renderer/bucket_parameters.hpp>
#include <mbgl/style/layers/fill_layer_impl.hpp>
#include <mbgl/renderer/layers/render_fill_layer.hpp>
#include <mbgl/util/math.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#endif

#include <mapbox/earcut.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <cassert>

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

        static const StringIdentity idLineUBOName = stringIndexer().get("LineUBO");
        const shaders::LineUBO lineUBO{
            /*matrix = */ util::cast<float>(matrix),
            /*units_to_pixels = */ {1.0f / parameters.pixelsToGLUnits[0], 1.0f / parameters.pixelsToGLUnits[1]},
            /*ratio = */ 1.0f / tileID.pixelsToTileUnits(1.0f, zoom),
            /*device_pixel_ratio = */ parameters.pixelRatio};

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
        uniforms.createOrUpdate(idLineUBOName, &lineUBO, parameters.context);
        uniforms.createOrUpdate(idLinePropertiesUBOName, &linePropertiesUBO, parameters.context);
        uniforms.createOrUpdate(idLineInterpolationUBOName, &lineInterpolationUBO, parameters.context);

#if MLN_RENDER_BACKEND_METAL
        static const StringIdentity idExpressionInputsUBOName = stringIndexer().get("ExpressionInputsUBO");
        const auto expressionUBO = LayerTweaker::buildExpressionUBO(zoom, parameters.frameCount);
        uniforms.createOrUpdate(idExpressionInputsUBOName, &expressionUBO, parameters.context);

        static const StringIdentity idLinePermutationUBOName = stringIndexer().get("LinePermutationUBO");
        const shaders::LinePermutationUBO permutationUBO = {
            /* .color = */ {/*.source=*/shaders::AttributeSource::Constant, /*.expression=*/{}},
            /* .blur = */ {/*.source=*/shaders::AttributeSource::Constant, /*.expression=*/{}},
            /* .opacity = */ {/*.source=*/shaders::AttributeSource::Constant, /*.expression=*/{}},
            /* .gapwidth = */ {/*.source=*/shaders::AttributeSource::Constant, /*.expression=*/{}},
            /* .offset = */ {/*.source=*/shaders::AttributeSource::Constant, /*.expression=*/{}},
            /* .width = */ {/*.source=*/shaders::AttributeSource::Constant, /*.expression=*/{}},
            /* .floorwidth = */ {/*.source=*/shaders::AttributeSource::Constant, /*.expression=*/{}},
            /* .pattern_from = */ {/*.source=*/shaders::AttributeSource::Constant, /*.expression=*/{}},
            /* .pattern_to = */ {/*.source=*/shaders::AttributeSource::Constant, /*.expression=*/{}},
            /* .overdrawInspector = */ false,
            /* .pad = */ 0,
            0,
            0,
            0};
        uniforms.createOrUpdate(idLinePermutationUBOName, &permutationUBO, parameters.context);
#endif // MLN_RENDER_BACKEND_METAL
    };

private:
    shaders::LinePropertiesUBO linePropertiesUBO;
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

void CustomDrawableLayerHost::Interface::setColor(Color color) {
    if (currentColor != color) finish();
    currentColor = color;
}

void CustomDrawableLayerHost::Interface::setBlur(float blur) {
    if (currentBlur != blur) finish();
    currentBlur = blur;
}

void CustomDrawableLayerHost::Interface::setOpacity(float opacity) {
    if (currentOpacity != opacity) finish();
    currentOpacity = opacity;
}

void CustomDrawableLayerHost::Interface::setGapWidth(float gapWidth) {
    if (currentGapWidth != gapWidth) finish();
    currentGapWidth = gapWidth;
}

void CustomDrawableLayerHost::Interface::setOffset(float offset) {
    if (currentOffset != offset) finish();
    currentOffset = offset;
}

void CustomDrawableLayerHost::Interface::setWidth(float width) {
    if (currentWidth != width) finish();
    currentWidth = width;
}

void CustomDrawableLayerHost::Interface::addPolyline(const GeometryCoordinates& coordinates,
                                                     const gfx::PolylineGeneratorOptions& options) {
    if (!lineShader) lineShader = lineShaderDefault();
    assert(lineShader);
    if (!builder || builder->getShader() != lineShader) {
        builder = createBuilder("lines", lineShader);
    }
    assert(builder);
    assert(builder->getShader() == lineShader);
    builder->addPolyline(coordinates, options);
}

void CustomDrawableLayerHost::Interface::addFill(const GeometryCollection& geometry) {
    if (!fillShader) fillShader = fillShaderDefault();
    assert(fillShader);
    if (!builder || builder->getShader() != fillShader) {
        builder = createBuilder("fill", fillShader);
    }
    assert(builder);
    assert(builder->getShader() == fillShader);

    // build the fill vertices, indexes and segments
    using VertexVector = gfx::VertexVector<FillLayoutVertex>;
    const std::shared_ptr<VertexVector> sharedVertices = std::make_shared<VertexVector>();
    VertexVector& vertices = *sharedVertices;

    using TriangleIndexVector = gfx::IndexVector<gfx::Triangles>;
    const std::shared_ptr<TriangleIndexVector> sharedTriangles = std::make_shared<TriangleIndexVector>();
    TriangleIndexVector& triangles = *sharedTriangles;

    SegmentVector<FillAttributes> triangleSegments;

    for (auto& polygon : classifyRings(geometry)) {
        std::size_t startVertices = vertices.elements();
        std::size_t totalVertices = 0;

        for (const auto& ring : polygon) {
            std::size_t nVertices = ring.size();
            if (nVertices == 0) continue;

            for (std::size_t i = 0; i < nVertices; ++i) {
                vertices.emplace_back(FillProgram::layoutVertex(ring[i]));
            }
        }

        std::vector<uint32_t> indices = mapbox::earcut(polygon);

        std::size_t nIndices = indices.size();
        assert(nIndices % 3 == 0);

        if (triangleSegments.empty() ||
            triangleSegments.back().vertexLength + totalVertices > std::numeric_limits<uint16_t>::max()) {
            triangleSegments.emplace_back(startVertices, triangles.elements());
        }

        auto& triangleSegment = triangleSegments.back();
        assert(triangleSegment.vertexLength <= std::numeric_limits<uint16_t>::max());
        const auto triangleIndex = static_cast<uint16_t>(triangleSegment.vertexLength);

        for (std::size_t i = 0; i < nIndices; i += 3) {
            triangles.emplace_back(
                triangleIndex + indices[i], triangleIndex + indices[i + 1], triangleIndex + indices[i + 2]);
        }

        triangleSegment.vertexLength += totalVertices;
        triangleSegment.indexLength += nIndices;
    }

    // add to builder
    static const StringIdentity idVertexAttribName = stringIndexer().get("a_pos");
    builder->setVertexAttrNameId(idVertexAttribName);
    gfx::VertexAttributeArray attrs;
    if (const auto& attr = attrs.add(idVertexAttribName)) {
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
    builder->flush();
}

void CustomDrawableLayerHost::Interface::finish() {
    if (builder && !builder->empty()) {
        if (builder->getShader() == lineShader) {
            // finish building lines

            // create line tweaker
            const shaders::LinePropertiesUBO linePropertiesUBO{
                currentColor, currentBlur, currentOpacity, currentGapWidth, currentOffset, currentWidth, 0, 0, 0};
            auto tweaker = std::make_shared<LineDrawableTweaker>(linePropertiesUBO);

            // finish drawables
            builder->flush();
            for (auto& drawable : builder->clearDrawables()) {
                assert(tileID.has_value());
                drawable->setTileID(tileID.value());
                drawable->addTweaker(tweaker);

                TileLayerGroup* tileLayerGroup = static_cast<TileLayerGroup*>(layerGroup.get());
                tileLayerGroup->addDrawable(RenderPass::Translucent, tileID.value(), std::move(drawable));
            }
        } else if (builder->getShader() == fillShader) {
            // finish building fills

            // TODO: create fill tweaker

            // TODO: finish emitting drawables
        }
    }
}

gfx::ShaderPtr CustomDrawableLayerHost::Interface::lineShaderDefault() const {
    gfx::ShaderGroupPtr lineShaderGroup = shaders.getShaderGroup("LineShader");

    const std::unordered_set<StringIdentity> propertiesAsUniforms{
        stringIndexer().get("a_color"),
        stringIndexer().get("a_blur"),
        stringIndexer().get("a_opacity"),
        stringIndexer().get("a_gapwidth"),
        stringIndexer().get("a_offset"),
        stringIndexer().get("a_width"),
    };

    return lineShaderGroup->getOrCreateShader(context, propertiesAsUniforms);
}

gfx::ShaderPtr CustomDrawableLayerHost::Interface::fillShaderDefault() const {
    gfx::ShaderGroupPtr fillShaderGroup = shaders.getShaderGroup("FillShader");

    const std::unordered_set<StringIdentity> propertiesAsUniforms{
        stringIndexer().get("a_color"),
        stringIndexer().get("a_opacity"),
    };

    return fillShaderGroup->getOrCreateShader(context, propertiesAsUniforms);
}

std::unique_ptr<gfx::DrawableBuilder> CustomDrawableLayerHost::Interface::createBuilder(const std::string& name,
                                                                                        gfx::ShaderPtr shader) const {
    std::unique_ptr<gfx::DrawableBuilder> builder_ = context.createDrawableBuilder(name);
    builder_->setShader(std::static_pointer_cast<gfx::ShaderProgramBase>(shader));
    builder_->setSubLayerIndex(0);
    builder_->setEnableDepth(false);
    builder_->setColorMode(gfx::ColorMode::alphaBlended());
    builder_->setCullFaceMode(gfx::CullFaceMode::disabled());
    builder_->setEnableStencil(false);
    builder_->setRenderPass(RenderPass::Translucent);

    return builder_;
}

} // namespace style
} // namespace mbgl
