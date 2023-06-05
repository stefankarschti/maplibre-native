#pragma once

#include <mbgl/gfx/drawable_impl.hpp>
#include <mbgl/gfx/index_buffer.hpp>
#include <mbgl/gfx/program.hpp>
#include <mbgl/gfx/uniform.hpp>
#include <mbgl/gl/defines.hpp>
#include <mbgl/gl/enum.hpp>
#include <mbgl/gl/program.hpp>
#include <mbgl/gl/uniform_buffer_gl.hpp>
#include <mbgl/gl/vertex_array.hpp>
#include <mbgl/platform/gl_functions.hpp>
#include <mbgl/programs/segment.hpp>
#include <mbgl/renderer/paint_parameters.hpp>
#include <mbgl/util/mat4.hpp>

#include <cstdint>
#include <memory>
#include <vector>

namespace mbgl {
namespace gl {

using namespace platform;

class DrawableGL::Impl final {
public:
    Impl() = default;
    ~Impl() = default;

    std::vector<UniqueDrawSegment> segments;

    std::vector<TextureID> textures;

    std::vector<std::uint16_t> indexes;

    VertexAttributeArrayGL vertexAttributes;

    gfx::IndexBuffer indexBuffer = {0, nullptr};
    gfx::UniqueVertexBufferResource attributeBuffer;

    UniformBufferArrayGL uniformBuffers;

    gfx::DepthMode depthMode = gfx::DepthMode::disabled();
    gfx::StencilMode stencilMode;
    gfx::ColorMode colorMode;
    gfx::CullFaceMode cullFaceMode;
    GLfloat pointSize = 0.0f;
};

struct DrawableGL::DrawSegmentGL final : public gfx::Drawable::DrawSegment {
    DrawSegmentGL(gfx::DrawMode mode_, Segment<void>&& segment_, VertexArray&& vertexArray_)
        : gfx::Drawable::DrawSegment(mode_, std::move(segment_)),
          vertexArray(std::move(vertexArray_)) {}

    const VertexArray& getVertexArray() const { return vertexArray; }
    void setVertexArray(VertexArray&& value) { vertexArray = std::move(value); }

protected:
    VertexArray vertexArray;
};

} // namespace gl
} // namespace mbgl