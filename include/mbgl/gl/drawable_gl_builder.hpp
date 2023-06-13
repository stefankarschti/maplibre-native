#pragma once

#include <mbgl/gfx/drawable_builder.hpp>
#include <mbgl/gl/vertex_attribute_gl.hpp>

namespace mbgl {
namespace gl {

/**
    Base class for OpenGL-specific drawable builders.
 */
class DrawableGLBuilder final : public gfx::DrawableBuilder {
public:
    DrawableGLBuilder(std::string name_)
        : gfx::DrawableBuilder(std::move(name_)) {}
    ~DrawableGLBuilder() override = default;

    const gfx::VertexAttributeArray& getVertexAttributes() const override { return vertexAttributes; }
    void setVertexAttributes(const VertexAttributeArrayGL& value) { vertexAttributes = value; }
    void setVertexAttributes(VertexAttributeArrayGL&& value) { vertexAttributes = std::move(value); }

protected:
    gfx::UniqueDrawable createDrawable() const override;

    using DrawSegment = gfx::Drawable::DrawSegment;
    std::unique_ptr<DrawSegment> createSegment(gfx::DrawMode, SegmentBase&&) override;

    /// Setup the SDK-specific aspects after all the values are present
    void init() override;

private:
    VertexAttributeArrayGL vertexAttributes;
};

} // namespace gl
} // namespace mbgl
