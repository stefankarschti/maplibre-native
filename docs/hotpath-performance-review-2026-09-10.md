# Shared-core hot-path performance review

Reviewed on 2026-09-10 at commit `0ffe6336b4e7`. Scope: shared rendering, symbol placement, tile decoding/layout, feature queries, and GeoJSON processing. Backend implementations were inspected where necessary to determine the consequences of shared-core work. No source code was changed.

The strongest broad opportunity is to preserve existing drawable metadata when geometry and bindings have not changed. Next are allocation-heavy collision queries and unnecessary source/layer orchestration. Several narrower workloads have larger asymptotic opportunities: symbol sort keys, large feature queries, and repeated MVT layer parsing.

This is a **source-based review, not a measured profile**. Rankings reflect expected user-visible impact, frequency, and reach across platforms; they are not measured speedups. “High confidence” means the redundant work or complexity is directly visible in the inspected code. It does not establish its fraction of application frame time. The local `build-macos-metal` configuration is Debug with Tracy disabled; no release performance baseline, device trace, or before/after benchmark was collected. No tests or builds were run for this document-only review.

Code links point into this checkout and include the relevant starting line. Existing unrelated working-tree changes were left untouched. Platform SDK/JNI/Objective-C overhead, network behavior, database tuning, third-party internals beyond the MVT decoder, and exhaustive shader/GPU analysis are outside this review's conclusions.

**Ranked opportunities**

“Every frame” below means every rendered frame reaching the relevant path. It does not imply that an idle map continuously renders. Effort: S = localized change; M = component-level change and regression coverage; L = ownership or invalidation changes across components.

| Rank | Opportunity | Expected impact and trigger | Primary cost | Effort / confidence |
| --- | --- | --- | --- | --- |
| 1 | Preserve drawable attributes and segments | High: fill/raster-heavy maps and dense symbols, including camera motion | Frame CPU, allocation, backend setup | M / High |
| 2 | Reuse collision-query duplicate tracking | High: dense labels, especially line labels and repeated anchor attempts | Placement CPU and allocation | M / High |
| 3 | Process layer renderability once; index layers by source | High with multiple sources or many layers; smaller for one-source styles | Frame CPU and change-request churn | S–M / High |
| 4 | Replace incremental ordered-list construction for symbol placement | High with many `symbol-sort-key` ranges | Frame CPU; quadratic iterator traversal | S–M / High |
| 5 | Reuse decoded MVT layer metadata and selected feature data | High during complex tile loading and large rendered-feature queries | Worker/query CPU and allocation | M–L / High |
| 6 | Avoid rebuilding unchanged dynamic symbol vertices | High for dense labels during stationary-camera redraws; less benefit during camera motion | Frame CPU and buffer updates | M–L / High |
| 7 | Precompute symbol query ranks and remove result copies | High query-latency benefit for large selections; no benefit without queries | Query CPU and memory traffic | S–M / High |
| 8 | Reuse uniform staging and common per-tile calculations | Medium: many drawables, outlines, halos, and segments | Frame CPU and memory traffic | M / High |
| 9 | Reuse tile-cover results across compatible updates/sources | Medium; higher with many sources and high-pitch covers | Frame CPU | M / High |
| 10 | Retain heatmap composite and image-source drawables | Medium when those layers are present; localized opportunity | Allocation and graphics-resource setup | S–M / High |
| 11 | Prepare the constant polygon in `within` expressions | High with complex spatial filters; low general prevalence | Tile layout/filter CPU | M / High |
| 12 | Hoist repeated GeoJSON cluster property-map copies | Medium for large clustered datasets with several aggregate properties | Source construction CPU and allocation | S / High |

These improvements overlap. For example, reducing symbol work can shrink the measured benefit of later uniform optimizations. Do not add their estimated benefits together. For an application dominated by selection queries, prioritize 5 and 7; for complex spatial filters, promote 11; for ordinary animated vector maps, begin with 1–3.

**1. Preserve drawable attributes and segments when their inputs are unchanged**

