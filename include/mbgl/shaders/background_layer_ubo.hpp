#pragma once

#include <mbgl/shaders/layer_ubo.hpp>

#include <memory>

namespace mbgl {
namespace shaders {

struct alignas(16) BackgroundDrawableUBO {
    std::array<float, 4 * 4> matrix;
};
static_assert(sizeof(BackgroundDrawableUBO) % 16 == 0);

struct alignas(16) BackgroundLayerUBO {
    /*  0 */ Color color;
    /* 16 */ float opacity;
    // overdrawInspector is used only in Metal, while in GL this is a 16 bytes empty padding.
    /* 20 */ bool overdrawInspector;
    /* 21 */ uint8_t pad1, pad2, pad3;
    /* 24 */ float pad4, pad5;
    /* 32 */
};
static_assert(sizeof(BackgroundLayerUBO) == 32);

struct alignas(16) BackgroundPatternLayerUBO {
    /*  0 */ std::array<float, 2> pattern_tl_a;
    /*  8 */ std::array<float, 2> pattern_br_a;
    /* 16 */ std::array<float, 2> pattern_tl_b;
    /* 24 */ std::array<float, 2> pattern_br_b;
    /* 32 */ std::array<float, 2> texsize;
    /* 40 */ std::array<float, 2> pattern_size_a;
    /* 48 */ std::array<float, 2> pattern_size_b;
    /* 56 */ std::array<float, 2> pixel_coord_upper;
    /* 64 */ std::array<float, 2> pixel_coord_lower;
    /* 72 */ float tile_units_to_pixels;
    /* 76 */ float scale_a;
    /* 80 */ float scale_b;
    /* 84 */ float mix;
    /* 88 */ float opacity;
    // overdrawInspector is used only in Metal, while in GL this is a 4 bytes empty padding.
    /* 92 */ bool overdrawInspector;
    /* 93 */ uint8_t pad1, pad2, pad3;
    /* 96 */
};
static_assert(sizeof(BackgroundPatternLayerUBO) == 96);

} // namespace shaders
} // namespace mbgl
