# PR #4551: SDF fill-pattern implementation review

**PR:** [Core: Add support for colorized SDF fill patterns](https://github.com/maplibre/maplibre-native/pull/4551)  
**Review date:** 2026-10-02  
**Head:** `19df9aea9576f9cb13df3fbb7ac4c50da1dcc4f0` (`sdf-fill-pattern`)  
**Base:** `ba9fc57bc88725d7602fd652e601384e9f2e3183`  
**Assessment:** One correctness finding, P2, with high confidence from source inspection. Resolve the image-replacement behavior before merging. Runtime reproduction remains outstanding.

## Finding

### F01 — P2: Replacing an image with a same-sized image of the other SDF type leaves stale atlas pixels

**Changed location:** [src/mln/renderer/image_manager.cpp:68–78](../../src/mln/renderer/image_manager.cpp#L68).

When an existing image is replaced with different pixels, the same dimensions, and a changed `sdf` flag, the new `layoutChanged` condition enters the branch that erases `updatedImageVersions`. This triggers a bucket rebuild but suppresses the texture patch normally used for same-sized image updates. The dynamic atlas can reuse the existing allocation without uploading the replacement. The new bucket then renders the old image data using the new SDF interpretation.

This is a regression introduced by including `sdfChanged` in the branch previously reserved for size changes. An ordinary same-sized replacement previously incremented the image version and reached the texture-patching path.

The complete source path is:

1. [ImageManager::updateImage](../../src/mln/renderer/image_manager.cpp#L67) returns a layout change and erases the version entry when only the SDF type changes.
2. [RenderOrchestrator](../../src/mln/renderer/render_orchestrator.cpp#L272) removes the entry from `PatternAtlas` and requests relayout. Geometry tiles obtain their textures from `DynamicTextureAtlas`, whose allocation is not invalidated by that removal.
3. [GeometryTileWorker::finalizeLayout](../../src/mln/tile/geometry_tile_worker.cpp#L568) requests the new atlas before handing the new layout to the tile. The tile still owns its previous layout at this point; it replaces that layout in [GeometryTile::onLayout](../../src/mln/tile/geometry_tile.cpp#L356). Atlas references are released by the [old layout's destructor](../../src/mln/tile/geometry_tile.cpp#L166).
4. [DynamicTextureAtlas](../../src/mln/gfx/dynamic_texture_atlas.cpp#L162) computes the pattern allocation ID from the image ID and pixel area. Both are unchanged, so it reserves the already-referenced allocation. [TextureHandle](../../include/mln/gfx/dynamic_texture.hpp#L22) marks uploads necessary only when the allocation's reference count is one. The [conditional upload](../../src/mln/gfx/dynamic_texture_atlas.cpp#L215) is therefore skipped.
5. [populateImagePatches](../../src/mln/tile/geometry_tile.cpp#L55) only patches images present in `updatedImageVersions`. The erased entry prevents this fallback from uploading the new pixels. Meanwhile, the rebuilt `ImagePosition` and fill bucket do receive the new SDF flag.

**Observable scenario:** Render an opaque RGBA pattern under image ID `p`. After a completed frame, replace `p` with an SDF stripe image of identical dimensions and set its SDF flag to true. Keep the layer visible and use a red `fill-color`. The source path above produces a solid red region from the old opaque alpha channel instead of the replacement stripe silhouette. The reverse SDF-to-RGBA replacement has the same stale-pixel problem. This affects the shared image/atlas path across rendering backends and can also affect symbols using the replaced image.

**Coverage:** The new [SDFChangeRequiresRelayout test](../../test/renderer/image_manager.test.cpp#L68) checks the return value, the stored SDF flag, and an empty version map. It never allocates an atlas or observes uploaded pixels, so it cannot detect this failure. Both test images also contain the same zero-initialized pixel data.

**Regression case to validate:** Render once, replace the same image ID with visibly different pixels and the opposite SDF flag while preserving dimensions, wait for relayout, and compare the rendered result with a fresh map initialized directly with the replacement image. Exercise both directions. This recipe has not been executed in this review.

## Coverage gaps

These are validation gaps, not additional confirmed rendering defects.

- **The per-layer fixture does not exercise shared buckets.** Its two layers use [different `fill-pattern` values](../../metrics/integration/render-tests/fill-pattern-sdf/per-layer/style.json#L27). [LayoutGroupKey equality](../../src/mln/renderer/group_by_layout.cpp#L32) delegates to [FillLayer::Impl::hasLayoutDifference](../../src/mln/style/layers/fill_layer_impl.cpp#L6), which treats different pattern values as a layout difference. Consequently, the fixture exercises two buckets. The [unit test](../../test/renderer/fill_bucket.test.cpp#L10) directly populates the metadata map, proving per-layer storage but not propagation through layout and rendering.
- **Data-driven pattern selection is untested by the new render fixtures.** The fixture named [data-driven](../../metrics/integration/render-tests/fill-pattern-sdf/data-driven/style.json#L39) varies `fill-color` while retaining a literal pattern. None of the four new styles exercises the [per-feature pattern dependency recording](../../src/mln/layout/pattern_layout.hpp#L220), multiple SDF pattern IDs, or zoom-dependent pattern selection.
- **The changed uniform-buffer stride needs multi-drawable regression coverage.** The CPU fill union grows to [112 bytes](../../include/mln/shaders/fill_layer_ubo.hpp#L127), including for solid fills that share the union. Metal's corresponding union and Vulkan/WebGPU padding appear consistent in source. The new default-zoom fixtures do not establish correct indexing across multiple tiles, solid fills, and triangulated outlines.
- **Outline coverage is narrow.** The literal fixture enables antialiasing, but the new fixtures do not cover pitched views, fractional zoom, data-driven color with antialiasing enabled, or partially transparent foreground colors.

## Implementation assessment

The initial-render path is coherent: `ImagePosition` preserves the SDF flag; `PatternLayout` records constant and per-feature dependencies; `FillLayerTweaker` supplies the bucket's per-layer flag and color interpolation factor; and both pattern shaders consume foreground color and opacity on all four backends. The attribute declarations and backend attribute tables were checked together. No additional source-level mismatch was found in those mappings or in the expanded drawable-buffer layouts.

The non-SDF shader branches retain their existing color calculation. On Metal and WebGPU, the newly applied outline alpha is confined to the SDF branch. This supports the intended RGBA compatibility at the shader level; it is not a substitute for rendering the existing regression suite.

Mixed SDF/RGBA patterns within one layer remain unsupported by design. The [feature request](https://github.com/maplibre/maplibre-native/issues/4526) explicitly permits an initial warning-based limitation, so that restriction is not counted as a defect. Warning suppression is per layer within each bucket, as reflected in the new unit test.

## Validation and limits

Reviewed the PR's text diff, shader sources and attribute metadata, image update and atlas lifecycles, layer grouping, new unit tests, render-style definitions, and the literal expected image. `git diff --check` passed for the reviewed base-to-head diff.

No builds, C++ unit tests, shader compilations, or render tests were run. The PR author's reported local test results were not independently reproduced. The modified binary `metrics/cache-style.db` was not audited internally, and the expected render images were not regenerated. F01 is established by tracing the implementation; its visual reproduction remains to be run.

Only this review document was added. Production code and existing local files were left unchanged.
