#pragma once

#include <mbgl/gfx/drawable.hpp>
#include <mbgl/gl/vertex_attribute_gl.hpp>

#include <memory>

namespace mbgl {

namespace gfx {

class IndexBuffer;
class VertexBufferResource;

using UniqueVertexBufferResource = std::unique_ptr<gfx::VertexBufferResource>;

} // namespace gfx

namespace gl {

class VertexArray;

class DrawableGL : public gfx::Drawable {
public:
    DrawableGL();
    virtual ~DrawableGL();

    void draw(const PaintParameters &) const override;

    void setVertexData(std::vector<std::uint8_t> vertexData, std::vector<uint16_t> indexes,
                       std::size_t indexOffset, std::size_t indexLength);
    std::vector<std::uint16_t>& getIndexData() const override;
    std::vector<std::uint8_t>& getVertexData() const override;

    const gfx::VertexAttributeArray& getVertexAttributes() const override { return vertexAttributes; }
    void setVertexAttributes(const gfx::VertexAttributeArray& value) override {
        vertexAttributes = static_cast<const VertexAttributeArrayGL&>(value);
    }
    void setVertexAttributes(gfx::VertexAttributeArray&& value) override {
        vertexAttributes = std::move(static_cast<VertexAttributeArrayGL&&>(value));
    }

    const gl::VertexArray& getVertexArray() const;
    void setVertexArray(gl::VertexArray&&, gfx::UniqueVertexBufferResource&&,
                        gfx::IndexBuffer&&, std::size_t attrOffset);

protected:
    class Impl;
    const std::unique_ptr<Impl> impl;

    // For testing only.
    DrawableGL(std::unique_ptr<Impl>);
    
private:
    VertexAttributeArrayGL vertexAttributes;
};

} // namespace gl
} // namespace mbgl