Evidence: [RenderFillLayer::update](/Users/stefan/maplibre-native-stefan/src/mln/renderer/layers/render_fill_layer.cpp:232) creates a new attribute array for each tile, reads paint bindings, and calls `updateVertexAttributes` for existing fill/outline drawables. [RenderRasterLayer::update](/Users/stefan/maplibre-native-stefan/src/mln/renderer/layers/render_raster_layer.cpp:182) similarly rebuilds metadata through `buildVertexData`, including the existing-drawable path at line 320. [updateTileDrawable](/Users/stefan/maplibre-native-stefan/src/mln/renderer/layers/render_symbol_layer.cpp:352) repopulates attributes for existing symbol drawables; [VertexAttributeArray::set](/Users/stefan/maplibre-native-stefan/src/mln/gfx/vertex_attribute.cpp:100) creates a replacement attribute object each time.

For fills and rasters, the consequences extend beyond small wrapper allocations:

- [Drawable::setVertexAttributes](/Users/stefan/maplibre-native-stefan/include/mln/gfx/drawable.hpp:194) resets `attributeUpdateTime`, forcing binding reconstruction.
- [OpenGL updateVertexAttributes](/Users/stefan/maplibre-native-stefan/src/mln/gl/drawable_gl.cpp:88) allocates new segment objects with invalid VAOs. [Upload](/Users/stefan/maplibre-native-stefan/src/mln/gl/drawable_gl.cpp:180) rebuilds bindings and creates the missing VAOs.
- [Metal updateVertexAttributes](/Users/stefan/maplibre-native-stefan/src/mln/mtl/drawable.cpp:357) also recreates segments. [Metal attribute binding](/Users/stefan/maplibre-native-stefan/src/mln/mtl/upload_pass.cpp:163) passes `!lastUpdate` as `forceUpdate` to shared-buffer resolution, so resetting the timestamp causes redundant buffer-update attempts.

The cost scales with visible layer/tile combinations, attributes, and segments, even when camera movement only requires new matrices. **Do not count every update attempt as a GPU allocation or transfer:** [Metal BufferResource::update](/Users/stefan/maplibre-native-stefan/src/mln/mtl/buffer_resource.cpp:98) compares unchanged full-size buffers before replacement; OpenGL and Vulkan also retain shared buffers when their data has not changed. Metadata construction, binding work, VAO churn on GL, and some full-buffer comparisons remain real costs.

Recommended option: add a narrow fast path preserving attributes and segments when bucket identity, geometry/index revisions, attribute layout, and paint-binding layout are unchanged. Changed shared paint buffers should still upload normally. [Line layers](/Users/stefan/maplibre-native-stefan/src/mln/renderer/layers/render_line_layer.cpp:336) and [circle layers](/Users/stefan/maplibre-native-stefan/src/mln/renderer/layers/render_circle_layer.cpp:231) already retain existing drawables without rebuilding this metadata, providing local examples.

Alternative: make attribute updates incremental and split segment replacement from binding updates at the gfx interface. This helps more callers but changes a wider contract. Symbol attributes can be retained independently of truly dynamic position/opacity buffers.

Validation: replay a fixed pan/zoom over fill-heavy vector tiles and raster tiles; count attribute allocations, segment allocations, binding rebuilds, VAO creation, and actual buffer bytes. Test feature-state paint changes, constant-to-data-driven transitions, fill outlines/patterns, raster masking, bucket replacement, and context recreation. Success is near-zero geometry-metadata reconstruction for stable tiles while pixels and feature-state behavior remain identical.

**2. Remove per-query hash allocations from collision duplicate tracking**

Evidence: [GridIndex::query for boxes](/Users/stefan/maplibre-native-stefan/src/mln/util/grid_index.hpp:235) constructs two local `std::unordered_set`s. Each newly visited primitive performs `contains` followed by `insert`. The [circle overload](/Users/stefan/maplibre-native-stefan/src/mln/util/grid_index.hpp:300) repeats the pattern. [CollisionIndex::placeFeature](/Users/stefan/maplibre-native-stefan/src/mln/text/collision_index.cpp:149) calls `hitTest` for label boxes; line placement calls it for projected circles at line 317. `hitTest` uses these same query implementations.

