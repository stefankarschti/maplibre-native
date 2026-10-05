# Shared-core hot-path performance review

Originally reviewed on 2026-09-10 at `0ffe6336b4e7`; rechecked on 2026-10-05 at `8fe70930853e`. Scope: **interactive rendering and symbol placement** during pan, zoom, rotation, pitch, and animation-driven redraws. Backend code was inspected to establish the consequences of shared-core work. Only this document was changed.

The broadest candidate is preserving stable drawable metadata. Dense labels add two distinct opportunities: removing allocation from collision tests and reducing preparation performed even when collision placement is throttled. Start implementation with bounded changes whose removed work is easy to count, then use frame traces to choose among the larger reuse proposals.

This is a **source-based review, not a measured profile**. The repeated operations below are visible in current code; their contribution to frame time and the priority order remain hypotheses. No release baseline, device trace, or before/after benchmark was collected. The inspected `build-macos-metal/CMakeCache.txt` still specifies Debug and Tracy off. No builds or runtime tests were run for this document-only update. Code links refer to this checkout at the reviewed commit.

Standalone feature-query, MVT decoding/layout, `within` filtering, and GeoJSON cluster-construction findings from the original review have been removed from the active recommendations to keep this review focused. SDK bindings, networking, storage, debug visualization, optional rendered-feature capture, and exhaustive shader/GPU analysis are also outside scope.

**Priorities and triggering workloads**

“Every frame” means every rendered frame reaching the relevant path; an idle map need not render continuously. Effort: S = localized change; M = component change with regression coverage; L = cross-component ownership/invalidation work. Priority reflects likely reach and frequency, not measured speedup.

| Priority | Opportunity | When it can help | Removed work / effort |
| --- | --- | --- | --- |
| 1 | Preserve drawable attributes and segments | Stable fill/raster tiles and dense symbols, including camera motion | Metadata allocation and backend setup / M |
| 2 | Reuse collision duplicate tracking | Placement frames with dense labels, line labels, or repeated anchor attempts | Hash-node allocation and repeated lookup / M |
| 3 | Apply renderability once; group layers by source | Every render-tree update, especially with multiple populated sources | Repeated layer scans and group remove/add requests / S, then M |
| 4 | Batch symbol placement ordering | Every preparation with many `symbol-sort-key` ranges, even between placement runs | Quadratic list traversal / S–M |
| 5 | Gate dynamic symbol-buffer regeneration | Fixed-camera redraws; vertical-only placement can also benefit during motion | Reprojection, glyph-buffer writes, backend updates / M–L |
| 6 | Reuse uniform staging and common calculations | Many drawables, fill outlines, text/halo pairs, or segments | Temporary allocation and duplicate CPU calculations / S–M |
| 7 | Share equivalent tile-cover results | Multiple compatible sources; optionally repeated fixed-camera updates | Cover traversal and temporary allocation / M |
| 8 | Retain heatmap composite and image drawables | Rendered frames containing these overlays | Drawable and texture setup / S–M |

These opportunities overlap. Measure each increment against the previous version; do not add hypothetical benefits together. Promote 4 for styles with many sort keys, 2 for placement spikes, and 8 only when the relevant overlays are present. The numbered list is a review-priority order, not a descending estimate of milliseconds saved.

**Conditional frame-time estimates: mobile OpenGL/Vulkan**

The estimate target is **mobile OpenGL and Vulkan**. No specific handset, style, or device trace has been supplied, so absolute savings cannot be inferred from the source alone. The following are **low-confidence planning scenarios**, calculated from explicitly assumed current path costs and removable fractions. Those inputs are engineering assumptions, not measurements or claims about a typical phone. They describe optimized builds with warm tiles and the stated workload; actual savings can fall outside the ranges, including zero. Replace the assumed costs with exclusive CPU timings from the target trace before using these numbers to commit to a performance target.

The model is `CPU time saved = current cost of the affected work × net fraction eliminated`. The fractions below assume modest new bookkeeping overhead. They apply to the named work, not the entire frame or placement pass. Rows are sorted by the upper end of their scenario estimate; these are different workloads, so this is a ranking of conditional potential, not a comparison on one map.

| Finding | Workload and assumed current cost of the affected work | Assumed net fraction eliminated | Estimated CPU saving per affected frame |
| --- | --- | --- | --- |
| 4: batch placement ordering | Many sort-key ranges; list-order construction costs 1–5 ms | 80–95% | About **0.8–4.8 ms**, including frames reusing collision placement |
| 5: gate dynamic symbol buffers | Dense labels with reusable output; regeneration costs 0.5–3 ms | 80–95% | About **0.4–2.9 ms** on eligible redraws |
| 2: collision duplicate scratch | Dense placement; duplicate tracking alone costs 0.5–3 ms | 50–80% | About **0.25–2.4 ms** on placement frames; **0 ms** between placement runs |
| 1: retain drawable metadata | Many stable fill/raster/symbol drawables; metadata and resulting backend setup cost 0.5–2 ms | 50–90% | About **0.25–1.8 ms** during camera motion or other redraws |
| 3: final renderability and source grouping | Many populated sources/layers; redundant scans and group churn cost 0.2–1 ms | 50–80% | About **0.1–0.8 ms**; assumes both changes, not just moving the status loop |
| 7: share tile covers | Five sources with identical effective cover arguments; their cover calculations total 0.25–1 ms | Approximately 80%, assuming negligible lookup/result-handling cost | About **0.2–0.8 ms**; lower with fewer compatible sources |
| 6: uniform staging and shared calculations | Many drawables; staging and repeated calculations cost 0.2–1 ms | 25–60% | About **0.05–0.6 ms**; capacity reuse alone targets only part of this |
| 8: retain overlay resources | Active heatmap/image overlays; repeated resource setup costs 0.1–0.5 ms | 50–80% | About **0.05–0.4 ms**; excludes heatmap density rendering |

