#pragma once

#include <mbgl/gfx/backend.hpp>
#include <mbgl/gfx/command_encoder.hpp>
#include <mbgl/gfx/draw_scope.hpp>
#include <mbgl/gfx/program.hpp>
#include <mbgl/gfx/renderbuffer.hpp>
#include <mbgl/gfx/rendering_stats.hpp>
#include <mbgl/gfx/texture.hpp>
#include <mbgl/gfx/types.hpp>

namespace mbgl {

class PaintParameters;
class ProgramParameters;

namespace gfx {

class Drawable;
class DrawableBuilder;
class DrawableTweaker;
class OffscreenTexture;
class ShaderProgramBase;

using DrawablePtr = std::shared_ptr<Drawable>;
using UniqueDrawableBuilder = std::unique_ptr<DrawableBuilder>;
using DrawableTweakerPtr = std::shared_ptr<DrawableTweaker>;
using ShaderProgramBasePtr = std::shared_ptr<ShaderProgramBase>;


class Context {
protected:
    Context(uint32_t maximumVertexBindingCount_)
        : maximumVertexBindingCount(maximumVertexBindingCount_) {
    }

public:
    static constexpr const uint32_t minimumRequiredVertexBindingCount = 8;
    const uint32_t maximumVertexBindingCount;

public:
    Context(Context&&) = delete;
    Context(const Context&) = delete;
    Context& operator=(Context&& other) = delete;
    Context& operator=(const Context& other) = delete;
    virtual ~Context() = default;

public:
    // Called at the end of a frame.
    virtual void performCleanup() = 0;

    // Called when the app receives a memory warning and before it goes to the background.
    virtual void reduceMemoryUsage() = 0;

public:
    virtual std::unique_ptr<OffscreenTexture> createOffscreenTexture(Size, TextureChannelDataType) = 0;

public:
    // Creates an empty texture with the specified dimensions.
    Texture createTexture(const Size size,
                          TexturePixelType format = TexturePixelType::RGBA,
                          TextureChannelDataType type = TextureChannelDataType::UnsignedByte) {
        return { size, createTextureResource(size, format, type) };
    }

protected:
    virtual std::unique_ptr<TextureResource>
        createTextureResource(Size, TexturePixelType, TextureChannelDataType) = 0;

public:
    template <RenderbufferPixelType pixelType>
    Renderbuffer<pixelType>
    createRenderbuffer(const Size size) {
        return { size, createRenderbufferResource(pixelType, size) };
    }

protected:
    virtual std::unique_ptr<RenderbufferResource>
    createRenderbufferResource(RenderbufferPixelType, Size) = 0;

public:
    DrawScope createDrawScope() {
        return DrawScope{ createDrawScopeResource() };
    }

protected:
    virtual std::unique_ptr<DrawScopeResource> createDrawScopeResource() = 0;

public:
    virtual std::unique_ptr<CommandEncoder> createCommandEncoder() = 0;

    virtual const RenderingStats& renderingStats() const = 0;

#if !defined(NDEBUG)
public:
    virtual void visualizeStencilBuffer() = 0;
    virtual void visualizeDepthBuffer(float depthRangeSize) = 0;
#endif

    virtual void clearStencilBuffer(int32_t) = 0;
    
    
public:
    /// Activate the shader, vertex attributes, etc., specified by the drawable
    virtual bool setupDraw(const PaintParameters&, const gfx::Drawable&) = 0;

    /// Create a new drawable builder
    virtual UniqueDrawableBuilder createDrawableBuilder(std::string name) = 0;
    
    /// Create a new drawable tweaker
    virtual DrawableTweakerPtr createDrawableTweaker() = 0;
};

} // namespace gfx
} // namespace mbgl