Dense placement therefore creates and destroys hash-table storage repeatedly, potentially once for every attempted box/circle. Empty queries need not allocate, and early collision exits already limit work; the opportunity concerns populated queries, especially misses or predicates rejecting many candidates.

Recommended option: use reusable generation-stamp arrays indexed by the existing primitive IDs, with separate box/circle tracking. A query advances its generation rather than allocating nodes or clearing all entries. Keep scratch state owned by the query caller or a placement-local context so concurrent read queries remain safe. Account for generation wraparound and growth as symbols enter the grid.

Lower-risk option: reuse scratch containers and use `insert(uid).second` for one lookup instead of two. Note that clearing a node-based set still frees its nodes; retaining only its bucket array is not a complete allocation fix. A caller-owned flat scratch representation or a small-vector path for tiny queries may be preferable after measurement.

Validation: profile placement with sparse/dense point labels, line labels, multiple variable anchors, and cross-source collision groups. Measure allocations per placement and p95/p99 placement duration. Compare exact results with [grid-index tests](/Users/stefan/maplibre-native-stefan/test/util/grid_index.test.cpp), including duplicate cells, boxes/circles, predicates, early exits, and independent concurrent query scratch. Do not remove deduplication or change collision order merely to make the benchmark faster.

**3. Resolve renderability once and avoid scanning every layer for every source**

Evidence: [createRenderTree](/Users/stefan/maplibre-native-stefan/src/mln/renderer/render_orchestrator.cpp:364) initializes `updateList` to false, loops over every source, scans every layer to find matches, then scans **all layers again inside the source loop** to apply renderability.

There are two distinct costs. With S sources and L layers, the two scans each perform O(S × L) visits. More subtly, an already-visible layer belonging to source B remains false in the new `updateList` while source A is processed. It is marked non-renderable, then marked renderable again when B is processed. [markLayerRenderable and activateLayerGroup](/Users/stefan/maplibre-native-stefan/src/mln/renderer/render_layer.cpp:203) allocate remove/add requests; [orchestrator group operations](/Users/stefan/maplibre-native-stefan/src/mln/renderer/render_orchestrator.cpp:908) erase and reinsert the existing group. The final visibility can be correct while intermediate work is unnecessary. This is not a claim that the entire drawable is recreated or that a visible flicker occurs.

Recommended first step: apply final renderability once after all sources have contributed. Include the no-source case and preserve pending requests created earlier during style diff processing. This removes one S × L scan and, in the steady-state multi-source example, removes the intermediate off/on request pair.

Second option: maintain source-to-ordered-layer membership, rebuilding it when style membership/order changes. Compute visibility, zoom eligibility, and 3D flags once per layer, then perform source-specific work only on its members. The association work becomes approximately O(S + L), excluding tile updates and ordered render-item insertion.

For illustration, the existing [multiple-sources benchmark](/Users/stefan/maplibre-native-stefan/benchmark/api/render.benchmark.cpp:126) adds 50 sources with 50 layers each. Those additions alone imply at least 125,000 source/layer combinations per full scan and 250,000 visits across both scans; the pre-existing base style increases the totals. These are operation counts, not timings. Its empty added sources do not establish the magnitude of active drawable-group churn, so also test populated sources.

Validation: 1, 5, and 50 populated sources; track change-request counts and `createRenderTree` CPU time. A steady frame should enqueue no visibility remove/add pairs when membership is unchanged. Exercise adding/removing/reordering layers, changing visibility and zoom bounds, background/custom layers without sources, and heatmap render-target activation.

**4. Build symbol placement order in a batch instead of searching a linked list for every range**

Evidence: [RenderSymbolLayer::prepare](/Users/stefan/maplibre-native-stefan/src/mln/renderer/layers/render_symbol_layer.cpp:196) clears placement data each frame. For every sort-key range it uses `std::upper_bound` and inserts at that position. Crucially, [LayerPlacementData is a std::list](/Users/stefan/maplibre-native-stefan/src/mln/renderer/render_layer.hpp:75), so binary search has linear iterator traversal. Across R ranges, rebuilding the ordered list can require O(R²) iterator steps, plus one node allocation per entry. The issue is not vector-element shifting.