The decimal endpoints are arithmetic results from the assumptions, not measurement precision. For example, finding 1 yields 1.2 ms if the targeted metadata/setup work takes 1.5 ms and retention eliminates 80%; if that work takes only 0.1 ms, the same fraction saves 0.08 ms. Source inspection establishes repetition, not which baseline applies.

Treat GL and Vulkan as separate measurements. For 1, GL additionally avoids fill/raster VAO reconstruction; Vulkan shares metadata costs but has no GL VAOs. For 5, the current Vulkan path replaces changed shared vertex buffers, whereas GL updates them; avoiding regeneration can remove different amounts of driver/resource work. For 6, consolidated staging-vector reuse applies to Vulkan, while shared calculations can benefit both. These differences justify backend-specific profiling, not an unsupported fixed GL/Vulkan speed ratio. No GPU-time reduction is included in the estimates.

For ordinary moving-camera maps, 1 remains the broadest candidate and 2 targets placement spikes. Finding 4 can overtake both with enough sort-key ranges. Finding 5 saves essentially nothing on the line/variable-anchor branches while their camera inputs change; its moving-camera opportunity is the vertical-only branch. Findings 7 and 8 need compatible sources and overlays respectively. A style missing the relevant work gets no gain.

Placement savings must also be weighted by frequency: saving 2 ms on five placement frames per second at 60 rendered frames per second averages about 0.17 ms per rendered frame, while still removing 2 ms from each affected frame. This is an illustration, not an assumed placement cadence or a prediction of p95/p99 improvement.

These are CPU savings, not guaranteed reductions in displayed frame intervals. A simplified overlapping pipeline has a frame period near `max(CPU critical-path time, GPU time)`; reducing CPU work helps throughput only while CPU remains the limiting stage. A frame already capped at 60 Hz may stay at 16.67 ms and gain deadline headroom instead. For scale, 1 ms is 6% of a 60 Hz frame budget and 12% of a 120 Hz budget. Measure presentation timing and GPU time alongside the CPU changes.

**1. Preserve drawable attributes and segments when their inputs are unchanged**

