// Generated code, do not modify this file!
// Generated on 2023-05-16T12:04:47.554Z by stefan using shaders/generate_shader_code.js

#pragma once
#include <mbgl/shaders/shader_source.hpp>

namespace mbgl {
namespace shaders {

template <>
struct ShaderSource<BuiltIn::BackgroundShader, gfx::Backend::Type::OpenGL> {
    static constexpr const char* name = "BackgroundShader";
    static constexpr const char* vertex = R"(layout (location = 0) in vec2 a_pos;
layout (std140) uniform BackgroundDrawableUBO {
    mat4 u_matrix;
};

void main() {
    gl_Position = u_matrix * vec4(a_pos, 0, 1);
}
)";
    static constexpr const char* fragment = R"(layout (std140) uniform BackgroundLayerUBO {
    vec4 u_color;
    vec4 u_opacity_pad3;
};

void main() {
    fragColor = u_color * u_opacity_pad3.x;
#ifdef OVERDRAW_INSPECTOR
    fragColor = vec4(1.0);
#endif
}
)";
};

} // namespace shaders
} // namespace mbgl