This preparation precedes the [placement throttle](/Users/stefan/maplibre-native-stefan/src/mln/renderer/render_orchestrator.cpp:452), so its cost is paid even on frames that reuse recent collision placement. It is most relevant when `symbol-sort-key` has many distinct values across visible tiles; unkeyed symbols use the simpler append path.

Recommended option: for keyed placement, append all entries in traversal order, then use stable list sorting once, preserving the equal-key order currently produced by `upper_bound`. Retain the unkeyed append behavior. This changes ordering work to O(R log R) without first changing the container contract. A more substantial option is to collect into a reserved vector and stable-sort it, provided users do not depend on list iterator/reference behavior. A merge of already-sorted per-tile ranges is another option if measurements justify the extra machinery.

Validation: vary range count and tile count, with interleaved keys and many equal keys. Compare exact placement traversal order and rendered collisions, including mixed tiles and layer grouping. Measure `prepare` separately from actual placement. Avoid relying on the placement throttle to hide this cost.

**5. Reuse MVT layer parsing and selectively reuse feature decoding**

Evidence: [VectorMVTTileData::getLayer](/Users/stefan/maplibre-native-stefan/src/mln/tile/vector_mvt_tile_data.cpp:88) caches the top-level layer directory, but returns a new `VectorMVTTileLayer` for each call. Its constructor creates a fresh decoder layer; the [vendored layer constructor](/Users/stefan/maplibre-native-stefan/vendor/vector-tile/include/mapbox/vector_tile.hpp:366) traverses the layer protobuf and rebuilds feature views, keys, and values.

[GeometryTileWorker::parse](/Users/stefan/maplibre-native-stefan/src/mln/tile/geometry_tile_worker.cpp:445) requests a layer for each layout group. Different groups targeting the same source layer repeat that metadata parse. [getFeature and getGeometries](/Users/stefan/maplibre-native-stefan/src/mln/tile/vector_mvt_tile_data.cpp:45) also create new wrappers and cache decoded geometry only in each wrapper; accepted features appearing in multiple groups can be decoded repeatedly. Existing `groupLayers` already shares compatible layout work, so the remaining opportunity is across distinct groups.

There is a particularly strong query case: [FeatureIndex::addFeature](/Users/stefan/maplibre-native-stefan/src/mln/geometry/feature_index.cpp:258) creates a source-layer object separately for each candidate feature that reaches materialization. For K candidates in an MVT layer with F encoded feature entries, repeated layer-directory construction alone can approach O(K × F), before feature conversion and intersection work.

Recommended first step: cache parsed immutable layer metadata within the relevant tile/query lifetime. In queries, a source-layer cache local to one query is a bounded change. In tile layout, share read-only parsed layer storage with wrappers whose lifetime keeps both storage and encoded bytes alive.

Higher-memory option: retain lazy geometry/property decoding by feature index while processing multiple layout groups. Bound this by tile/layout lifetime or a measured cache budget; eagerly decoding every feature can waste time and memory on selective filters. `GeometryTileData::clone()` and worker/render-thread access require explicit ownership decisions. Do not introduce unsynchronized mutable caches into shared data.

Validation: compare 1/5/20 layout groups on the same large source layer, plus wide queries selecting many features. Count decoder-layer constructions and actual geometry decodes; report tile completion latency, query latency, and peak/resident memory. [Parse_VectorTile](/Users/stefan/maplibre-native-stefan/benchmark/parse/vector_tile.benchmark.cpp:8) visits each source layer once, so it will not alone reveal this repetition. Include [vector-tile tests](/Users/stefan/maplibre-native-stefan/test/tile/vector_tile.test.cpp), malformed data, polygon fixup, and tile eviction. This specific parsing finding applies to MVT; it is not a claim about the MLT implementation.