Evidence: [RenderFillLayer::update](../src/mln/renderer/layers/render_fill_layer.cpp#L322) allocates an attribute array per tile and calls `updateVertexAttributes` on existing fill, pattern, and basic-outline drawables. The triangulated-outline variant already has a modification-time guard. The [raster buildVertexData lambda](../src/mln/renderer/layers/render_raster_layer.cpp#L182) reconstructs attribute metadata and replaces existing drawable segments; unmasked tiles share `staticAttrs` only within that update call.

The downstream costs are concrete:

- [Drawable::setVertexAttributes](../include/mln/gfx/drawable.hpp#L194) resets the binding timestamp.
- [OpenGL updateVertexAttributes](../src/mln/gl/drawable_gl.cpp#L88) replaces segments with invalid VAOs; [upload](../src/mln/gl/drawable_gl.cpp#L185) rebuilds bindings and creates missing VAOs.
- [Metal updateVertexAttributes](../src/mln/mtl/drawable.cpp#L357) also replaces segments. [Metal attribute binding](../src/mln/mtl/upload_pass.cpp#L163) forces shared-buffer update attempts when the timestamp is absent.

Update attempts are not necessarily transfers. [Metal BufferResource::update](../src/mln/mtl/buffer_resource.cpp#L98) compares unchanged full-size buffers before replacement; [GL](../src/mln/gl/upload_pass.cpp#L95) and [Vulkan](../src/mln/vulkan/upload_pass.cpp#L65) retain unmodified shared vertex buffers. Metadata allocation, binding work, GL VAO churn, and Metal byte comparisons remain avoidable costs.

Implement fill/raster retention first: preserve metadata when bucket identity, geometry/index revisions, segment layout, and paint-binding layout are unchanged. Keep normal uploads for changed shared paint buffers. [Line](../src/mln/renderer/layers/render_line_layer.cpp#L439) and [circle](../src/mln/renderer/layers/render_circle_layer.cpp#L403) layers already retain existing drawable metadata. Avoid a gfx-interface redesign until a local fast path proves useful.

Treat symbols as a separate follow-up. [updateTileDrawable](../src/mln/renderer/layers/render_symbol_layer.cpp#L360) repopulates instance attributes with symbol instancing enabled, and vertex attributes otherwise. [VertexAttributeArray::set](../src/mln/gfx/vertex_attribute.cpp#L101) replaces each attribute object. This is not the same segment-replacement path as fill/raster. Preserve stable attribute descriptors while allowing dynamic positions, opacity, and sorted-instance data to update. Current [backend defines](../include/mln/shaders/layer_ubo.hpp#L73) enable symbol instancing on Metal/Vulkan, so validate both forms.

Validation: fixed pan/zoom over fills and rasters; count attribute/segment allocations, binding rebuilds, GL VAO creation, and actual buffer updates. Stable tiles should approach zero metadata reconstruction after warmup. Exercise feature-state and global-state paint changes, constant/data-driven transitions, patterns/outlines, raster masking, bucket replacement, and context recreation. Retention must preserve the existing distinction between old-style and current-tweaker drawables.

**2. Remove hash-node allocation from collision duplicate tracking**

Evidence: both [box queries](../src/mln/util/grid_index.hpp#L235) and [circle queries](../src/mln/util/grid_index.hpp#L300) create two local `std::unordered_set`s. Their cell-walking paths use `contains` followed by `insert` for each new primitive. [CollisionIndex::placeFeature](../src/mln/text/collision_index.cpp#L149) reaches these queries through box and projected-circle `hitTest` calls when overlap is disallowed.

This cost belongs to placement runs, not every frame: [continuous placement is throttled](../src/mln/renderer/render_orchestrator.cpp#L560). Populated misses and predicates rejecting many candidates are useful stress cases. Empty sets need not allocate, and early collision exits already limit work.

Use caller-owned generation-stamp arrays indexed by the existing dense primitive IDs, separately for boxes and circles. Advance the generation per query; grow storage as the grid grows. Account for wraparound, grid lifetime/reset, and independent scratch for concurrent or nested queries. Preserve traversal and predicate invocation order.

A smaller change is `insert(uid).second`, removing the double lookup. Merely retaining a node-based set's bucket array does not remove node allocation: `clear()` frees the nodes. Choose scratch storage based on visited-candidate counts and memory measurements.

Validation: sparse/dense point labels, line labels, variable anchors, and cross-source collision groups. Record allocations per placement and p95/p99 placement duration separately from frames reusing placement. Extend [grid-index coverage](../test/util/grid_index.test.cpp) for duplicate cells, box/circle combinations, predicates, early exits, generation wraparound, and scratch isolation. Collision outcomes must remain identical.

**3. Apply final renderability once; avoid scanning every layer for every source**

Evidence: [createRenderTree](../src/mln/renderer/render_orchestrator.cpp#L402) initializes `updateList` to false, scans all layers for each source, then scans all layers again inside the source loop to apply renderability. Both scans cost O(S × L) for S sources and L layers.

An already-visible layer belonging to source B remains false while earlier source A is processed. It is marked non-renderable, then renderable when B is reached. [markLayerRenderable / activateLayerGroup](../src/mln/renderer/render_layer.cpp#L209) allocate remove/add requests, and [group operations](../src/mln/renderer/render_orchestrator.cpp#L967) erase/reinsert the existing group. Final visibility can be correct despite this intermediate churn; this does not imply drawable recreation or visible flicker.

First, apply renderability once after all sources have contributed. Flush accumulated changes outside the source loop, including requests from earlier style-diff processing and updates with no sources. The separate source-less background/custom-layer logic needs explicit regression coverage; moving the status loop alone is not a redesign of that logic.

Then, if association remains material, build source-to-ordered-layer membership in one pass. A per-update grouping avoids persistent-cache invalidation; retain it across updates only if measurement justifies it. Compute visibility, zoom eligibility, and 3D flags once per layer, preserving global layer order and source-specific relayout rules, including global-state dependencies. Association visits can become O(S + L), excluding tile work and ordered render-item insertion.

The [multiple-sources benchmark](../benchmark/api/render.benchmark.cpp#L126) adds 50 sources with 50 layers each: at least 125,000 source/layer combinations per scan, or 250,000 visits across both scans. These are operation counts, not timings. The empty added sources do not establish active group churn.

Validation: 1/5/50 populated sources; measure `createRenderTree` time and change requests. A steady update should enqueue no visibility remove/add pairs for unchanged layers. Cover layer/source removal, reorder, visibility, zoom bounds, global-state relayout, source-less layers, and heatmap render-target activation.

**4. Build symbol placement order in a batch**

Evidence: [RenderSymbolLayer::prepare](../src/mln/renderer/layers/render_symbol_layer.cpp#L200) clears placement data, then uses `std::upper_bound` for every sort-key range. [LayerPlacementData is a std::list](../src/mln/renderer/render_layer.hpp#L76): binary search still traverses a linear number of iterators. Building R entries can require O(R²) iterator steps and R node allocations. This preparation occurs before the placement throttle, so frames reusing collision placement still pay it.

Append keyed entries in traversal order, then stable-sort the list once by sort key. This reduces ordering work to O(R log R) and preserves the equal-key order produced by `upper_bound`. Keep the unkeyed append path. **List sorting does not remove node allocation**; a reserved vector is a separate follow-up requiring a review of reference/iterator assumptions.

Validation: vary tiles and ranges with interleaved/equal keys; compare exact traversal order, bucket-group leadership, and resulting collisions. Check the keyed/unkeyed assumptions across retained and replacement buckets. Measure preparation independently of collision placement, including frames on which placement is skipped.

**5. Gate dynamic symbol-buffer regeneration by subpath**

Evidence: [RenderTreeImpl::prepare](../src/mln/renderer/render_orchestrator.cpp#L105) updates placement buckets every rendered frame. [SymbolBucket::updateVertices](../src/mln/renderer/buckets/symbol_bucket.cpp#L346) gates opacity updates but always calls `updateBucketDynamicAttributeData`. The function is not expensive for every symbol: ordinary fixed-anchor point labels without vertical placement fall through without rebuilding data.

The relevant branches have different dependencies:

| Branch | Current repeated work | Candidate reuse boundary |
| --- | --- | --- |
| [Map-aligned line labels](../src/mln/text/placement.cpp#L788) | Reproject text/icons and regenerate dynamic attributes | Unchanged transform, tile matrix/wrap, placement state, size inputs, and bucket data |
| [Variable-anchor point labels](../src/mln/text/placement.cpp#L832) | Rebuild text and fitted-icon attributes; construct `placedTextShifts` | Unchanged projection/size inputs, variable offsets, visibility/orientation, and bucket data |
| [Vertical-only point placement](../src/mln/text/placement.cpp#L935) | Re-emit anchor/angle or hidden glyphs | This branch does not read camera state; invalidate on its symbol visibility/orientation and bucket/layout inputs |

The vertical-only branch is a narrower first implementation candidate and can benefit during camera motion between placement changes. For line/variable-anchor paths, begin with stationary-camera redraws caused by unrelated animation. Do not skip line reprojection during camera motion.

Cache the inputs or explicit revisions that determine output, rather than using camera equality alone. Placement at an unchanged camera can change visibility and variable anchors. Preserve `hasVariablePlacement`, fitted-icon output, and opacity/fade updates independently. [VertexVectorBase::updateModified](../src/mln/gfx/vertex_vector.hpp#L37) already advances timestamps only for dirty vectors; the target is the preceding unnecessary clear/append work.

Backend benefit differs: [GL](../src/mln/gl/upload_pass.cpp#L105) uploads dirty shared vertices, while the current [Vulkan path](../src/mln/vulkan/upload_pass.cpp#L76) replaces changed shared vertex buffers. Metal may suppress an identical full-buffer replacement after a byte comparison. Avoiding generation can therefore save CPU even where actual transfer bytes are unchanged.

Validation: line labels, variable anchors, vertical writing, and icon-text-fit with moving and stationary cameras. Count glyphs regenerated, CPU writes, temporary allocations, and actual backend bytes/allocations. Exercise placement changes, bucket arrivals, hidden/oriented symbols, fading, zoom-dependent sizes, and world wraps. An idle map provides no redraws to optimize.

**6. Reuse uniform staging and compute shared values fewer times**

Evidence: [SymbolLayerTweaker::execute](../src/mln/renderer/layers/symbol_layer_tweaker.cpp#L92) constructs two drawable-count-sized vectors in consolidated-UBO builds, then repeats matrices, size evaluations, and interpolation factors per drawable. Text/halo pairs and multiple segments can share many inputs. [FillLayerTweaker](../src/mln/renderer/layers/fill_layer_tweaker.cpp#L60) has the same staging pattern and computes pattern values before selecting the fill variant.

Separate three small hypotheses: retain staging-vector capacity; move pattern-only work into pattern variants; reuse common calculations within the current frame and matching tile/bucket/variant. Start with these before introducing cross-frame uniform caching. Reusing capacity removes allocation, not the cost of populating or uploading uniforms. Rewrite all consumed fields and keep padding/unused union bytes deterministic; size uploads and UBO indices to the current drawable set, not retained capacity.

[LayerTweaker::getTileMatrix](../src/mln/renderer/layer_tweaker.cpp#L46) rebuilds the tile transform and projection product, but a universal tile-ID-only cache is invalid. Translation/anchor, origin, alignment, projection choice, 3D/depth mode, and [layer/sublayer depth offsets](../src/mln/renderer/layer_tweaker.cpp#L91) matter. Prefer caching a common intermediate or narrowly matching drawables.

[UBO consolidation](../include/mln/shaders/layer_ubo.hpp#L73) already exists for Metal/Vulkan/WebGPU, and paint UBOs have `propertiesUpdated` guards. [GL](../src/mln/gl/uniform_buffer_gl.cpp#L129), [Metal](../src/mln/mtl/buffer_resource.cpp#L98), and [Vulkan](../src/mln/vulkan/buffer_resource.cpp#L198) also compare content in their buffer-update paths. Do not count existing batching or equality checks as new savings.

Validation: staging allocations and matrix/size evaluations per frame, plus CPU time and actual uniform writes. Cover solid/pattern fills, outlines, text/icon/halo combinations, depth ordering, drawable-count changes, and style transitions. Measure retained memory after an unusually large frame.

**7. Share tile-cover calculations while continuing tile lifecycle work**

Evidence: after its non-rendering-source early return, [TilePyramid::update](../src/mln/renderer/tile_pyramid.cpp#L94) derives ideal/prefetch arguments and calls `util::tileCover` for each eligible rendering source. Compatible sources can request identical covers within one update; stationary-camera redraws can repeat them across updates.

Start with reuse within one render update for identical effective arguments. Key by the actual cover inputs: transform/viewport, ideal or prefetch zoom, zoom range, overscaled zoom, and LOD parameters. Source type, tile size, and zoom shift affect the derived arguments; matching just camera and nominal zoom is insufficient. Keep the cached result immutable, including when Tile mode truncates a consumer's cover. Consider cross-frame retention only after measuring this smaller change.

Reuse only the geometric cover. Continue tile-arrival reconciliation, parent/child retention, expiry/necessity updates, relayout, cache processing, and fades. Equal camera parameters do not imply an unchanged tile lifecycle.

Validation: compatible/incompatible sources, high pitch, resize, wrap jumps, overzoom, prefetch/LOD changes, and fixed-camera tile arrivals. Count cover calls per unique argument set and time cover generation separately from reconciliation. Use [tile-cover benchmarks](../benchmark/util/tilecover.benchmark.cpp) for kernel cost and a continuous-mode trace for frame impact.

**8. Retain heatmap composite resources and image-source drawables**

Evidence: [RenderHeatmapLayer::update](../src/mln/renderer/layers/render_heatmap_layer.cpp#L358) clears composite drawables each update, rebuilds the fullscreen-quad drawable, and creates a color-ramp texture. Its [256 × 1 ramp](../src/mln/renderer/layers/render_heatmap_layer.cpp#L39) is small; the main hypothesis is object/resource overhead.

Retain the composite drawable and ramp texture. Track ramp content changes explicitly: [updateColorRamp](../src/mln/renderer/layers/render_heatmap_layer.cpp#L99) mutates the existing image, and [global-state changes](../src/mln/renderer/layers/render_heatmap_layer.cpp#L61) can trigger it. Image-pointer equality alone cannot detect a changed ramp. Rebind on render-target changes and preserve resize/style lifecycles. Density rendering remains necessary when its inputs change; its target is already half viewport width/height.

Separately, the [image-data branch of RenderRasterLayer](../src/mln/renderer/layers/render_raster_layer.cpp#L237) clears and rebuilds drawables for all image matrices every update. Retain them while updating transformations; recreate for geometry/bucket/wrap-instance changes. [setTextures](../src/mln/renderer/layers/render_raster_layer.cpp#L159) already shares `bucket.texture2d`, so this finding does not imply a fresh source-image upload for every matrix.

Implement and measure these independently. Validation: camera animation, ramp/global-state/intensity changes, image content/coordinates/resampling, viewport resize, wrapping, and source removal. After warmup, stable composite/image geometry should create no new drawables, and an unchanged ramp should create no new textures. Compare pixels and resource counts; these changes do not address fragment-bound heatmaps.

**Implementation sequence and acceptance evidence**

| Workload | First concrete change | Evidence to require |
| --- | --- | --- |
| Multiple populated sources | 3: one final renderability pass | Remove/add pairs disappear; render-tree CPU improves |
| Many symbol sort-key ranges | 4: append then stable list sort | Identical ordering; preparation scales near R log R |
| Fill/raster-heavy camera motion | 1: retain metadata on existing drawables | Attribute/segment/VAO churn drops without stale paint or masks |
| Dense-label placement spikes | 2: caller-owned duplicate scratch | Fewer allocations and lower placement tail latency |
| Repeated symbol-buffer work | 5: vertical-only branch, then unchanged-camera paths | Fewer glyph writes; placement/fades remain correct |
| Remaining per-frame CPU | 6, then 7 if traces identify them | Lower staging/matrix/cover CPU with bounded retained memory |
| Heatmap/image overlays | 8, as independent patches | Stable drawable/texture counts after warmup |

**Establishing a reproducible baseline**

Use two complementary baselines: an optimized host run for quick core regressions, and an on-device run for actual mobile frame pacing. Compare before/after on the same device and backend; macOS Metal, iOS Metal, Android OpenGL, and Android Vulkan are separate result series. Simulator/emulator results are useful for harness validation, not mobile performance claims.

Inventory checked on 2026-10-05: this is an arm64 macOS host with Xcode 27.0, CMake, Ninja, Bazel/Bazelisk, and ADB available. `xcrun xctrace list devices` reported the host and simulators, with no physical iOS device; `adb devices -l` reported no Android devices. Recheck after connecting/unlocking a device and enabling its development access. The existing `build-macos-metal` is Debug. Android's [Versions.kt](../platform/android/buildSrc/src/main/kotlin/Versions.kt) requests NDK `28.2.13676358` and CMake `3.24.0+`; neither was present in the inspected Android SDK installation, so resolve those SDK prerequisites before building. No performance run or build was executed during this inventory; the recipes below are source-checked instructions.

| Existing infrastructure | Reuse it for | Limits to preserve in the report |
| --- | --- | --- |
| [Host C++ benchmark runner](../platform/macos/macos.cmake#L102), [Google Benchmark options](../vendor/benchmark/docs/user_guide.md) | Offline render regression tests and tile-cover kernels; JSON and repeated runs already supported | Render API tests use `MapMode::Static` and headless image readback; repetition statistics are not per-frame percentiles |
| [GLFW benchmark mode](../platform/glfw/glfw_view.cpp#L1158) | Continuous fixed-camera redraws with a chosen style; host CPU profiling | Re-invalidates continuously and reduces pacing limits; printed “fps” is derived from CPU render-call duration, not presentation rate; no general camera-replay CLI |
| [Android BenchmarkActivity](../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/benchmark/BenchmarkActivity.kt#L172) and [instrumentation test](../platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/benchmark/Benchmark.kt) | Camera tour on a real `MapView` using TextureView; separate synchronous/asynchronous runs; JSON results | Defaults use remote styles; exports means and slowest-1% means, not raw frames or p50/p95/p99 |
| [iOS BenchmarkApp](../platform/ios/BUILD.bazel#L232) and [controller](../platform/ios/benchmark/MBXBenchViewController.mm#L103) | Continuous-mode core rendering on a physical Metal device, with per-location averages | Headless frontend with synchronous flushing and per-frame image readback/display; not normal `MLNMapView` presentation |

**Common measurement contract**

Before taking baseline A, freeze the harness and inputs that will also run against candidate B. Use optimized builds with matching symbols, compiler/NDK, feature flags, and validation settings. Keep debuggers, collision visualization, rendered-feature capture, and heavyweight allocation recording out of the headline timing run; collect diagnostic traces separately with the same instrumentation on A and B.

Save each run under an artifact directory such as `perf-results/<revision>/<device>/<backend>/<scenario>/<run>/`. Include the full commit SHA and dirty patch/build identifier, build command/configuration, executable/APK and matching symbols, OS/GPU/driver, viewport/pixel ratio/refresh rate, style and asset hashes, camera schedule, cache state, swap mode, power/thermal state, profiler configuration, raw output, and a screenshot confirming the scene. A Git SHA alone does not identify an uncommitted fix.

Use bundled assets or a frozen local tile/style/glyph/sprite server. Warm the complete route before the warm-cache measurement; a style-loaded callback alone does not establish that all route tiles are ready. Keep cold tile-arrival runs separate. Reject runs with missing assets, failed styles, altered viewport, or thermal throttling instead of averaging them into the result.

For the targeted traces, start with one warmup and at least five comparable 30–60-second measured windows per build, alternating A/B order across runs and allowing the device to cool. Existing longer benchmarks can retain their own schedule. Fix camera changes to a reproducible time schedule; for a kernel workload, use a fixed operation count. Avoid an unconstrained “advance camera once per rendered frame” loop, which changes the workload when a fix increases FPS.

Record per-frame renderer CPU elapsed time, placement duration/count, presented-frame deadline misses, allocations/resource counts, and peak/steady memory. Keep placement-running and placement-reusing frames separate. **RenderingStats.renderingTime is not GPU execution time:** [the implementation](../src/mln/renderer/renderer_impl.cpp#L460) measures CPU wall time around `encoder->present`; `encodingTime` is the remaining elapsed render-tree time. Both are seconds in core and are converted to milliseconds by the Android benchmark. They can include waits and are not CPU utilization counters. For a combined renderer elapsed-time distribution, sum the two values for each frame before calculating percentiles; do not add their separate p95/p99 values. Measure GPU execution/presentation independently with platform tools.

**Host: first baseline without a new harness**

Run from the repository root. Reuse the [macOS preset](../CMakePresets.json), but override its Debug default in a separate build directory:

```sh
cmake --preset macos-metal -B build-perf-macos-metal \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DMLN_USE_TRACY=OFF -DMLN_WITH_CLANG_TIDY=OFF -DMLN_WITH_COVERAGE=OFF
cmake --build build-perf-macos-metal \
  --target mbgl-benchmark-runner mbgl-glfw -j 8
mkdir -p perf-results/baseline/host-metal
build-perf-macos-metal/mbgl-benchmark-runner --benchmark_list_tests=true
build-perf-macos-metal/mbgl-benchmark-runner \
  --benchmark_filter='^(TileCoverPitchedViewport|API_renderStill_reuse_map|API_renderStill_multiple_sources)(/|$)' \
  --benchmark_repetitions=7 --benchmark_min_warmup_time=2 \
  --benchmark_out=perf-results/baseline/host-metal/kernels.json \
  --benchmark_out_format=json
```

The [render fixtures](../benchmark/api/render.benchmark.cpp#L24) already use a local cache database and disable networking. Run with the repository as the working directory. The multiple-source fixture has empty added sources: it exercises layer scans, not populated drawable-group churn. The tile-cover fixture measures the kernel, not the cross-source saving in finding 7. Repeated benchmark measurements report variability of iteration averages; they do not establish navigation p99.

For continuous fixed-camera redraws, supply a frozen style whose complete resources are already local/cached, keep the window dimensions constant, and leave it focused:

```sh
build-perf-macos-metal/platform/glfw/mbgl-glfw \
  --style file:///absolute/path/to/frozen-style.json \
  --cache /absolute/path/to/frozen-cache.db --offline --benchmark \
  --lon=-73.992857 --lat=40.726989 --zoom=15 --bearing=0 --pitch=45 \
  2>&1 | tee perf-results/baseline/host-metal/redraw.log
```

The paths above are inputs to prepare, not fixtures created by this review. After warmup, capture a consistent interval; stop the app after the run. GLFW's [one-second reports](../platform/glfw/glfw_view.cpp#L1245) are useful smoke measurements, but retain a trace or add raw-frame output before claiming percentiles. Benchmark mode deliberately forces redraws; it is useful for findings 1/5/6/8 and must be labeled as such.

The installed Instruments templates include Time Profiler, Allocations, Game Performance, and Metal System Trace. Attach to the warmed process using its PID:

```sh
xcrun xctrace record --template 'Time Profiler' --time-limit 30s \
  --attach PID --output perf-results/baseline/host-metal/cpu.trace
```

Capture allocations and GPU behavior in separate runs. Use [Apple's Metal performance workflow](https://developer.apple.com/documentation/xcode/analyzing-the-performance-of-your-metal-app/) for CPU/GPU overlap and presentation. The repository's Tracy integration should not be assumed to work unchanged on Metal: its [instrumentation header](../include/mln/util/instrumentation.hpp#L28) currently requires an OpenGL/Vulkan backend define. Instruments is the host/iOS starting point here. The `macos-vulkan` preset is an optional MoltenVK comparison when installed; it does not reproduce an Android Vulkan driver.

Repeat the same commands against the fix with a different output directory. The vendored [compare.py](../vendor/benchmark/tools/compare.py) can compare Google Benchmark JSON after installing its adjacent Python requirements; keep individual repetitions, not only the aggregate rows.

**Android: reuse the existing camera-tour benchmark**

Connect a physical phone, enable USB debugging, approve the host, and select the serial from `adb devices -l`. Use the [Android build setup](mdbook/src/platforms/android/README.md) and required SDK/NDK versions. Override the benchmark styles through `MapLibreAndroidTestApp/src/main/res/values/developer-config.xml`, using the `benchmark_style_names` / `benchmark_style_urls` arrays shown in the [existing benchmark guide](mdbook/src/platforms/android/benchmark.md). Pin resources too; merely pinning a URL does not freeze its contents. For a host-local HTTP fixture server, `adb reverse tcp:PORT tcp:PORT` can expose it as device localhost, subject to the app's network configuration.

Build both real backend flavors using the [CI recipe](../.github/workflows/android-ci.yml#L141). From `platform/android`:

```sh
./gradlew :MapLibreAndroidTestApp:assembleOpenglRelease \
  :MapLibreAndroidTestApp:assembleOpenglReleaseAndroidTest \
  :MapLibreAndroidTestApp:assembleVulkanRelease \
  :MapLibreAndroidTestApp:assembleVulkanReleaseAndroidTest \
  -PtestBuildType=release -Pmaplibre.abis=arm64-v8a
```

Install and run one backend at a time; both use the same application ID. Substitute the actual serial. Commands below remain in `platform/android`:

```sh
PERF_ANDROID_SERIAL=DEVICE_SERIAL
mkdir -p ../../perf-results/baseline/android-opengl
adb -s "$PERF_ANDROID_SERIAL" install -r \
  MapLibreAndroidTestApp/build/outputs/apk/opengl/release/MapLibreAndroidTestApp-opengl-release.apk
adb -s "$PERF_ANDROID_SERIAL" install -r \
  MapLibreAndroidTestApp/build/outputs/apk/androidTest/opengl/release/MapLibreAndroidTestApp-opengl-release-androidTest.apk
adb -s "$PERF_ANDROID_SERIAL" shell am instrument -w -r \
  -e class org.maplibre.android.benchmark.Benchmark \
  org.maplibre.android.testapp.test/org.maplibre.android.InstrumentationRunner \
  > ../../perf-results/baseline/android-opengl/instrumentation.txt
adb -s "$PERF_ANDROID_SERIAL" exec-out run-as org.maplibre.android.testapp \
  cat files/benchmark_results.json \
  > ../../perf-results/baseline/android-opengl/benchmark_results.json
```

For Vulkan, use the `vulkan` APK paths and a separate `android-vulkan` output directory. Verify that results contain the intended `renderer`, revision, timestamp, positive FPS, and valid timings, and that instrumentation reports a successful test; an old JSON file can survive a failed run. `run-as` requires a debuggable installed app: the [source manifest](../platform/android/MapLibreAndroidTestApp/src/main/AndroidManifest.xml#L8) sets it, but verify the merged Release APK rather than inferring it from `BuildConfig.DEBUG`. If extraction is denied, preserve the already-printed JSON from logcat or add an instrumentation-owned result export/test attachment. Use the same optimized test packaging on both sides; switching only one side to Debug invalidates the comparison.

The [current schedule](../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/benchmark/BenchmarkActivity.kt#L177) is four locations, two swap modes, one discarded 15-second-per-leg warmup, and three recorded 70-second-per-leg tours. That is roughly **30 minutes per style per backend**, plus loading. Start with one representative style. Keep `syncRendering=true` and `false` results separate; they measure different synchronization behavior.

Interpret the existing output carefully. [FrameTimeStore.low1p](../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/utils/BenchmarkUtils.kt#L95) averages the slowest 1% of samples; it is not p99. The [listener](../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/benchmark/BenchmarkActivity.kt#L219) starts before style loading and only resets the FPS counter afterwards, so timing arrays include startup frames while FPS uses the tour interval. Keep those metrics as legacy summaries until the small measurement-window change described below is made consistently to A and B.

For frame deadlines and scheduling, capture an [Android Studio system trace](https://developer.android.com/studio/profile/cpu-profiler) or Perfetto during the same tour. [FrameTimeline](https://perfetto.dev/docs/data-sources/frametimeline) requires Android 12+ and documents SurfaceView limitations; this benchmark uses TextureView, but still inspect which app/surface frames are represented. For GPU diagnosis use [AGI on supported devices](https://developer.android.com/agi/start). Its OpenGL frame-capture mode uses ANGLE, so an ANGLE capture must not replace the native OpenGL timing baseline.

For core attribution, reuse [Tracy](mdbook/src/profiling/tracy-profiling.md): pass `-DMLN_USE_TRACY=ON` through the shared native CMake arguments in [NativeBuildPlugin.kt](../platform/android/buildSrc/src/main/kotlin/NativeBuildPlugin.kt), ensure `TRACY_ON_DEMAND` is defined consistently for the core and Tracy client, and forward `adb -s "$PERF_ANDROID_SERIAL" forward tcp:8086 tcp:8086`. No ready-made Gradle Tracy property was found. Preserve the client/server version: [vendor/tracy.cmake](../vendor/tracy.cmake#L10) currently fetches `master`. Existing zones cover render-tree creation, source/layer preparation, and placement; add narrow zones only where these cannot isolate the proposed change. The current GPU-zone integration is OpenGL-only; Vulkan CPU zones do not supply Vulkan GPU timing.

**iOS: reuse BenchmarkApp, then validate normal presentation**

Use a physical iPhone/iPad for performance and a simulator only to verify setup. Follow the [iOS signing/Bazel guide](mdbook/src/platforms/ios/README.md), including a local bundle prefix and development team. From the repository root:

```sh
bazel run //platform/ios:xcodeproj \
  --@rules_xcodeproj//xcodeproj:extra_common_flags="--//:renderer=metal"
xed platform/ios/MapLibre.xcodeproj
```

Select the generated `BenchmarkApp` scheme and physical device, explicitly use **Release** for Run/Profile, and preserve symbols. The [project defaults to Debug](../platform/ios/BUILD.bazel#L267). Record the selected scheme/configuration and resolved build settings. For a scripted build after signing is configured:

```sh
PERF_IOS_UDID=DEVICE_UDID
xcodebuild -project platform/ios/MapLibre.xcodeproj -scheme BenchmarkApp \
  -configuration Release -destination "id=$PERF_IOS_UDID" \
  -derivedDataPath build-perf-ios build
```

Run from Xcode without the debugger attached, or install the resulting `.app` through `xcrun devicectl device install app --device "$PERF_IOS_UDID" /path/to/BenchmarkApp.app` and launch its configured bundle ID. Save the app's console output. Optional `LOG_TO_DOCUMENTS_DIR` support already exists in the controller, but is not enabled by the current Bazel target; a structured JSON/CSV export would be a small follow-up. The old `bench_UITests.swift` and `benchmark/ios` wrapper sources are not current top-level benchmark test targets in the inspected Bazel project, so do not assume `xcodebuild test` automatically runs them.

The controller selects a [bundled local style](../platform/ios/benchmark/MBXBenchViewController.mm#L77) only when its tile sentinel exists; otherwise it falls back to MapTiler. Verify the logged style and freeze the bundled assets. It waits for `isFullyLoaded`, then measures five seconds per location, emitting average encoding/present timings. However, `easeTo` starts **before** that loading wait, so variable loading can consume different parts of the animation. Its [readback/image display](../platform/ios/benchmark/MBXBenchViewController.mm#L197) also changes GPU synchronization and wall-clock pacing. Use these results as a labeled core stress baseline, then repeat the same scene/route in the existing iOS `App`/`MLNMapView` for user-visible performance.

Use Product → Profile with Time Profiler for CPU attribution and Game Performance/Metal System Trace for presentation and GPU work. A repeatable attachment to an already-running benchmark is:

```sh
mkdir -p perf-results/baseline/ios-metal
xcrun xctrace record --template 'Time Profiler' --device "$PERF_IOS_UDID" \
  --attach BenchmarkApp --time-limit 30s \
  --output perf-results/baseline/ios-metal/cpu.trace
```

Select the same measured phase in both traces; whole-app startup profiles are not steady-state frame measurements. The SDK sample already receives [frame stats](../platform/ios/app/MBXViewController.mm#L3050), providing an existing hook for raw samples. iOS validates shared-core and Metal effects; it cannot substitute for Android GL/Vulkan measurements.

**Small additions needed for decision-quality comparisons**

Extend the existing apps and benchmark runner before building a new framework. Put any harness changes in both A and B before recording the baseline:

- Add explicit warmup/measure phase markers, a configurable camera schedule, and raw-frame CSV/JSON output using existing frame callbacks. On Android, reset both timing stores after warmup; on iOS, start the measured camera transition after assets are ready. Buffer records in memory and write after the timed window. Include frame/phase IDs and placement-running status where available.
- Add short, frozen scenarios that the world tours do not cover: populated multi-source fills/rasters for 1/3/7; dense line/variable-anchor labels with sort keys and rotation/pitch for 2/4; stationary camera plus unrelated paint animation and vertical writing for 5; many outlines/halos for 6; heatmap/image overlays for 8. Reuse GLFW forced redraws for host screening, then test a real redraw trigger on phones.
- Add allocation-free counters or narrow trace zones for attribute/segment/VAO construction, collision duplicate tracking, placement-order construction, dynamic glyph writes, uniform staging/matrix evaluations, and cover calls. RenderingStats already supplies resource counters, but some count update attempts rather than physical transfers. Collect allocation call stacks in a separate diagnostic run.
- Add only missing kernels to the existing Google Benchmark target: collision queries and sort-key preparation currently have no direct benchmark. Vary candidate/range counts and verify identical results outside timed sections. A kernel speedup is supporting evidence; require the device trace to show that it reduces the targeted frame/placement cost.

Existing [Android device CI](../.github/workflows/android-device-test.yml#L45) already consumes benchmark APKs and stores JSON through the [collection](../scripts/aws-device-farm/collect-benchmark-outputs.mjs) and [database](../scripts/aws-device-farm/update-benchmark-db.mjs) scripts. Its benchmark job currently selects OpenGL; Vulkan render-test coverage is not a Vulkan performance baseline. Use that pipeline for broader device coverage after local A/B results, if access is available. The older [run-benchmark.sh](../platform/android/scripts/run-benchmark.sh) still references the removed `legacy` flavor, and the plot script expects older renderer names; use the current Gradle flavors/JSON and adapt reporting before reuse. No cloud runs are required to establish the first baseline.

**Comparing A and B**

For each identical device/backend/style/phase/swap-mode group, compute per-run p50/p95/p99 from raw frames, then compare the distribution of run summaries. Keep the existing Android slowest-1% mean separately named. Report `saved_ms = baseline_ms − candidate_ms` and `saved_percent = 100 × saved_ms / baseline_ms`, alongside frame count, deadline-miss rate, placement frequency, and memory. Never derive p99 from average FPS or average per-run p99 values into a claimed pooled percentile.

First repeat A against A to establish noise. Accept a fix only when the A/B effect exceeds that variation across repeated runs, the intended operation counts decrease, and matching render tests/pixels and ordering remain correct. Report unchanged FPS as such if the gain is CPU headroom under a frame cap. Preserve raw artifacts and failures; do not sum the scenario estimates above or extrapolate host milliseconds to phones. Replace those assumptions with measured before/after results as fixes land.

Existing style-dependency guards, layout grouping, placement throttling, per-source Y-sorted tile reuse, line/circle drawable retention, dirty vertex timestamps, UBO consolidation, buffer comparisons, and shared raster textures constrain the opportunity. The proposed work preserves those mechanisms and targets redundant CPU work around them.