**6. Gate dynamic symbol-vertex regeneration on the inputs it actually uses**

Evidence: [RenderTreeImpl::prepare](/Users/stefan/maplibre-native-stefan/src/mln/renderer/render_orchestrator.cpp:93) updates placement buckets each frame. [SymbolBucket::updateVertices](/Users/stefan/maplibre-native-stefan/src/mln/renderer/buckets/symbol_bucket.cpp:346) gates opacity updates, but always calls `updateBucketDynamicAttributeData`. That [function](/Users/stefan/maplibre-native-stefan/src/mln/text/placement.cpp:776) reprojects line labels, or clears and rebuilds dynamic text/icon attributes for variable anchors and vertical placement. The variable-anchor path also constructs a temporary `placedTextShifts` map.

Clearing/appending [marks the vertex vector dirty](/Users/stefan/maplibre-native-stefan/src/mln/gfx/vertex_vector.hpp:69), so identical output can still lead to backend update work. Conversely, `updateModified()` only advances timestamps for dirty vectors: it does **not** indiscriminately dirty all static symbol data. The opportunity is to avoid the unnecessary regeneration itself.

Recommended option: record the transform/projection inputs, placement/visibility generation, bucket revision, and relevant size/layout state used for the dynamic buffer. Reuse output when these are unchanged. Begin with stationary-camera frames caused by unrelated paint animation or another source. Continue updating opacity/fade uniforms independently.

Alternative: for camera motion, move suitable point/variable-anchor transformations into shader inputs, or reuse scratch storage and reduce CPU output expansion. This changes more rendering behavior and needs separate backend design; it is a second-stage option. Line reprojection genuinely depends on the camera, so simply skipping it during motion is invalid.

Validation: dense line labels, variable-anchor POIs, vertical writing, and icon-text-fit, both with a moving camera and a fixed camera plus unrelated repaint. Measure dynamic bytes generated/uploaded, CPU time, and temporary allocations. Test placement changes at an unchanged camera, bucket arrival, fading, orientation changes, and tile wrapping. An entirely idle map has no frames to optimize here.

**7. Replace linear symbol-rank searches inside query sorting; eliminate extra result copies**

Evidence: [FeatureIndex::lookupSymbolFeatures](/Users/stefan/maplibre-native-stefan/src/mln/geometry/feature_index.cpp:196) sorts K hits. With `featureSortOrder` present, every comparison performs two linear `std::find`s in an N-entry order vector. This produces O(K log K × N) rank-search work. Large box queries can make K comparable to N.

Recommended option: lazily construct a feature-index-to-first-rank map for the current order snapshot, then use constant-time rank lookup during sorting. A query-local map changes work to O(N + K log K) without retained state. A cached map saves more on repeated queries but must be replaced when the order snapshot changes. Keep the **first occurrence** of duplicate feature IDs: the current `std::find` semantics matter when a feature has multiple symbol instances. For tiny hit sets, the original lookup can be cheaper than building a full map; use measurements to decide on a threshold.

Two independent low-risk copy reductions exist in the same output path. [queryRenderedSymbols](/Users/stefan/maplibre-native-stefan/src/mln/renderer/render_orchestrator.cpp:613) uses `for (auto layer : bucketSymbols)`, copying each map entry and its feature vector before moving from the copy. [FeatureIndex::addFeature](/Users/stefan/maplibre-native-stefan/src/mln/geometry/feature_index.cpp:298) inserts a completed local `Feature` as an lvalue. Iterate the former by reference and move the latter when implementing. Returning separate public features per style layer may still require independent values; those copies cannot all be removed blindly.

Validation: point queries and progressively larger boxes over overlapping, viewport-Y-sorted symbols; include duplicate source features and a bearing change between queries. Compare the exact output order and content. Use [API query benchmarks](/Users/stefan/maplibre-native-stefan/benchmark/api/query.benchmark.cpp:67) as a starting point, adding a workload that actually produces `featureSortOrder`. Measure query p50/p95 and allocation/bytes copied. Coordinate measurements with finding 5, which targets a different cost in the same queries.

**8. Reuse uniform staging and compute shared tile values fewer times**

Evidence: [SymbolLayerTweaker::execute](/Users/stefan/maplibre-native-stefan/src/mln/renderer/layers/symbol_layer_tweaker.cpp:92) creates two drawable-count-sized staging vectors in the consolidated-UBO configuration. Its drawable loop repeatedly computes matrices, size evaluations, and interpolation factors; text/halo or other drawables can share many of those inputs. [FillLayerTweaker](/Users/stefan/maplibre-native-stefan/src/mln/renderer/layers/fill_layer_tweaker.cpp:60) follows the same allocation pattern and computes pattern-specific values before selecting the actual fill variant. [getTileMatrix](/Users/stefan/maplibre-native-stefan/src/mln/renderer/layer_tweaker.cpp:28) recomputes the tile transform and projection product per invocation.

Recommended option: retain staging-vector capacity in each tweaker and cache common calculations for the current tile/variant within a frame. Skip pattern work for solid fills. Separate slowly changing tile properties from camera-dependent matrices where the existing shader contract permits it.

A later option is to upload only changed uniform blocks/ranges, but measure the actual backend benefit. [UBO consolidation](/Users/stefan/maplibre-native-stefan/include/mln/shaders/layer_ubo.hpp:73) already exists for Metal, Vulkan, and WebGPU. Paint uniform blocks also have `propertiesUpdated` guards. [OpenGL](/Users/stefan/maplibre-native-stefan/src/mln/gl/uniform_buffer_gl.cpp:129), [Metal](/Users/stefan/maplibre-native-stefan/src/mln/mtl/buffer_resource.cpp:98), and [Vulkan](/Users/stefan/maplibre-native-stefan/src/mln/vulkan/buffer_resource.cpp:198) have content-comparison paths that can suppress unchanged writes. The opportunity is not to introduce batching or equality checks that already exist.

Validation: count staging allocations and matrix/size computations per frame, alongside actual buffer updates. Cache keys must preserve translation/anchor, origin, projection mode, alignment, and the [layer/sublayer depth adjustment](/Users/stefan/maplibre-native-stefan/src/mln/renderer/layer_tweaker.cpp:52). Tile ID alone is insufficient for a universal final-matrix cache. Retained vector capacity also needs a memory-reduction policy after unusually large frames.

**9. Reuse tile-cover calculations without suppressing tile lifecycle work**

Evidence: [TilePyramid::update](/Users/stefan/maplibre-native-stefan/src/mln/renderer/tile_pyramid.cpp:94) derives ideal/prefetch covers and calls `util::tileCover` for each rendering source on each update. Equivalent sources can request the same cover, and a stationary camera can request it repeatedly while fading or other style work drives redraws.

Recommended option: memoize cover results within one render update for sources with identical effective cover arguments. This avoids cross-frame invalidation initially. A second option retains the previous cover for unchanged camera/viewport/projection, zoom range, effective zoom, source-type/tile-size effects, and LOD parameters. Prefetch cover parameters must be distinguished from ideal cover parameters.

Only reuse the geometric cover result. The remainder of `TilePyramid::update` must still process tile arrivals, retained parents/children, expiry/necessity changes, relayout, cache changes, and fades. Equal camera parameters do not mean the source has no work to do. Reuse becomes less valuable when every source has different cover parameters.

Validation: multiple compatible and deliberately incompatible sources, high pitch, resize, wrap jumps, overzoom, prefetch changes, and fixed-camera tile arrivals. Count cover calls per unique argument set and time cover generation separately from tile reconciliation. Existing [tile-cover benchmarks](/Users/stefan/maplibre-native-stefan/benchmark/util/tilecover.benchmark.cpp) provide a kernel baseline; also measure whole-frame behavior.

**10. Retain heatmap composite resources and image-source drawables**

Evidence: [RenderHeatmapLayer::update](/Users/stefan/maplibre-native-stefan/src/mln/renderer/layers/render_heatmap_layer.cpp:347) clears the composite layer's drawables every update, rebuilds its fullscreen quad, and creates a new color-ramp texture at line 381. The ramp itself is only 256 × 1 ([constructor](/Users/stefan/maplibre-native-stefan/src/mln/renderer/layers/render_heatmap_layer.cpp:43)), so this is principally object/resource overhead rather than a large texture-bandwidth saving.

Separately, the [image-data branch in RenderRasterLayer](/Users/stefan/maplibre-native-stefan/src/mln/renderer/layers/render_raster_layer.cpp:237) clears and rebuilds image-source drawables for all matrices each update. Despite a nearby TODO about texture sharing, [setTextures](/Users/stefan/maplibre-native-stefan/src/mln/renderer/layers/render_raster_layer.cpp:159) already uses `bucket.texture2d`; do not claim the source image is necessarily uploaded once per matrix per frame.

Recommended option: retain the heatmap composite drawable and color-ramp texture, updating ramp pixels only on color-ramp changes and rebinding/resizing when the render target changes. Retain image-source drawables while updating their transformation data; rebuild only for geometry/bucket/wrap-instance changes.

Alternative: implement the heatmap and image-source changes separately; they have independent lifecycle concerns and can be measured independently. Heatmap density rendering itself remains necessary when its inputs change, and the density target is already reduced to half viewport width/height. This proposal does not promise to solve fragment-bound heatmaps.

Validation: steady camera animation, heatmap color and intensity changes, image content/coordinate updates, viewport resize, world wrapping, and source removal. After warmup, unchanged ramps should create no new textures, and stable composite/image geometry should create no new drawables. Compare final pixels and actual resource counts.

**11. Prepare `within` polygon projections once per reusable evaluation context**

Evidence: [Within::evaluate](/Users/stefan/maplibre-native-stefan/src/mln/style/expression/within.cpp:221) calls `featureWithinPolygons` for each supported feature. [featureWithinPolygons](/Users/stefan/maplibre-native-stefan/src/mln/style/expression/within.cpp:147) reconstructs the literal polygon's tile-coordinate geometry and bounding box every time. [getTilePolygon](/Users/stefan/maplibre-native-stefan/src/mln/style/expression/within.cpp:41) allocates rings and projects every vertex using the math at line 23.

For F feature evaluations and P vertices in a constant filter polygon, preparation alone is O(F × P), in addition to the necessary containment tests. A bounded prepared representation can reduce repeated preparation to O(P) for the relevant coordinate context.

Recommended option: cache the projected polygon and bbox in a tile/evaluation-owned context. The current polygon projection depends on canonical zoom; begin with a conservative coordinate-context key and preserve exact integer rounding. Shared expressions can be evaluated concurrently, so an unsynchronized mutable cache on `Within` is inappropriate.

Alternative: precompute a normalized polygon representation and transform it cheaply per zoom, but require exact semantic comparisons before adopting it. Do not change holes, boundary rules, or antimeridian handling to achieve a speedup.

Validation: hundreds/thousands of polygon vertices and many point/line features, including mostly rejected features. Separate projection time from containment time and record temporary allocations. Exercise different zooms, wrapped coordinates, holes, boundary points, and expression tests. This deserves high priority only when spatial filters are common in the target styles.

**12. Copy cluster property maps once per callback, not once per aggregate**

Evidence: [GeoJSONData::create](/Users/stefan/maplibre-native-stefan/src/mln/style/sources/geojson_source_impl.cpp:108) installs cluster map/reduce callbacks. The map callback assigns `feature->properties = properties` inside the loop over `clusterProperties`; reduce similarly assigns `feature->properties = toFill` for each applicable aggregate.

For C aggregate properties and P input properties, the map callback can perform O(C × P) property-copy work per feature even though all aggregates use the same input. Reduce has analogous repetition. This is source/index construction work, not an ordinary camera-render hot path.

Recommended option: assign the shared input once before evaluating the relevant aggregates, retaining the existing no-work behavior when there are no aggregates or none apply. Expected map-copy work becomes O(P) per callback. A larger alternative is a non-owning evaluation feature adapter, which could remove the copy but introduces lifetime/API concerns and is unnecessary as a first step.

Validation: large clustered point collections with 0/1/5/20 aggregate expressions, wide property maps, missing reduce keys, and nested property values. Compare every cluster property and total index-build time/allocations. Preserve aggregate evaluation order and the current `accumulated` semantics. With one aggregate or no clustering, expected benefit is small or absent.

**Recommended implementation choices**

| Objective | Start with | Reason |
| --- | --- | --- |
| Best broad frame-time opportunity | 1, then 2 | Avoid repeated per-drawable setup and inner-loop allocation |
| Small, readily reviewable changes first | 3's final renderability pass, 4's batch list sort, 7's copies, 12 | Bounded transformations with directly checkable semantics |
| Dense labels and navigation | 2, 4, 6, then 8 | Addresses collision spikes, preparation, and per-frame glyph work separately |
| Faster tile arrival/style-heavy maps | 5, plus 11 when used | Removes duplicated decoding/preparation on worker paths |
| Large selection/hover workloads | 5's query-local layer cache and 7 | Removes independent superlinear costs in query materialization and ordering |
| Lowest architectural risk | Narrow fast paths and local scratch before persistent caches | Smaller invalidation and ownership surface |

**How to establish impact before implementing**

1. Establish a release or optimized-with-symbols baseline using deterministic local tiles, fixed style/camera traces, viewport/pixel ratio, device, backend, and thermal conditions. Separate warm-cache rendering from tile decode/layout and source construction. Keep correctness validation outside timed sections.
2. Use existing [Tracy instrumentation guidance](/Users/stefan/maplibre-native-stefan/docs/mdbook/src/profiling/tracy-profiling.md) and backend captures to distinguish CPU preparation, placement, allocation, driver encoding, and GPU work. A wait for a drawable is not proof that core CPU code is slow. Record p50/p95/p99 frame/placement/query duration, not just average FPS.
3. Use the existing [RenderingStats counters](/Users/stefan/maplibre-native-stefan/include/mln/gfx/rendering_stats.hpp:20) for buffers, textures, update bytes, and draw calls, supplementing them with allocation and actual backend-transfer measurements. Some counters count attempted updates before a backend equality check, so counter changes alone do not prove GPU bandwidth savings.
4. Cover five representative scenarios: populated multi-source vector pan/zoom; dense symbol rotation/pitch; fixed-camera redraws with unrelated animation; cold/warm tile layout with many style groups; and small/large rendered-feature queries. Add heatmap/image overlays, clustered GeoJSON, and spatial filters when those features matter.
5. Validate on at least one Metal device and an Android device for each supported GL/Vulkan configuration being targeted. Shared-core changes have different backend consequences; desktop-only results do not establish mobile impact. Keep WebGPU in regression coverage if it is a supported target.
6. Implement and measure one hypothesis at a time. Run relevant unit/expression/render tests and exact ordering checks. Accept an optimization only if its target metric improves beyond run-to-run variation without unacceptable memory growth or visual/semantic differences.

The [render API benchmarks](/Users/stefan/maplibre-native-stefan/benchmark/api/render.benchmark.cpp:63) use static map rendering, which takes a different placement path from continuous interaction. They are useful but insufficient for navigation frame pacing. The current [benchmark registration](/Users/stefan/maplibre-native-stefan/benchmark/CMakeLists.txt:1) also lacks direct coverage of several proposed kernels, including collision scratch reuse and large sort-key placement preparation. Any added benchmarks described above are proposed follow-up work; none were added or executed in this review.

**Existing optimizations that limit the opportunity**

The review explicitly accounted for style dependency guards, layout grouping, placement throttling, cached Y-sorted tiles within a source preparation, line/circle drawable reuse, dirty vertex timestamps, consolidated UBOs, backend byte-comparison guards, and shared raster textures. These are reasons to make targeted changes rather than assuming all visible loops or update calls imply redundant GPU work. No blanket container replacement, placement-quality reduction, shader rewrite, or threading expansion is recommended without a profile showing that it addresses the dominant cost.
