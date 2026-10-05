# Plugin API review: complete style inventory

**Review revision:** R2 · **Refreshed:** 2026-09-14 · **Previous inventory:** R1 at `af7905cfd2ae0cb6446c18cfbcd52887ae773c75`.

This is a read-only fixture inventory at `b60be9a7ef3bf8e27911c1d5cd9926edb66b8cd5`. “Capability gap” below means a plugin cannot reproduce that family through the new ABI as exposed; it does **not** mean existing builtin styles stop working. “Supported substrate” records implemented API machinery, not successful execution of these fixtures. No render, query, platform or expression tests were executed for this inventory. The tables below preserve the complete enumerated semantic families; the final ledgers list every identified style file, every supplemental source-code location, and all dependency revisions. See the [main review](plugin-api-review.md) for verified defects, priorities and the supported scope.

## R2 refresh from the R1 inventory

The tracked corpus adds one [builtin `text-offset/semiliteral` style](../../metrics/integration/render-tests/text-offset/semiliteral/style.json), 13 `semiliteral` expression integration fixtures, and two expression-equality inputs. All prior JSON/GeoJSON candidates remain present; the style-spec reference is modified. Totals increase from 10,434 to **10,450 candidates**, 1,572 to **1,573 styles**, and 466 to **481 raw expression fixtures**. The semantic-family/operator/property tables and every file ledger below were regenerated for R2, not copied forward as current counts.

Expression traversal now explicitly visits each child inside a `semiliteral` array. Styles observe 50 operators (previously 48: newly observed `semiliteral` and `length`), raw fixtures observe 84 (previously 83), and the combined classification table has 87 operator rows. Typed FLOAT2 plugin paint can consume `semiliteral`; this does not add plugin layout properties or arbitrary final array types. See the main review's R2 expression analysis for type/dependency limits.

The four new `PluginRendering` cases in [rendering.test.cpp](../../test/plugin/rendering.test.cpp#L171) construct styles programmatically and extend the source-locator count from 151 to 152. They cover F01–F03, which are now resolved in source. They are not four additional JSON style files. The 13 n-gon image fixtures, 175 render families and 30 query families are unchanged. No runtime test execution is claimed.

## Enumeration method and scope

Enumeration used two Python inventory/report passes with strict JSON parsing. The inventory reads tracked checkout JSON/GeoJSON and recursively follows initialized submodule test/fixture/benchmark JSON. It ignores untracked user work and the unrelated edited GLFW CMake file. All 63 visited submodules are initialized; their 1,021 test-data files contain no style document. Dependency revisions and per-module counts are recorded below. No files were fetched.

| Scope | JSON/GeoJSON enumerated | Style documents / parser fixture documents |
| --- | --- | --- |
| other tracked JSON | 8 | 0 |
| benchmark fixtures | 3 | 3 |
| metrics archived results/manifests | 7036 | 7 |
| integration shared assets | 44 | 6 |
| integration expression tests | 363 | 0 |
| integration query tests | 252 | 126 |
| integration render tests | 1299 | 1299 |
| metrics test styles | 68 | 42 |
| platform assets/tests/examples | 198 | 24 |
| plugin examples/other | 1 | 1 |
| plugin render tests | 14 | 13 |
| native other fixtures | 48 | 22 |
| native expression equality | 118 | 0 |
| native parser fixtures | 59 | 30 |
| submodule dependency test data | 939 | 0 |

The 10,450 candidates comprise 9,384 root tracked `.json`, 45 root tracked `.geojson`, and 1,021 dependency test JSON/GeoJSON. Scope labels are path-based: 82 dependency files under `platform/windows/vendor/vcpkg` appear in the platform scope; the owner field identifies them as dependency files. Of 1,573 identified style documents, 1,543 are ordinary style-shaped documents and 30 are parser fixtures; 1,517 distinct normalized JSON hashes remain after byte-independent structural deduplication. Counts include platform examples/shared styles and are not a count of tests executed. There are 1,299 render styles in 175 family folders and 126 query styles in 30 family folders, paired with 126 query expected JSON files.

| Classification | Files |
| --- | --- |
| other JSON/data/config/expected | 627 |
| style document | 1543 |
| metrics recorded result/manifest | 7029 |
| GeoJSON geometry/data | 584 |
| raw expression fixture | 481 |
| query expected result | 126 |
| parser expectations/auxiliary | 29 |
| parser style fixture | 30 |
| JSON parse/read error | 1 |

The style walk includes initial layers/sources and objects supplied by `addLayer`, `addSource` and inline `setStyle` operations: 4,999 layer instances and 1,633 source instances. It observes 163 family/section/property keys in those objects plus 93 property mutations; one mutation has an externally loaded layer, recorded as `<unresolved>/paint/raster-fade-duration` (the property itself also exists on raster styles). 50 distinct expression operators occur in styles. The 481 raw expression fixtures are separate: 363 expression integration fixtures and 118 expression-equality inputs, covering 84 operators. Legacy functions are separately counted: 2,121 occurrences in 226 styles.
Expression traversal uses the C++ registry, skips `literal` payloads, interpolation curve descriptors and match label arrays, and does not interpret coordinates, GeoJSON feature properties, font lists, expected outputs or raw input records as expressions. Legacy filter forms are separately indexed. Native parsing is not run, so valid JSON is not claimed to be a valid style. Supplemental source-code search identifies 152 test source files with style loading/construction calls; it does not execute or fully expand programmatic styles or dynamically assembled fixture strings.

## Layer-family capability matrix

| Family | Instances / style files | Native plugin classification | Reason and boundary | Example |
| --- | --- | --- | --- | --- |
| background | 583 / 559 | capability gap | Source-free backgrounds and background sprite patterns have no native-plugin source-free phase or sampler resource contract. Builtin background remains available. [source-required factory](../../src/mln/plugin/plugin_style_layer_factory.cpp#L13) | [metrics/integration/render-tests/background-color/colorSpace-hcl/style.json](../../metrics/integration/render-tests/background-color/colorSpace-hcl/style.json#L1) |
| circle | 311 / 269 | partial | Point marks, scalar/color/FLOAT2 paint, camera/feature/state evaluation and precise queries fit this path. Circle sort-key/layout ordering and exact builtin equivalence are not established. [paint-only registration](../../src/mln/style/layers/plugin_style_layer.cpp#L86) | [metrics/integration/render-tests/circle-blur/blending/style.json](../../metrics/integration/render-tests/circle-blur/blending/style.json#L1) |
| color-relief | 10 / 8 | capability gap | Elevation-colored DEM rendering needs DEM texture/elevation input; tile feature geometry and four paint value types do not expose that resource/context. [geometry/layout ABI](../../include/mln/plugin/plugin_api.h#L267) | [metrics/integration/render-tests/color-relief/hillshade/style.json](../../metrics/integration/render-tests/color-relief/hillshade/style.json#L1) |
| fill | 614 / 211 | partial | A plugin can triangulate supplied polygons and draw solid paint. Full tile clipping/stencil, patterns, opaque-pass behavior and existing fill ordering are absent from the exposed draw contract. [fixed draw state](../../src/mln/renderer/layers/render_plugin_style_layer.cpp#L243) | [metrics/integration/render-tests/combinations/background-opaque--fill-opaque/style.json](../../metrics/integration/render-tests/combinations/background-opaque--fill-opaque/style.json#L1) |
| fill-extrusion | 111 / 101 | partial | FLOAT3 vertices permit custom 3D meshes, but host lighting, mutable feature-dependent worker layout, depth writes and full extrusion pass behavior are not exposed. [fixed draw state](../../src/mln/renderer/layers/render_plugin_style_layer.cpp#L243) | [metrics/integration/render-tests/combinations/background-opaque--fill-extrusion-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--fill-extrusion-translucent/style.json#L1) |
| heatmap | 41 / 40 | capability gap | Density accumulation plus offscreen target/composite pass and density evaluation context are missing. Scalar weight/radius property evaluation alone is insufficient. [geometry/layout ABI](../../include/mln/plugin/plugin_api.h#L267) | [metrics/integration/render-tests/combinations/background-opaque--heatmap-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--heatmap-translucent/style.json#L1) |
| hillshade | 55 / 52 | capability gap | DEM sampling, hillshade preparation textures and related resource/pass control are not exposed. [geometry/layout ABI](../../include/mln/plugin/plugin_api.h#L267) | [metrics/integration/render-tests/color-relief/hillshade/style.json](../../metrics/integration/render-tests/color-relief/hillshade/style.json#L1) |
| line | 1701 / 253 | partial | Worker geometry permits a basic stroke tessellator. Configurable cap/join/sort layout, dash/pattern atlas, line-distance/gradient evaluation and full clipping/pass behavior do not cross this interface. [paint-only registration](../../src/mln/style/layers/plugin_style_layer.cpp#L86) | [metrics/integration/render-tests/combinations/background-opaque--line-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--line-translucent/style.json#L1) |
| location-indicator | 19 / 19 | capability gap | The builtin uses source-free location/bearing state and images; the new path requires a geometry source and has no image/sampler or location tuple type. [source-required factory](../../src/mln/plugin/plugin_style_layer_factory.cpp#L13) | [metrics/tests/location_indicator/change_image/style.json](../../metrics/tests/location_indicator/change_image/style.json#L1) |
| ngon | 14 / 14 | supported substrate | The example exercises the intended procedural point-mark path; semantic correctness and complete backend parity require the implementation review and execution. [native evaluation](../../src/mln/style/plugin_property.cpp#L93) | [plugins/ngon-layer/examples/interpolation.json](../../plugins/ngon-layer/examples/interpolation.json#L1) |
| plugin-layer-metal-rendering | 3 / 1 | not applicable | Existing Darwin demonstration of the separate C++/Metal plugin layer path; it is not one of the 13 native C-ABI ngon fixtures. [platform/darwin/app/PluginLayerTestStyle.json](../../platform/darwin/app/PluginLayerTestStyle.json#L1) | [platform/darwin/app/PluginLayerTestStyle.json](../../platform/darwin/app/PluginLayerTestStyle.json#L1) |
| raster | 110 / 106 | capability gap | Raster/image/video/canvas pixels, sampler resources and raster crossfade are not exposed through GeometryTileLayer plus shader uniform/attribute resources. [geometry/layout ABI](../../include/mln/plugin/plugin_api.h#L267) | [metrics/integration/render-tests/background-opacity/overlay/style.json](../../metrics/integration/render-tests/background-opacity/overlay/style.json#L1) |
| symbol | 1427 / 660 | capability gap | Glyph shaping, image resolution/atlas, font stacks/formatted text, collision/placement, line labels and variable anchors have no host services or layout-property contract here. [paint-only registration](../../src/mln/style/layers/plugin_style_layer.cpp#L86) | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L1) |

The generic feature callback receives geometry type, source-local feature index and flat tile-coordinate paths; it receives no feature ID/property map, evaluated layout property values, images/glyphs or collision service ([geometry/layout ABI](../../include/mln/plugin/plugin_api.h#L267)). That boundary limits feature-driven topology/layout even though host paint evaluation and feature-state binders exist ([native evaluation](../../src/mln/style/plugin_property.cpp#L93), [paint binder](../../src/mln/renderer/buckets/plugin_bucket.cpp#L160)).

## Source and root-style services

| Source | Instances / style files | Classification | Example |
| --- | --- | --- | --- |
| canvas | 3 / 3 | capability gap | [metrics/integration/render-tests/canvas/default/style.json](../../metrics/integration/render-tests/canvas/default/style.json#L1) |
| geojson | 1072 / 968 | supported host source | [metrics/integration/render-tests/circle-blur/blending/style.json](../../metrics/integration/render-tests/circle-blur/blending/style.json#L1) |
| image | 19 / 19 | capability gap | [metrics/integration/render-tests/combinations/fill-opaque--image-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-opaque--image-translucent/style.json#L1) |
| raster | 89 / 87 | capability gap | [metrics/integration/render-tests/background-opacity/overlay/style.json](../../metrics/integration/render-tests/background-opacity/overlay/style.json#L1) |
| raster-dem | 63 / 56 | capability gap | [metrics/integration/render-tests/color-relief/hillshade/style.json](../../metrics/integration/render-tests/color-relief/hillshade/style.json#L1) |
| vector | 386 / 357 | supported host source | [metrics/integration/render-tests/combinations/color-relief--hillshade--vector-fill--color-relief--hillshade/style.json](../../metrics/integration/render-tests/combinations/color-relief--hillshade--vector-fill--color-relief--hillshade/style.json#L1) |
| video | 1 / 1 | capability gap | [metrics/integration/render-tests/video/default/style.json](../../metrics/integration/render-tests/video/default/style.json#L1) |

Vector and GeoJSON providers remain host-owned and supply GeometryTileLayer features, including source-layer filtering and source-generated/clustered geometry. Registration does not add a custom source provider. Raster, image, raster-dem, video and canvas style presence establishes input use cases, not current backend success; the plugin ABI has neither texture/sampler descriptors nor pixel/DEM source input. Video/canvas fixtures may be platform-specific or ignored by the existing harness.
All observed root keys are inventoried: `bearing` (139), `center` (794), `comment` (1), `font-faces` (3), `glyphs` (454), `id` (23), `layers` (1560), `light` (2), `metadata` (1514), `name` (41), `pitch` (187), `roll` (11), `sources` (1566), `sprite` (492), `transition` (178), `version` (1572), `zoom` (917). Camera position/zoom/bearing/pitch/roll and common layer metadata stay host responsibilities. `sprite` (492 styles), `glyphs` (454) and `font-faces` (3) are builtin resource services the native plugin does not consume. `light` occurs in two root styles; no light context is supplied to plugin uniform/layout callbacks. No `terrain`, `sky` or `projection` root object occurs in this identified JSON corpus; absence is not an implementation claim.

## Every observed property

Rows group by the actual layer family and style section; all keys are listed. The classification describes a plugin analogue, not whether the existing builtin accepts the property. A scalar property on an unsupported rendering family does not make the entire family representable. FLOAT/FLOAT2/COLOR/STRING are the only property value types; strings have an enum GPU encoding, while formatted/resolved-image, boolean, arbitrary arrays, padding and variable-anchor collections are not direct value types ([value types](../../include/mln/plugin/plugin_api.h#L62), [GPU encodings](../../include/mln/plugin/plugin_api.h#L188)). Boolean/array/object expression intermediates can still evaluate to a supported final type.
### <unresolved>

| Section/property | Object occurrences / mutation occurrences | Classification | Example |
| --- | --- | --- | --- |
| paint/raster-fade-duration | 0 / 1 | host source mutation; layer loaded externally | [metrics/integration/render-tests/satellite-v9/z0/style.json](../../metrics/integration/render-tests/satellite-v9/z0/style.json#L1) |

### background

| Section/property | Object occurrences / mutation occurrences | Classification | Example |
| --- | --- | --- | --- |
| layout/visibility | 29 / 6 | supported host common layer property | [metrics/integration/render-tests/background-visibility/none/style.json](../../metrics/integration/render-tests/background-visibility/none/style.json#L15) |
| paint/background-color | 531 / 13 | capability gap: family resources/layout/context | [metrics/integration/render-tests/background-color/colorSpace-hcl/style.json](../../metrics/integration/render-tests/background-color/colorSpace-hcl/style.json#L16) |
| paint/background-color-transition | 1 / 1 | partial: opt-in camera/constant transitions | [metrics/integration/render-tests/background-color/transition/style.json](../../metrics/integration/render-tests/background-color/transition/style.json#L31) |
| paint/background-opacity | 27 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/background-opacity/color/style.json](../../metrics/integration/render-tests/background-opacity/color/style.json#L16) |
| paint/background-pattern | 14 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/background-opacity/image/style.json](../../metrics/integration/render-tests/background-opacity/image/style.json#L17) |

### circle

| Section/property | Object occurrences / mutation occurrences | Classification | Example |
| --- | --- | --- | --- |
| layout/circle-sort-key | 1 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/circle-sort-key/literal/style.json](../../metrics/integration/render-tests/circle-sort-key/literal/style.json#L77) |
| layout/visibility | 1 / 2 | supported host common layer property | [metrics/integration/render-tests/regressions/mapbox-gl-js#7708/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%237708/style.json#L37) |
| paint/circle-blur | 7 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/circle-blur/blending/style.json](../../metrics/integration/render-tests/circle-blur/blending/style.json#L58) |
| paint/circle-color | 143 / 3 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/circle-blur/blending/style.json](../../metrics/integration/render-tests/circle-blur/blending/style.json#L54) |
| paint/circle-opacity | 10 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/circle-opacity/blending/style.json](../../metrics/integration/render-tests/circle-opacity/blending/style.json#L52) |
| paint/circle-pitch-alignment | 12 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/circle-pitch-alignment/map-scale-map/style.json](../../metrics/integration/render-tests/circle-pitch-alignment/map-scale-map/style.json#L46) |
| paint/circle-pitch-scale | 14 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/circle-pitch-alignment/map-scale-map/style.json](../../metrics/integration/render-tests/circle-pitch-alignment/map-scale-map/style.json#L45) |
| paint/circle-radius | 177 / 15 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/circle-blur/blending/style.json](../../metrics/integration/render-tests/circle-blur/blending/style.json#L53) |
| paint/circle-stroke-color | 37 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/circle-blur/literal-stroke/style.json](../../metrics/integration/render-tests/circle-blur/literal-stroke/style.json#L51) |
| paint/circle-stroke-opacity | 5 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/circle-stroke-opacity/function/style.json](../../metrics/integration/render-tests/circle-stroke-opacity/function/style.json#L51) |
| paint/circle-stroke-width | 50 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/circle-blur/literal-stroke/style.json](../../metrics/integration/render-tests/circle-blur/literal-stroke/style.json#L50) |
| paint/circle-translate | 17 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/circle-translate-anchor/map/style.json](../../metrics/integration/render-tests/circle-translate-anchor/map/style.json#L50) |
| paint/circle-translate-anchor | 4 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/circle-translate-anchor/map/style.json](../../metrics/integration/render-tests/circle-translate-anchor/map/style.json#L54) |

### color-relief

| Section/property | Object occurrences / mutation occurrences | Classification | Example |
| --- | --- | --- | --- |
| paint/color-relief-color | 10 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/color-relief/hillshade/style.json](../../metrics/integration/render-tests/color-relief/hillshade/style.json#L28) |
| paint/color-relief-opacity | 10 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/color-relief/hillshade/style.json](../../metrics/integration/render-tests/color-relief/hillshade/style.json#L27) |

### fill

| Section/property | Object occurrences / mutation occurrences | Classification | Example |
| --- | --- | --- | --- |
| layout/fill-sort-key | 1 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/fill-sort-key/literal/style.json](../../metrics/integration/render-tests/fill-sort-key/literal/style.json#L85) |
| layout/visibility | 296 / 0 | supported host common layer property | [metrics/integration/render-tests/fill-visibility/none/style.json](../../metrics/integration/render-tests/fill-visibility/none/style.json#L37) |
| paint/fill-antialias | 138 / 0 | partial: shader equivalent possible; no bool property type | [metrics/integration/render-tests/custom-layer-js/depth/style.json](../../metrics/integration/render-tests/custom-layer-js/depth/style.json#L61) |
| paint/fill-color | 527 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/combinations/background-opaque--fill-opaque/style.json](../../metrics/integration/render-tests/combinations/background-opaque--fill-opaque/style.json#L59) |
| paint/fill-opacity | 275 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/combinations/background-opaque--fill-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--fill-translucent/style.json#L59) |
| paint/fill-outline-color | 42 / 2 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/fill-outline-color/fill/style.json](../../metrics/integration/render-tests/fill-outline-color/fill/style.json#L49) |
| paint/fill-pattern | 37 / 2 | capability gap: resources/value/context | [metrics/integration/render-tests/fill-opacity/property-function-pattern/style.json](../../metrics/integration/render-tests/fill-opacity/property-function-pattern/style.json#L126) |
| paint/fill-translate | 9 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/fill-translate-anchor/map/style.json](../../metrics/integration/render-tests/fill-translate-anchor/map/style.json#L49) |
| paint/fill-translate-anchor | 5 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/fill-translate-anchor/map/style.json](../../metrics/integration/render-tests/fill-translate-anchor/map/style.json#L53) |

### fill-extrusion

| Section/property | Object occurrences / mutation occurrences | Classification | Example |
| --- | --- | --- | --- |
| layout/visibility | 12 / 0 | supported host common layer property | [benchmark/fixtures/api/style.json](../../benchmark/fixtures/api/style.json#L38) |
| paint/fill-extrusion-base | 25 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/feature-state/promote-id-fill-extrusion/style.json](../../metrics/integration/render-tests/feature-state/promote-id-fill-extrusion/style.json#L149) |
| paint/fill-extrusion-color | 77 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/combinations/background-opaque--fill-extrusion-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--fill-extrusion-translucent/style.json#L59) |
| paint/fill-extrusion-height | 83 / 1 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/custom-layer-js/tent-3d/style.json](../../metrics/integration/render-tests/custom-layer-js/tent-3d/style.json#L132) |
| paint/fill-extrusion-opacity | 22 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/combinations/fill-extrusion-translucent--fill-opaque/style.json](../../metrics/integration/render-tests/combinations/fill-extrusion-translucent--fill-opaque/style.json#L52) |
| paint/fill-extrusion-pattern | 8 / 0 | capability gap: resources/value/context | [metrics/integration/render-tests/fill-extrusion-pattern/@2x/style.json](../../metrics/integration/render-tests/fill-extrusion-pattern/@2x/style.json#L95) |
| paint/fill-extrusion-translate | 6 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/fill-extrusion-translate-anchor/map/style.json](../../metrics/integration/render-tests/fill-extrusion-translate-anchor/map/style.json#L52) |
| paint/fill-extrusion-translate-anchor | 2 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/fill-extrusion-translate-anchor/map/style.json](../../metrics/integration/render-tests/fill-extrusion-translate-anchor/map/style.json#L53) |
| paint/fill-extrusion-vertical-gradient | 1 / 0 | partial: shader equivalent possible; no bool property type | [metrics/integration/render-tests/fill-extrusion-vertical-gradient/false/style.json](../../metrics/integration/render-tests/fill-extrusion-vertical-gradient/false/style.json#L98) |

### heatmap

| Section/property | Object occurrences / mutation occurrences | Classification | Example |
| --- | --- | --- | --- |
| paint/heatmap-color | 2 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/heatmap-color/expression/style.json](../../metrics/integration/render-tests/heatmap-color/expression/style.json#L38) |
| paint/heatmap-intensity | 4 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/heatmap-intensity/function/style.json](../../metrics/integration/render-tests/heatmap-intensity/function/style.json#L38) |
| paint/heatmap-opacity | 24 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/combinations/background-opaque--heatmap-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--heatmap-translucent/style.json#L59) |
| paint/heatmap-radius | 6 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/heatmap-radius/antimeridian/style.json](../../metrics/integration/render-tests/heatmap-radius/antimeridian/style.json#L33) |
| paint/heatmap-weight | 3 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/heatmap-weight/identity-property-function/style.json](../../metrics/integration/render-tests/heatmap-weight/identity-property-function/style.json#L39) |

### hillshade

| Section/property | Object occurrences / mutation occurrences | Classification | Example |
| --- | --- | --- | --- |
| layout/visibility | 1 / 0 | supported host common layer property | [platform/android/MapLibreAndroidTestApp/src/main/assets/outdoor.json](../../platform/android/MapLibreAndroidTestApp/src/main/assets/outdoor.json#L30) |
| paint/hillshade-accent-color | 7 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/combinations/color-relief--hillshade--color-relief--hillshade/style.json](../../metrics/integration/render-tests/combinations/color-relief--hillshade--color-relief--hillshade/style.json#L75) |
| paint/hillshade-exaggeration | 27 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/combinations/background-opaque--hillshade-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--hillshade-translucent/style.json#L37) |
| paint/hillshade-highlight-color | 10 / 1 | capability gap: family resources/layout/context | [metrics/integration/render-tests/combinations/color-relief--hillshade--color-relief--hillshade/style.json](../../metrics/integration/render-tests/combinations/color-relief--hillshade--color-relief--hillshade/style.json#L74) |
| paint/hillshade-illumination-altitude | 3 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/hillshade-alt/basic-0/style.json](../../metrics/integration/render-tests/hillshade-alt/basic-0/style.json#L35) |
| paint/hillshade-illumination-anchor | 2 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/combinations/color-relief--hillshade--color-relief--hillshade/style.json](../../metrics/integration/render-tests/combinations/color-relief--hillshade--color-relief--hillshade/style.json#L71) |
| paint/hillshade-illumination-direction | 3 / 1 | capability gap: family resources/layout/context | [metrics/integration/render-tests/hillshade-low-zoom/multidirectional/style.json](../../metrics/integration/render-tests/hillshade-low-zoom/multidirectional/style.json#L38) |
| paint/hillshade-method | 18 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/combinations/color-relief--hillshade--color-relief--hillshade/style.json](../../metrics/integration/render-tests/combinations/color-relief--hillshade--color-relief--hillshade/style.json#L70) |
| paint/hillshade-shadow-color | 10 / 1 | capability gap: family resources/layout/context | [metrics/integration/render-tests/combinations/color-relief--hillshade--color-relief--hillshade/style.json](../../metrics/integration/render-tests/combinations/color-relief--hillshade--color-relief--hillshade/style.json#L73) |

### line

| Section/property | Object occurrences / mutation occurrences | Classification | Example |
| --- | --- | --- | --- |
| layout/line-cap | 675 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/icon-offset/pitched-offset/style.json](../../metrics/integration/render-tests/icon-offset/pitched-offset/style.json#L43) |
| layout/line-join | 1125 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/icon-offset/pitched-offset/style.json](../../metrics/integration/render-tests/icon-offset/pitched-offset/style.json#L44) |
| layout/line-miter-limit | 36 / 0 | capability gap: plugin-defined layout properties/worker values | [platform/android/MapLibreAndroidTestApp/src/main/assets/outdoor.json](../../platform/android/MapLibreAndroidTestApp/src/main/assets/outdoor.json#L1884) |
| layout/line-sort-key | 1 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/line-sort-key/literal/style.json](../../metrics/integration/render-tests/line-sort-key/literal/style.json#L61) |
| layout/visibility | 985 / 1 | supported host common layer property | [metrics/integration/render-tests/icon-offset/pitched-offset/style.json](../../metrics/integration/render-tests/icon-offset/pitched-offset/style.json#L34) |
| paint/line-blur | 11 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/line-blur/function/style.json](../../metrics/integration/render-tests/line-blur/function/style.json#L38) |
| paint/line-color | 1546 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/combinations/background-opaque--line-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--line-translucent/style.json#L59) |
| paint/line-dasharray | 564 / 0 | capability gap: resources/value/context | [metrics/integration/render-tests/icon-offset/pitched-offset/style.json](../../metrics/integration/render-tests/icon-offset/pitched-offset/style.json#L58) |
| paint/line-gap-width | 38 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/line-gap-width/function/style.json](../../metrics/integration/render-tests/line-gap-width/function/style.json#L68) |
| paint/line-gradient | 3 / 0 | capability gap: resources/value/context | [metrics/integration/render-tests/line-gradient/gradient-tile-boundaries/style.json](../../metrics/integration/render-tests/line-gradient/gradient-tile-boundaries/style.json#L42) |
| paint/line-offset | 71 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/icon-offset/pitched-offset/style.json](../../metrics/integration/render-tests/icon-offset/pitched-offset/style.json#L59) |
| paint/line-opacity | 465 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/line-cap/butt/style.json](../../metrics/integration/render-tests/line-cap/butt/style.json#L41) |
| paint/line-pattern | 23 / 1 | capability gap: resources/value/context | [metrics/integration/render-tests/line-pattern/@2x/style.json](../../metrics/integration/render-tests/line-pattern/@2x/style.json#L68) |
| paint/line-translate | 9 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/line-translate-anchor/map/style.json](../../metrics/integration/render-tests/line-translate-anchor/map/style.json#L39) |
| paint/line-translate-anchor | 5 / 0 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/line-translate-anchor/map/style.json](../../metrics/integration/render-tests/line-translate-anchor/map/style.json#L43) |
| paint/line-width | 1596 / 1 | partial: numeric/color/float2/enum paint; family rendering limits | [metrics/integration/render-tests/debug/padding/ease-to-btm-distort/style.json](../../metrics/integration/render-tests/debug/padding/ease-to-btm-distort/style.json#L108) |

### location-indicator

| Section/property | Object occurrences / mutation occurrences | Classification | Example |
| --- | --- | --- | --- |
| layout/bearing-image | 17 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/tests/location_indicator/change_image/style.json](../../metrics/tests/location_indicator/change_image/style.json#L86) |
| layout/shadow-image | 17 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/tests/location_indicator/change_image/style.json](../../metrics/tests/location_indicator/change_image/style.json#L88) |
| layout/top-image | 16 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/tests/location_indicator/change_image/style.json](../../metrics/tests/location_indicator/change_image/style.json#L87) |
| paint/accuracy-radius | 19 / 0 | capability gap: family resources/layout/context | [metrics/tests/location_indicator/change_image/style.json](../../metrics/tests/location_indicator/change_image/style.json#L95) |
| paint/accuracy-radius-border-color | 18 / 0 | capability gap: family resources/layout/context | [metrics/tests/location_indicator/change_image/style.json](../../metrics/tests/location_indicator/change_image/style.json#L101) |
| paint/accuracy-radius-color | 18 / 0 | capability gap: family resources/layout/context | [metrics/tests/location_indicator/change_image/style.json](../../metrics/tests/location_indicator/change_image/style.json#L100) |
| paint/bearing | 19 / 0 | capability gap: family resources/layout/context | [metrics/tests/location_indicator/change_image/style.json](../../metrics/tests/location_indicator/change_image/style.json#L64) |
| paint/bearing-image-size | 19 / 0 | capability gap: family resources/layout/context | [metrics/tests/location_indicator/change_image/style.json](../../metrics/tests/location_indicator/change_image/style.json#L96) |
| paint/image-tilt-displacement | 19 / 0 | capability gap: family resources/layout/context | [metrics/tests/location_indicator/change_image/style.json](../../metrics/tests/location_indicator/change_image/style.json#L93) |
| paint/location | 19 / 0 | capability gap: family resources/layout/context | [metrics/tests/location_indicator/change_image/style.json](../../metrics/tests/location_indicator/change_image/style.json#L94) |
| paint/perspective-compensation | 19 / 0 | capability gap: family resources/layout/context | [metrics/tests/location_indicator/change_image/style.json](../../metrics/tests/location_indicator/change_image/style.json#L92) |
| paint/shadow-image-size | 19 / 0 | capability gap: family resources/layout/context | [metrics/tests/location_indicator/change_image/style.json](../../metrics/tests/location_indicator/change_image/style.json#L98) |
| paint/top-image-size | 19 / 0 | capability gap: family resources/layout/context | [metrics/tests/location_indicator/change_image/style.json](../../metrics/tests/location_indicator/change_image/style.json#L97) |

### ngon

| Section/property | Object occurrences / mutation occurrences | Classification | Example |
| --- | --- | --- | --- |
| paint/ngon-blur | 2 / 0 | supported substrate: registered paint binding | [plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json](../../plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json#L230) |
| paint/ngon-color | 14 / 1 | supported substrate: registered paint binding | [plugins/ngon-layer/examples/interpolation.json](../../plugins/ngon-layer/examples/interpolation.json#L85) |
| paint/ngon-corners | 14 / 1 | supported substrate: registered paint binding | [plugins/ngon-layer/examples/interpolation.json](../../plugins/ngon-layer/examples/interpolation.json#L78) |
| paint/ngon-opacity | 3 / 0 | supported substrate: registered paint binding | [plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json](../../plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json#L234) |
| paint/ngon-pitch-alignment | 5 / 0 | supported substrate: registered paint binding | [plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json](../../plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json#L258) |
| paint/ngon-pitch-scale | 5 / 0 | supported substrate: registered paint binding | [plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json](../../plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json#L262) |
| paint/ngon-radius | 14 / 1 | supported substrate: registered paint binding | [plugins/ngon-layer/examples/interpolation.json](../../plugins/ngon-layer/examples/interpolation.json#L72) |
| paint/ngon-rotate | 6 / 1 | supported substrate: registered paint binding | [plugins/ngon-layer/examples/interpolation.json](../../plugins/ngon-layer/examples/interpolation.json#L79) |
| paint/ngon-stroke-color | 5 / 0 | supported substrate: registered paint binding | [plugins/ngon-layer/examples/interpolation.json](../../plugins/ngon-layer/examples/interpolation.json#L94) |
| paint/ngon-stroke-opacity | 2 / 0 | supported substrate: registered paint binding | [plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json](../../plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json#L246) |
| paint/ngon-stroke-width | 11 / 0 | supported substrate: registered paint binding | [plugins/ngon-layer/examples/interpolation.json](../../plugins/ngon-layer/examples/interpolation.json#L91) |
| paint/ngon-translate | 3 / 0 | supported substrate: registered paint binding | [plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json](../../plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json#L250) |
| paint/ngon-translate-anchor | 2 / 0 | supported substrate: registered paint binding | [plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json](../../plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json#L254) |

### plugin-layer-metal-rendering

| Section/property | Object occurrences / mutation occurrences | Classification | Example |
| --- | --- | --- | --- |
| paint/fill-color | 1 / 0 | not applicable: separate plugin path | [platform/darwin/app/PluginLayerTestStyle.json](../../platform/darwin/app/PluginLayerTestStyle.json#L72) |
| paint/scale | 3 / 0 | not applicable: separate plugin path | [platform/darwin/app/PluginLayerTestStyle.json](../../platform/darwin/app/PluginLayerTestStyle.json#L538) |

### raster

| Section/property | Object occurrences / mutation occurrences | Classification | Example |
| --- | --- | --- | --- |
| layout/visibility | 6 / 0 | supported host common layer property | [metrics/integration/render-tests/raster-visibility/none/style.json](../../metrics/integration/render-tests/raster-visibility/none/style.json#L36) |
| paint/raster-brightness-max | 4 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/image/raster-brightness/style.json](../../metrics/integration/render-tests/image/raster-brightness/style.json#L46) |
| paint/raster-brightness-min | 4 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/image/raster-brightness/style.json](../../metrics/integration/render-tests/image/raster-brightness/style.json#L45) |
| paint/raster-contrast | 4 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/image/raster-contrast/style.json](../../metrics/integration/render-tests/image/raster-contrast/style.json#L45) |
| paint/raster-fade-duration | 105 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/background-opacity/overlay/style.json](../../metrics/integration/render-tests/background-opacity/overlay/style.json#L29) |
| paint/raster-hue-rotate | 5 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/image/raster-hue-rotate/style.json](../../metrics/integration/render-tests/image/raster-hue-rotate/style.json#L45) |
| paint/raster-opacity | 36 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/combinations/background-opaque--raster-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--raster-translucent/style.json#L38) |
| paint/raster-resampling | 4 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/image/raster-resampling/style.json](../../metrics/integration/render-tests/image/raster-resampling/style.json#L45) |
| paint/raster-saturation | 3 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/image/raster-saturation/style.json](../../metrics/integration/render-tests/image/raster-saturation/style.json#L45) |

### symbol

| Section/property | Object occurrences / mutation occurrences | Classification | Example |
| --- | --- | --- | --- |
| layout/icon-allow-overlap | 361 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/combinations/symbol-translucent--symbol-translucent/style.json](../../metrics/integration/render-tests/combinations/symbol-translucent--symbol-translucent/style.json#L53) |
| layout/icon-anchor | 47 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/icon-anchor/bottom-left/style.json](../../metrics/integration/render-tests/icon-anchor/bottom-left/style.json#L29) |
| layout/icon-ignore-placement | 320 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/feature-state/symbol-paint/style.json](../../metrics/integration/render-tests/feature-state/symbol-paint/style.json#L69) |
| layout/icon-image | 825 / 12 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/combinations/background-opaque--symbol-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--symbol-translucent/style.json#L59) |
| layout/icon-offset | 46 / 1 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/icon-no-cross-source-collision/default/style.json](../../metrics/integration/render-tests/icon-no-cross-source-collision/default/style.json#L99) |
| layout/icon-optional | 18 / 0 | capability gap: plugin-defined layout properties/worker values | [benchmark/fixtures/api/style.json](../../benchmark/fixtures/api/style.json#L6190) |
| layout/icon-padding | 31 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/icon-padding/databind/style.json](../../metrics/integration/render-tests/icon-padding/databind/style.json#L53) |
| layout/icon-pitch-alignment | 7 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/icon-offset/pitched-offset/style.json](../../metrics/integration/render-tests/icon-offset/pitched-offset/style.json#L84) |
| layout/icon-rotate | 53 / 13 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/icon-rotate/literal/style.json](../../metrics/integration/render-tests/icon-rotate/literal/style.json#L50) |
| layout/icon-rotation-alignment | 85 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/icon-offset/pitched-offset/style.json](../../metrics/integration/render-tests/icon-offset/pitched-offset/style.json#L83) |
| layout/icon-size | 166 / 3 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/icon-offset/pitched-offset/style.json](../../metrics/integration/render-tests/icon-offset/pitched-offset/style.json#L85) |
| layout/icon-text-fit | 270 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/icon-text-fit/both-collision-variable-anchor-text-fit/style.json](../../metrics/integration/render-tests/icon-text-fit/both-collision-variable-anchor-text-fit/style.json#L52) |
| layout/icon-text-fit-padding | 44 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/icon-text-fit/both-collision-variable-anchor-text-fit/style.json](../../metrics/integration/render-tests/icon-text-fit/both-collision-variable-anchor-text-fit/style.json#L53) |
| layout/symbol-avoid-edges | 37 / 0 | capability gap: plugin-defined layout properties/worker values | [benchmark/fixtures/api/style.json](../../benchmark/fixtures/api/style.json#L5788) |
| layout/symbol-placement | 407 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/combinations/background-opaque--symbol-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--symbol-translucent/style.json#L60) |
| layout/symbol-sort-key | 6 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/symbol-sort-key/icon-expression/style.json](../../metrics/integration/render-tests/symbol-sort-key/icon-expression/style.json#L70) |
| layout/symbol-spacing | 158 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json](../../metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json#L55) |
| layout/symbol-z-order | 5 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/symbol-z-order/disabled/style.json](../../metrics/integration/render-tests/symbol-z-order/disabled/style.json#L72) |
| layout/text-allow-overlap | 381 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/debug/tile-overscaled/style.json](../../metrics/integration/render-tests/debug/tile-overscaled/style.json#L41) |
| layout/text-anchor | 447 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/icon-text-fit/both-collision-variable-anchor-text-fit/style.json](../../metrics/integration/render-tests/icon-text-fit/both-collision-variable-anchor-text-fit/style.json#L43) |
| layout/text-field | 1092 / 3 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L187) |
| layout/text-font | 1096 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L214) |
| layout/text-ignore-placement | 329 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/extent/1024-symbol/style.json](../../metrics/integration/render-tests/extent/1024-symbol/style.json#L59) |
| layout/text-justify | 83 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/text-field/formatted-images-variable-anchors-justification/style.json](../../metrics/integration/render-tests/text-field/formatted-images-variable-anchors-justification/style.json#L92) |
| layout/text-keep-upright | 16 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/text-keep-upright/line-placement-false/style.json](../../metrics/integration/render-tests/text-keep-upright/line-placement-false/style.json#L45) |
| layout/text-letter-spacing | 60 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/regressions/mapbox-gl-js#6919/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%236919/style.json#L44) |
| layout/text-line-height | 5 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/text-line-height/literal/style.json](../../metrics/integration/render-tests/text-line-height/literal/style.json#L51) |
| layout/text-max-angle | 21 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/debug/collision-lines-pitched/style.json](../../metrics/integration/render-tests/debug/collision-lines-pitched/style.json#L45) |
| layout/text-max-width | 389 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L219) |
| layout/text-offset | 198 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/feature-state/symbol-paint/style.json](../../metrics/integration/render-tests/feature-state/symbol-paint/style.json#L79) |
| layout/text-optional | 17 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/regressions/mapbox-gl-js#7032/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%237032/style.json#L54) |
| layout/text-padding | 91 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/text-pitch-alignment/viewport-overzoomed-single-glyph/style.json](../../metrics/integration/render-tests/text-pitch-alignment/viewport-overzoomed-single-glyph/style.json#L60) |
| layout/text-pitch-alignment | 17 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/regressions/mapbox-gl-js#4860/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234860/style.json#L32) |
| layout/text-radial-offset | 19 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/icon-text-fit/both-collision-variable-anchor-text-fit/style.json](../../metrics/integration/render-tests/icon-text-fit/both-collision-variable-anchor-text-fit/style.json#L46) |
| layout/text-rotate | 10 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/text-keep-upright/point-placement-align-viewport-false/style.json](../../metrics/integration/render-tests/text-keep-upright/point-placement-align-viewport-false/style.json#L46) |
| layout/text-rotation-alignment | 90 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/regressions/mapbox-gl-js#4860/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234860/style.json#L31) |
| layout/text-size | 798 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L218) |
| layout/text-transform | 115 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/text-transform/lowercase/style.json](../../metrics/integration/render-tests/text-transform/lowercase/style.json#L51) |
| layout/text-variable-anchor | 36 / 1 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/icon-text-fit/both-collision-variable-anchor-text-fit/style.json](../../metrics/integration/render-tests/icon-text-fit/both-collision-variable-anchor-text-fit/style.json#L45) |
| layout/text-variable-anchor-offset | 20 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/text-variable-anchor-offset/all-anchors-icon-text-fit/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/all-anchors-icon-text-fit/style.json#L53) |
| layout/text-writing-mode | 31 / 0 | capability gap: plugin-defined layout properties/worker values | [metrics/integration/render-tests/icon-text-fit/textFit-grid-long-vertical/style.json](../../metrics/integration/render-tests/icon-text-fit/textFit-grid-long-vertical/style.json#L28) |
| layout/visibility | 214 / 0 | supported host common layer property | [metrics/integration/render-tests/icon-visibility/none/style.json](../../metrics/integration/render-tests/icon-visibility/none/style.json#L47) |
| paint.visible/icon-opacity | 1 / 0 | legacy style class; not native-plugin contract | [test/fixtures/resources/style-unused-sources.json](../../test/fixtures/resources/style-unused-sources.json#L23) |
| paint.visible/text-opacity | 1 / 0 | legacy style class; not native-plugin contract | [test/fixtures/resources/style-unused-sources.json](../../test/fixtures/resources/style-unused-sources.json#L24) |
| paint/icon-color | 44 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/combinations/background-opaque--symbol-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--symbol-translucent/style.json#L63) |
| paint/icon-halo-blur | 4 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/icon-halo-blur/function/style.json](../../metrics/integration/render-tests/icon-halo-blur/function/style.json#L33) |
| paint/icon-halo-color | 15 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/icon-halo-blur/default/style.json](../../metrics/integration/render-tests/icon-halo-blur/default/style.json#L31) |
| paint/icon-halo-width | 15 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/icon-halo-blur/default/style.json](../../metrics/integration/render-tests/icon-halo-blur/default/style.json#L32) |
| paint/icon-opacity | 107 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json](../../metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json#L58) |
| paint/icon-translate | 6 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json](../../metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json#L60) |
| paint/icon-translate-anchor | 2 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/icon-translate-anchor/map/style.json](../../metrics/integration/render-tests/icon-translate-anchor/map/style.json#L53) |
| paint/text-color | 471 / 3 | capability gap: family resources/layout/context | [metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json](../../metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json#L59) |
| paint/text-halo-blur | 245 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/text-halo-blur/function/style.json](../../metrics/integration/render-tests/text-halo-blur/function/style.json#L37) |
| paint/text-halo-color | 393 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/symbol-sort-key/text-expression/style.json](../../metrics/integration/render-tests/symbol-sort-key/text-expression/style.json#L83) |
| paint/text-halo-width | 407 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/symbol-sort-key/text-expression/style.json](../../metrics/integration/render-tests/symbol-sort-key/text-expression/style.json#L82) |
| paint/text-opacity | 78 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/icon-opacity/icon-only/style.json](../../metrics/integration/render-tests/icon-opacity/icon-only/style.json#L57) |
| paint/text-translate | 8 / 1 | capability gap: family resources/layout/context | [metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json](../../metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json#L69) |
| paint/text-translate-anchor | 2 / 0 | capability gap: family resources/layout/context | [metrics/integration/render-tests/text-translate-anchor/map/style.json](../../metrics/integration/render-tests/text-translate-anchor/map/style.json#L57) |


## Every observed expression operator

Native host expression conversion/evaluation is implemented, including dependency gating and feature-state support ([native evaluation](../../src/mln/style/plugin_property.cpp#L93), [conversion and capability checks](../../src/mln/style/plugin_property.cpp#L333)). Arithmetic, decision, lookup and conversion operators are not blanket unsupported. Layout/glyph/image/offscreen service gaps remain separate from evaluation. Specialized heatmap density, line progress and elevation inputs are absent from `EvaluationContext(zoom, feature, state)` used by plugin paint. `within`/`distance` need geometry/canonical-tile context; the filter path supplies canonical tile ID, while plugin paint evaluation does not ([filter context](../../src/mln/layout/plugin_layout.cpp#L91)).
| Operator | Style occurrences / raw-fixture occurrences | Classification | Style example |
| --- | --- | --- | --- |
| ! | 2 / 5 | supported native evaluator subject to result type/dependencies | [benchmark/fixtures/renderer/liberty.json](../../benchmark/fixtures/renderer/liberty.json#L1) |
| != | 20 / 6 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L1) |
| % | 3 / 3 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/symbol-placement/line-center-tile-map-mode/style.json](../../metrics/integration/render-tests/symbol-placement/line-center-tile-map-mode/style.json#L1) |
| * | 3 / 9 | supported native evaluator subject to result type/dependencies | [plugins/ngon-layer/examples/interpolation.json](../../plugins/ngon-layer/examples/interpolation.json#L1) |
| + | 11 / 23 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/fill-extrusion-vertical-gradient/default/style.json](../../metrics/integration/render-tests/fill-extrusion-vertical-gradient/default/style.json#L1) |
| - | 4 / 7 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/line-dasharray/zero-length-gap/style.json](../../metrics/integration/render-tests/line-dasharray/zero-length-gap/style.json#L1) |
| / | 6 / 8 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/regressions/mapbox-gl-js#4172/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234172/style.json#L1) |
| < | 3 / 12 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L1) |
| <= | 5 / 7 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L1) |
| == | 118 / 23 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L1) |
| > | 1 / 9 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L1) |
| >= | 6 / 8 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L1) |
| ^ | 0 / 3 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| abs | 0 / 1 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| acos | 0 / 3 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| all | 77 / 9 | supported native evaluator subject to result type/dependencies | [benchmark/fixtures/renderer/liberty.json](../../benchmark/fixtures/renderer/liberty.json#L1) |
| any | 0 / 6 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| array | 11 / 24 | supported evaluator; final result must fit float/float2/color/string | [metrics/integration/render-tests/regressions/mapbox-gl-native#10849/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%2310849/style.json#L1) |
| asin | 0 / 3 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| at | 1 / 5 | supported evaluator; final result must fit float/float2/color/string | [test/fixtures/style_parser/text-font.style.json](../../test/fixtures/style_parser/text-font.style.json#L1) |
| atan | 0 / 3 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| boolean | 15 / 36 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/fill-pattern/update-feature-state/style.json](../../metrics/integration/render-tests/fill-pattern/update-feature-state/style.json#L1) |
| case | 51 / 7 | supported evaluator; final result must fit float/float2/color/string | [metrics/integration/render-tests/fill-pattern/case-data-expression/style.json](../../metrics/integration/render-tests/fill-pattern/case-data-expression/style.json#L1) |
| ceil | 0 / 1 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| coalesce | 38 / 12 | supported evaluator; final result must fit float/float2/color/string | [metrics/integration/render-tests/feature-state/composite-expression/style.json](../../metrics/integration/render-tests/feature-state/composite-expression/style.json#L1) |
| collator | 2 / 28 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L1) |
| concat | 25 / 7 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L1) |
| cos | 0 / 4 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| distance | 0 / 2 | partial: host filter geometry context; paint evaluator omits canonical tile context | raw expression corpus only |
| downcase | 0 / 3 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| e | 0 / 3 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| elevation | 10 / 0 | capability gap: specialized evaluation context absent | [metrics/integration/render-tests/color-relief/hillshade/style.json](../../metrics/integration/render-tests/color-relief/hillshade/style.json#L1) |
| error | 0 / 2 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| feature-state | 29 / 0 | supported host evaluator; first frame after constant-to-state mutation remains runtime-pending (F20) | [metrics/integration/render-tests/feature-state/composite-expression/style.json](../../metrics/integration/render-tests/feature-state/composite-expression/style.json#L1) |
| floor | 0 / 1 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| format | 28 / 6 | capability gap as property result: formatted/resolvedImage types and image resources absent | [metrics/integration/render-tests/runtime-styling/layout-property-override-paint-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-override-paint-property-expression/style.json#L1) |
| geometry-type | 20 / 1 | supported native evaluator subject to result type/dependencies | [benchmark/fixtures/renderer/liberty.json](../../benchmark/fixtures/renderer/liberty.json#L1) |
| get | 548 / 348 | supported evaluator; final result must fit float/float2/color/string | [metrics/integration/render-tests/circle-blur/blending/style.json](../../metrics/integration/render-tests/circle-blur/blending/style.json#L1) |
| has | 22 / 3 | supported native evaluator subject to result type/dependencies | [benchmark/fixtures/renderer/liberty.json](../../benchmark/fixtures/renderer/liberty.json#L1) |
| heatmap-density | 1 / 3 | capability gap: specialized evaluation context absent | [metrics/integration/render-tests/heatmap-color/expression/style.json](../../metrics/integration/render-tests/heatmap-color/expression/style.json#L1) |
| id | 3 / 1 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/symbol-placement/line-center-tile-map-mode/style.json](../../metrics/integration/render-tests/symbol-placement/line-center-tile-map-mode/style.json#L1) |
| image | 26 / 9 | ABI result/resource gap; numeric branches also have missing image-availability context (F16 in main review) | [metrics/integration/render-tests/icon-image/image-expression/style.json](../../metrics/integration/render-tests/icon-image/image-expression/style.json#L1) |
| in | 0 / 8 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| index-of | 0 / 7 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| interpolate | 167 / 31 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/color-relief/hillshade/style.json](../../metrics/integration/render-tests/color-relief/hillshade/style.json#L1) |
| interpolate-hcl | 0 / 1 | raw legacy fixture; not in current native operator registry | raw expression corpus only |
| interpolate-lab | 0 / 1 | raw legacy fixture; not in current native operator registry | raw expression corpus only |
| is-supported-script | 2 / 1 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/is-supported-script/filter/style.json](../../metrics/integration/render-tests/is-supported-script/filter/style.json#L1) |
| join | 0 / 6 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| length | 2 / 4 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/text-offset/semiliteral/style.json](../../metrics/integration/render-tests/text-offset/semiliteral/style.json#L1) |
| let | 3 / 18 | supported evaluator; final result must fit float/float2/color/string | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L1) |
| line-progress | 3 / 0 | capability gap: specialized evaluation context absent | [metrics/integration/render-tests/line-gradient/gradient-tile-boundaries/style.json](../../metrics/integration/render-tests/line-gradient/gradient-tile-boundaries/style.json#L1) |
| literal | 37 / 39 | supported evaluator; final result must fit float/float2/color/string | [metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json](../../metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json#L1) |
| ln | 0 / 3 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| ln2 | 0 / 1 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| log10 | 0 / 3 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| log2 | 0 / 3 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| match | 118 / 26 | supported evaluator; final result must fit float/float2/color/string | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L1) |
| max | 0 / 6 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| min | 0 / 6 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| number | 17 / 80 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/text-size/composite-expression/style.json](../../metrics/integration/render-tests/text-size/composite-expression/style.json#L1) |
| number-format | 0 / 9 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| object | 0 / 5 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| pi | 1 / 1 | supported native evaluator subject to result type/dependencies | [test/fixtures/style_parser/expressions.style.json](../../test/fixtures/style_parser/expressions.style.json#L1) |
| properties | 0 / 1 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| resolved-locale | 1 / 2 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/collator/resolved-locale/style.json](../../metrics/integration/render-tests/collator/resolved-locale/style.json#L1) |
| rgb | 0 / 7 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| rgba | 2 / 8 | supported native evaluator subject to result type/dependencies | [test/fixtures/style_parser/expressions.style.json](../../test/fixtures/style_parser/expressions.style.json#L1) |
| round | 0 / 3 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| semiliteral | 1 / 16 | supported host evaluator for typed FLOAT2/fitting results; children evaluated; dependency and zoom rules remain | [metrics/integration/render-tests/text-offset/semiliteral/style.json](../../metrics/integration/render-tests/text-offset/semiliteral/style.json#L1) |
| sin | 0 / 4 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| slice | 0 / 5 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| split | 0 / 4 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| sqrt | 1 / 3 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/text-size/nan/style.json](../../metrics/integration/render-tests/text-size/nan/style.json#L1) |
| step | 61 / 6 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/feature-state/composite-expression/style.json](../../metrics/integration/render-tests/feature-state/composite-expression/style.json#L1) |
| string | 7 / 48 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/line-opacity/step-curve/style.json](../../metrics/integration/render-tests/line-opacity/step-curve/style.json#L1) |
| tan | 0 / 3 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| to-boolean | 0 / 4 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| to-color | 25 / 7 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/symbol-placement/line-center-tile-map-mode/style.json](../../metrics/integration/render-tests/symbol-placement/line-center-tile-map-mode/style.json#L1) |
| to-number | 2 / 4 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/symbol-placement/line-center-tile-map-mode/style.json](../../metrics/integration/render-tests/symbol-placement/line-center-tile-map-mode/style.json#L1) |
| to-rgba | 0 / 4 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| to-string | 9 / 7 | supported native evaluator subject to result type/dependencies | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L1) |
| typeof | 0 / 3 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| upcase | 0 / 3 | supported native evaluator subject to result type/dependencies | raw expression corpus only |
| var | 24 / 28 | supported evaluator; final result must fit float/float2/color/string | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L1) |
| within | 6 / 6 | partial: host filter geometry context; paint evaluator omits canonical tile context | [metrics/integration/render-tests/within/filter-with-inlined-geojson/style.json](../../metrics/integration/render-tests/within/filter-with-inlined-geojson/style.json#L1) |
| zoom | 194 / 15 | supported evaluator; declared camera/feature/composite/state capability required | [metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json](../../metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json#L1) |

Legacy filters have their own operator index: `!=` (330), `!has` (122), `!in` (404), `<` (24), `<=` (55), `==` (2919), `>` (27), `>=` (50), `all` (2166), `any` (74), `get` (1), `has` (72), `in` (1004), `none` (12). Raw fixtures include expected failures; their presence is not evidence that an operator compiles or evaluates in every use.

## Every observed metadata operation

| Operation | Occurrences / style files | Classification | Example |
| --- | --- | --- | --- |
| addCustomLayer | 4 / 4 | not applicable: separate custom-layer mechanism | [metrics/integration/render-tests/custom-layer-js/depth/style.json](../../metrics/integration/render-tests/custom-layer-js/depth/style.json#L1) |
| addImage | 76 / 34 | host operation retained; native plugin has no image/sampler consumption contract | [metrics/integration/render-tests/regressions/mapbox-gl-js#4928/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234928/style.json#L1) |
| addLayer | 36 / 33 | host lifecycle retained; plugin must be registered and source-backed | [metrics/integration/render-tests/regressions/mapbox-gl-js#2787/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%232787/style.json#L1) |
| addSource | 9 / 8 | host source operation retained; geometry sources usable by native plugin | [metrics/integration/render-tests/regressions/mapbox-gl-native#9900/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%239900/style.json#L1) |
| easeTo | 6 / 5 | host camera/LOD/padding behavior retained; uniform/query projections exposed | [metrics/integration/render-tests/debug/padding/ease-to-btm-distort/style.json](../../metrics/integration/render-tests/debug/padding/ease-to-btm-distort/style.json#L1) |
| idle | 3 / 3 | not applicable: harness scheduling/measurement | [metrics/integration/render-tests/text-variable-anchor-offset/pitched-offset/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/pitched-offset/style.json#L1) |
| pauseSource | 1 / 1 | host source operation retained; geometry sources usable by native plugin | [metrics/integration/render-tests/regressions/mapbox-gl-js#8817/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%238817/style.json#L1) |
| probeFileSize | 27 / 13 | not applicable: harness scheduling/measurement | [metrics/binary-size/android-arm64-v8a/style.json](../../metrics/binary-size/android-arm64-v8a/style.json#L1) |
| probeGFX | 12 / 10 | not applicable: harness scheduling/measurement | [metrics/tests/probes/gfx/fail-ib-mem-mismatch/style.json](../../metrics/tests/probes/gfx/fail-ib-mem-mismatch/style.json#L1) |
| probeGFXEnd | 11 / 10 | not applicable: harness scheduling/measurement | [metrics/tests/probes/gfx/fail-ib-mem-mismatch/style.json](../../metrics/tests/probes/gfx/fail-ib-mem-mismatch/style.json#L1) |
| probeGFXStart | 11 / 10 | not applicable: harness scheduling/measurement | [metrics/tests/probes/gfx/fail-ib-mem-mismatch/style.json](../../metrics/tests/probes/gfx/fail-ib-mem-mismatch/style.json#L1) |
| probeMemory | 9 / 3 | not applicable: harness scheduling/measurement | [metrics/tests/probes/memory/fail-memory-size-is-too-big/style.json](../../metrics/tests/probes/memory/fail-memory-size-is-too-big/style.json#L1) |
| probeMemoryEnd | 3 / 3 | not applicable: harness scheduling/measurement | [metrics/tests/probes/memory/fail-memory-size-is-too-big/style.json](../../metrics/tests/probes/memory/fail-memory-size-is-too-big/style.json#L1) |
| probeMemoryStart | 3 / 3 | not applicable: harness scheduling/measurement | [metrics/tests/probes/memory/fail-memory-size-is-too-big/style.json](../../metrics/tests/probes/memory/fail-memory-size-is-too-big/style.json#L1) |
| probeNetwork | 8 / 4 | not applicable: harness scheduling/measurement | [metrics/tests/probes/network/fail-requests-transferred/style.json](../../metrics/tests/probes/network/fail-requests-transferred/style.json#L1) |
| probeNetworkEnd | 4 / 4 | not applicable: harness scheduling/measurement | [metrics/tests/probes/network/fail-requests-transferred/style.json](../../metrics/tests/probes/network/fail-requests-transferred/style.json#L1) |
| probeNetworkStart | 4 / 4 | not applicable: harness scheduling/measurement | [metrics/tests/probes/network/fail-requests-transferred/style.json](../../metrics/tests/probes/network/fail-requests-transferred/style.json#L1) |
| querySourceFeatures | 1 / 1 | host source query retained; separate from plugin rendered-feature hit test | [metrics/integration/query-tests/regressions/mapbox-gl-js#6555/style.json](../../metrics/integration/query-tests/regressions/mapbox-gl-js%236555/style.json#L1) |
| removeFeatureState | 7 / 5 | supported substrate: host feature states and binder refresh; plugin tests only set | [metrics/integration/render-tests/regressions/mapbox-gl-js#8026/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%238026/style.json#L1) |
| removeImage | 4 / 4 | host operation retained; native plugin has no image/sampler consumption contract | [metrics/integration/render-tests/regressions/mapbox-gl-native#9976/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%239976/style.json#L1) |
| removeLayer | 11 / 11 | host lifecycle retained; plugin must be registered and source-backed | [metrics/integration/render-tests/regressions/mapbox-gl-js#2787/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%232787/style.json#L1) |
| removeSource | 1 / 1 | host source operation retained; geometry sources usable by native plugin | [metrics/integration/render-tests/regressions/mapbox-gl-native#9979/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%239979/style.json#L1) |
| setBearing | 9 / 9 | host camera/LOD/padding behavior retained; uniform/query projections exposed | [metrics/integration/render-tests/regressions/mapbox-gl-js#3365/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233365/style.json#L1) |
| setCenter | 23 / 20 | host camera/LOD/padding behavior retained; uniform/query projections exposed | [metrics/integration/render-tests/map-mode/tile-avoid-edges/style.json](../../metrics/integration/render-tests/map-mode/tile-avoid-edges/style.json#L1) |
| setFeatureState | 35 / 25 | supported substrate: host feature states and binder refresh; plugin tests only set | [metrics/integration/render-tests/feature-state/composite-expression/style.json](../../metrics/integration/render-tests/feature-state/composite-expression/style.json#L1) |
| setFilter | 9 / 9 | supported host common layer behavior | [metrics/integration/render-tests/regressions/mapbox-gl-native#5754/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%235754/style.json#L1) |
| setLayerZoomRange | 9 / 9 | supported host common layer behavior | [metrics/integration/render-tests/zoom-visibility/above/style.json](../../metrics/integration/render-tests/zoom-visibility/above/style.json#L1) |
| setLayoutProperty | 42 / 41 | partial: host visibility; plugin-defined layout properties rejected | [metrics/integration/render-tests/regressions/mapbox-gl-native#5701/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%235701/style.json#L1) |
| setLight | 1 / 1 | host builtin operation retained; no native-plugin lighting context | [metrics/integration/render-tests/regressions/mapbox-gl-js#5982/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235982/style.json#L1) |
| setPadding | 1 / 1 | host camera/LOD/padding behavior retained; uniform/query projections exposed | [metrics/integration/render-tests/debug/padding/set-padding/style.json](../../metrics/integration/render-tests/debug/padding/set-padding/style.json#L1) |
| setPaintProperty | 51 / 45 | supported substrate: registered paint conversion/binder updates | [metrics/integration/render-tests/background-color/transition/style.json](../../metrics/integration/render-tests/background-color/transition/style.json#L1) |
| setPitch | 1 / 1 | host camera/LOD/padding behavior retained; uniform/query projections exposed | [metrics/integration/render-tests/text-keep-upright/line-placement-true-roll-pitch-bearing/style.json](../../metrics/integration/render-tests/text-keep-upright/line-placement-true-roll-pitch-bearing/style.json#L1) |
| setRoll | 2 / 2 | host camera/LOD/padding behavior retained; uniform/query projections exposed | [metrics/integration/render-tests/text-keep-upright/line-placement-true-roll-pitch-bearing/style.json](../../metrics/integration/render-tests/text-keep-upright/line-placement-true-roll-pitch-bearing/style.json#L1) |
| setStyle | 99 / 99 | host lifecycle retained; plugin must be registered and source-backed | [metrics/integration/render-tests/basic-v9/z0-narrow-y/style.json](../../metrics/integration/render-tests/basic-v9/z0-narrow-y/style.json#L1) |
| setTileLodMinRadius | 1 / 1 | host camera/LOD/padding behavior retained; uniform/query projections exposed | [metrics/integration/render-tests/tile-lod/min-radius/style.json](../../metrics/integration/render-tests/tile-lod/min-radius/style.json#L1) |
| setTileLodMode | 3 / 3 | host camera/LOD/padding behavior retained; uniform/query projections exposed | [metrics/integration/render-tests/tile-lod/distance-based-pitch-threshold/style.json](../../metrics/integration/render-tests/tile-lod/distance-based-pitch-threshold/style.json#L1) |
| setTileLodPitchThreshold | 2 / 2 | host camera/LOD/padding behavior retained; uniform/query projections exposed | [metrics/integration/render-tests/tile-lod/distance-based-pitch-threshold/style.json](../../metrics/integration/render-tests/tile-lod/distance-based-pitch-threshold/style.json#L1) |
| setTileLodScale | 2 / 2 | host camera/LOD/padding behavior retained; uniform/query projections exposed | [metrics/integration/render-tests/tile-lod/distance-based-scale/style.json](../../metrics/integration/render-tests/tile-lod/distance-based-scale/style.json#L1) |
| setTileLodZoomShift | 1 / 1 | host camera/LOD/padding behavior retained; uniform/query projections exposed | [metrics/integration/render-tests/tile-lod/zoom-shift/style.json](../../metrics/integration/render-tests/tile-lod/zoom-shift/style.json#L1) |
| setZoom | 56 / 44 | host camera/LOD/padding behavior retained; uniform/query projections exposed | [metrics/integration/render-tests/line-dasharray/zoom-history/style.json](../../metrics/integration/render-tests/line-dasharray/zoom-history/style.json#L1) |
| sleep | 2 / 2 | not applicable: harness scheduling/measurement | [metrics/integration/render-tests/mixed-zoom/z10-z11/style.json](../../metrics/integration/render-tests/mixed-zoom/z10-z11/style.json#L1) |
| updateFakeCanvas | 1 / 1 | host operation retained; native plugin has no image/sampler consumption contract | [metrics/integration/render-tests/canvas/update/style.json](../../metrics/integration/render-tests/canvas/update/style.json#L1) |
| updateImage | 2 / 2 | host operation retained; native plugin has no image/sampler consumption contract | [metrics/integration/render-tests/runtime-styling/image-update-icon/style.json](../../metrics/integration/render-tests/runtime-styling/image-update-icon/style.json#L1) |
| wait | 415 / 302 | not applicable: harness scheduling/measurement | [metrics/integration/render-tests/background-color/transition/style.json](../../metrics/integration/render-tests/background-color/transition/style.json#L1) |

There are 99 `setStyle` operations: 75 inline objects are recursively inspected, 24 use string references. Six `local://styles/…` documents exist in the shared corpus; `local://mapbox-gl-styles/styles/{basic,bright,satellite}-v9.json` and `mapbox://styles/mapbox/streets-v11` are references without style bodies in this enumeration. Their remote/unavailable content is not claimed to have been inspected. Presence of a metadata operation also does not establish that every existing backend executes it; the native harness recognizes its supported operations in [render-test/parser.cpp](../../render-test/parser.cpp#L617).

## Query families

All 126 integration query style files have sibling expected JSON. The query runner chooses point or box query geometry and compares rendered features ([render-test/runner.cpp](../../render-test/runner.cpp#L903)). 129 total identified styles contain query metadata; the additional three are outside the integration query folder. The new ABI provides optional exact hit-testing, camera/projection inputs, evaluated paint values and conservative query radius ([include/mln/plugin/plugin_api.h](../../include/mln/plugin/plugin_api.h#L372)). This supplies a geometric query extension point; it does not reproduce symbol placement/collision selection, builtin pass-specific ordering or every existing feature-query edge case by itself.
| Query family | Styles | Classification | Example |
| --- | --- | --- | --- |
| circle-pitch-scale | 8 | supported substrate: plugin exact hit-test + radius, needs parity validation | [metrics/integration/query-tests/circle-pitch-scale/map-inside-align-map/style.json](../../metrics/integration/query-tests/circle-pitch-scale/map-inside-align-map/style.json#L1) |
| circle-radius | 7 | supported substrate: plugin exact hit-test + radius, needs parity validation | [metrics/integration/query-tests/circle-radius/feature-state/style.json](../../metrics/integration/query-tests/circle-radius/feature-state/style.json#L1) |
| circle-radius-features-in | 2 | supported substrate: plugin exact hit-test + radius, needs parity validation | [metrics/integration/query-tests/circle-radius-features-in/inside/style.json](../../metrics/integration/query-tests/circle-radius-features-in/inside/style.json#L1) |
| circle-stroke-width | 3 | supported substrate: plugin exact hit-test + radius, needs parity validation | [metrics/integration/query-tests/circle-stroke-width/feature-state/style.json](../../metrics/integration/query-tests/circle-stroke-width/feature-state/style.json#L1) |
| circle-translate | 3 | supported substrate: plugin exact hit-test + radius, needs parity validation | [metrics/integration/query-tests/circle-translate/box/style.json](../../metrics/integration/query-tests/circle-translate/box/style.json#L1) |
| circle-translate-anchor | 2 | supported substrate: plugin exact hit-test + radius, needs parity validation | [metrics/integration/query-tests/circle-translate-anchor/map/style.json](../../metrics/integration/query-tests/circle-translate-anchor/map/style.json#L1) |
| edge-cases | 4 | partial: host query lifecycle/options/geometry semantics; plugin-specific validation absent | [metrics/integration/query-tests/edge-cases/box-cutting-antimeridian-z0/style.json](../../metrics/integration/query-tests/edge-cases/box-cutting-antimeridian-z0/style.json#L1) |
| evaluated | 1 | supported substrate: host state/evaluation; absent from plugin query fixtures | [metrics/integration/query-tests/evaluated/line-width/style.json](../../metrics/integration/query-tests/evaluated/line-width/style.json#L1) |
| feature-state | 1 | supported substrate: host state/evaluation; absent from plugin query fixtures | [metrics/integration/query-tests/feature-state/default/style.json](../../metrics/integration/query-tests/feature-state/default/style.json#L1) |
| fill | 3 | partial: custom geometry hit test; rendering family limits | [metrics/integration/query-tests/fill/default/style.json](../../metrics/integration/query-tests/fill/default/style.json#L1) |
| fill-extrusion | 12 | partial: custom geometry hit test; rendering family limits | [metrics/integration/query-tests/fill-extrusion/base-in/style.json](../../metrics/integration/query-tests/fill-extrusion/base-in/style.json#L1) |
| fill-extrusion-translate | 1 | partial: custom geometry hit test; rendering family limits | [metrics/integration/query-tests/fill-extrusion-translate/multiple-layers/style.json](../../metrics/integration/query-tests/fill-extrusion-translate/multiple-layers/style.json#L1) |
| fill-features-in | 3 | partial: custom geometry hit test; rendering family limits | [metrics/integration/query-tests/fill-features-in/default/style.json](../../metrics/integration/query-tests/fill-features-in/default/style.json#L1) |
| fill-translate | 2 | partial: custom geometry hit test; rendering family limits | [metrics/integration/query-tests/fill-translate/literal/style.json](../../metrics/integration/query-tests/fill-translate/literal/style.json#L1) |
| fill-translate-anchor | 2 | partial: custom geometry hit test; rendering family limits | [metrics/integration/query-tests/fill-translate-anchor/map/style.json](../../metrics/integration/query-tests/fill-translate-anchor/map/style.json#L1) |
| geometry | 6 | partial: host query lifecycle/options/geometry semantics; plugin-specific validation absent | [metrics/integration/query-tests/geometry/linestring/style.json](../../metrics/integration/query-tests/geometry/linestring/style.json#L1) |
| invisible-features | 2 | partial: host query lifecycle/options/geometry semantics; plugin-specific validation absent | [metrics/integration/query-tests/invisible-features/visibility-none/style.json](../../metrics/integration/query-tests/invisible-features/visibility-none/style.json#L1) |
| line-gap-width | 6 | partial: custom geometry hit test; rendering family limits | [metrics/integration/query-tests/line-gap-width/feature-state/style.json](../../metrics/integration/query-tests/line-gap-width/feature-state/style.json#L1) |
| line-offset | 7 | partial: custom geometry hit test; rendering family limits | [metrics/integration/query-tests/line-offset/feature-state/style.json](../../metrics/integration/query-tests/line-offset/feature-state/style.json#L1) |
| line-translate | 2 | partial: custom geometry hit test; rendering family limits | [metrics/integration/query-tests/line-translate/inside/style.json](../../metrics/integration/query-tests/line-translate/inside/style.json#L1) |
| line-translate-anchor | 2 | partial: custom geometry hit test; rendering family limits | [metrics/integration/query-tests/line-translate-anchor/map/style.json](../../metrics/integration/query-tests/line-translate-anchor/map/style.json#L1) |
| line-width | 6 | partial: custom geometry hit test; rendering family limits | [metrics/integration/query-tests/line-width/feature-state/style.json](../../metrics/integration/query-tests/line-width/feature-state/style.json#L1) |
| line-width-features-in | 4 | partial: custom geometry hit test; rendering family limits | [metrics/integration/query-tests/line-width-features-in/inside/style.json](../../metrics/integration/query-tests/line-width-features-in/inside/style.json#L1) |
| options | 4 | partial: host query lifecycle/options/geometry semantics; plugin-specific validation absent | [metrics/integration/query-tests/options/filter-false/style.json](../../metrics/integration/query-tests/options/filter-false/style.json#L1) |
| regressions | 10 | partial: host query lifecycle/options/geometry semantics; plugin-specific validation absent | [metrics/integration/query-tests/regressions/mapbox-gl-js#3534/style.json](../../metrics/integration/query-tests/regressions/mapbox-gl-js%233534/style.json#L1) |
| remove-feature-state | 1 | supported substrate: host state/evaluation; absent from plugin query fixtures | [metrics/integration/query-tests/remove-feature-state/default/style.json](../../metrics/integration/query-tests/remove-feature-state/default/style.json#L1) |
| symbol | 12 | capability gap: symbol placement/collision semantics | [metrics/integration/query-tests/symbol/filtered-rotated-after-insert/style.json](../../metrics/integration/query-tests/symbol/filtered-rotated-after-insert/style.json#L1) |
| symbol-features-in | 7 | capability gap: symbol placement/collision semantics | [metrics/integration/query-tests/symbol-features-in/fractional-outside/style.json](../../metrics/integration/query-tests/symbol-features-in/fractional-outside/style.json#L1) |
| symbol-ignore-placement | 1 | capability gap: symbol placement/collision semantics | [metrics/integration/query-tests/symbol-ignore-placement/inside/style.json](../../metrics/integration/query-tests/symbol-ignore-placement/inside/style.json#L1) |
| world-wrapping | 2 | partial: host query lifecycle/options/geometry semantics; plugin-specific validation absent | [metrics/integration/query-tests/world-wrapping/box/style.json](../../metrics/integration/query-tests/world-wrapping/box/style.json#L1) |


## The 13 native ngon image fixtures

The manifest contains `base_test_path: "."` and empty expectation/ignore path lists ([plugins/ngon-layer/render-tests/manifest.json](../../plugins/ngon-layer/render-tests/manifest.json#L1)). Exactly 13 tracked `style.json` files each have a sibling `expected.png`; none has `queryGeometry`, `queryOptions` or sibling expected query JSON. Their assertion mechanism is the shared image comparison ([render-test/runner.cpp](../../render-test/runner.cpp#L903)), with operations applied before the captured result ([render-test/runner.cpp](../../render-test/runner.cpp#L935)). No explicit fixture-local numerical assertion is present. This inspection confirms fixture content/baselines, not that the suite ran or passed.
| Fixture | Assertion/content | Source |
| --- | --- | --- |
| all-properties-feature-driven | All 13 ngon paint properties use get; pitch 55°, bearing 25°. This includes numeric, color, FLOAT2 and enum properties. | [plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json](../../plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json#L1) |
| blur-and-opacity | Feature-driven blur and opacity with constant fill/stroke values. | [plugins/ngon-layer/render-tests/ngon/blur-and-opacity/style.json](../../plugins/ngon-layer/render-tests/ngon/blur-and-opacity/style.json#L1) |
| composite-fractional-zoom | Zoom 10.5; interpolate/zoom expressions affect radius and rotation through feature get and arithmetic. Corners uses get. | [plugins/ngon-layer/render-tests/ngon/composite-fractional-zoom/style.json](../../plugins/ngon-layer/render-tests/ngon/composite-fractional-zoom/style.json#L1) |
| corners-and-rotation | Feature-driven corners, rotation and color with constant radius/stroke. | [plugins/ngon-layer/render-tests/ngon/corners-and-rotation/style.json](../../plugins/ngon-layer/render-tests/ngon/corners-and-rotation/style.json#L1) |
| feature-state | wait; setFeatureState(source points, id 2, selected true); wait. case/boolean/feature-state affect radius, corners, rotation, color and translation. No removal/reset state operation. | [plugins/ngon-layer/render-tests/ngon/feature-state/style.json](../../plugins/ngon-layer/render-tests/ngon/feature-state/style.json#L1) |
| paint-update | wait; setPaintProperty points radius=32, corners=7, rotate=36, color=#16a34a; wait. One resulting baseline, no per-mutation intermediate assertion. | [plugins/ngon-layer/render-tests/ngon/paint-update/style.json](../../plugins/ngon-layer/render-tests/ngon/paint-update/style.json#L1) |
| pitch-map-map | Pitch 60°, bearing 35°; map alignment and map scale. | [plugins/ngon-layer/render-tests/ngon/pitch-map-map/style.json](../../plugins/ngon-layer/render-tests/ngon/pitch-map-map/style.json#L1) |
| pitch-map-viewport | Pitch 60°, bearing 35°; map alignment and viewport scale. | [plugins/ngon-layer/render-tests/ngon/pitch-map-viewport/style.json](../../plugins/ngon-layer/render-tests/ngon/pitch-map-viewport/style.json#L1) |
| pitch-viewport-map | Pitch 60°, bearing 35°; viewport alignment and map scale. | [plugins/ngon-layer/render-tests/ngon/pitch-viewport-map/style.json](../../plugins/ngon-layer/render-tests/ngon/pitch-viewport-map/style.json#L1) |
| pitch-viewport-viewport | Pitch 60°, bearing 35°; viewport alignment and viewport scale. | [plugins/ngon-layer/render-tests/ngon/pitch-viewport-viewport/style.json](../../plugins/ngon-layer/render-tests/ngon/pitch-viewport-viewport/style.json#L1) |
| tile-boundary-and-multipoint | Point/MultiPoint geometry near tile boundary, constant paints and opacity. | [plugins/ngon-layer/render-tests/ngon/tile-boundary-and-multipoint/style.json](../../plugins/ngon-layer/render-tests/ngon/tile-boundary-and-multipoint/style.json#L1) |
| translation-anchors | Bearing 90°; get drives translation, translate anchor and color. | [plugins/ngon-layer/render-tests/ngon/translation-anchors/style.json](../../plugins/ngon-layer/render-tests/ngon/translation-anchors/style.json#L1) |
| zero-radius | Constant radius zero image baseline. | [plugins/ngon-layer/render-tests/ngon/zero-radius/style.json](../../plugins/ngon-layer/render-tests/ngon/zero-radius/style.json#L1) |

Coverage absent from these 13 files: rendered/source query assertions; feature-state removal or reset; vector source/source-layer cases; line/polygon layout; text/icons/images/raster/DEM; custom registered layout properties; filter/add/remove-layer/source mutation; transition timing/intermediate frames; camera mutation operations; full expression operator/type coverage; explicit backend comparisons; shader/layout failures, registration/lifetime/threading tests. Other unit/platform tests may cover some of these; this statement is scoped to these 13 image fixtures. The four pitch combinations and one fractional-zoom case are present and must not be described as absent.

## All integration render family folders

Every one of the 175 family folders is listed below with its style count and layer-family association. Capability classification follows the layer-family matrix plus the exhaustive property/operation tables, so regressions/combinations retain their mixed-family nature.
| Family folder | Styles | Observed layer types | Example |
| --- | --- | --- | --- |
| background-color | 6 | background | [metrics/integration/render-tests/background-color/colorSpace-hcl/style.json](../../metrics/integration/render-tests/background-color/colorSpace-hcl/style.json#L1) |
| background-opacity | 3 | background, raster | [metrics/integration/render-tests/background-opacity/color/style.json](../../metrics/integration/render-tests/background-opacity/color/style.json#L1) |
| background-pattern | 6 | background | [metrics/integration/render-tests/background-pattern/@2x/style.json](../../metrics/integration/render-tests/background-pattern/@2x/style.json#L1) |
| background-visibility | 2 | background | [metrics/integration/render-tests/background-visibility/none/style.json](../../metrics/integration/render-tests/background-visibility/none/style.json#L1) |
| basic-v9 | 3 |  | [metrics/integration/render-tests/basic-v9/z0-narrow-y/style.json](../../metrics/integration/render-tests/basic-v9/z0-narrow-y/style.json#L1) |
| bright-v9 | 1 |  | [metrics/integration/render-tests/bright-v9/z0/style.json](../../metrics/integration/render-tests/bright-v9/z0/style.json#L1) |
| canvas | 2 | raster | [metrics/integration/render-tests/canvas/default/style.json](../../metrics/integration/render-tests/canvas/default/style.json#L1) |
| circle-blur | 7 | circle | [metrics/integration/render-tests/circle-blur/blending/style.json](../../metrics/integration/render-tests/circle-blur/blending/style.json#L1) |
| circle-color | 5 | circle | [metrics/integration/render-tests/circle-color/default/style.json](../../metrics/integration/render-tests/circle-color/default/style.json#L1) |
| circle-geometry | 6 | background, circle | [metrics/integration/render-tests/circle-geometry/linestring/style.json](../../metrics/integration/render-tests/circle-geometry/linestring/style.json#L1) |
| circle-opacity | 6 | circle | [metrics/integration/render-tests/circle-opacity/blending/style.json](../../metrics/integration/render-tests/circle-opacity/blending/style.json#L1) |
| circle-pitch-alignment | 4 | circle | [metrics/integration/render-tests/circle-pitch-alignment/map-scale-map/style.json](../../metrics/integration/render-tests/circle-pitch-alignment/map-scale-map/style.json#L1) |
| circle-pitch-scale | 3 | circle | [metrics/integration/render-tests/circle-pitch-scale/default/style.json](../../metrics/integration/render-tests/circle-pitch-scale/default/style.json#L1) |
| circle-radius | 6 | circle | [metrics/integration/render-tests/circle-radius/antimeridian/style.json](../../metrics/integration/render-tests/circle-radius/antimeridian/style.json#L1) |
| circle-sort-key | 1 | circle | [metrics/integration/render-tests/circle-sort-key/literal/style.json](../../metrics/integration/render-tests/circle-sort-key/literal/style.json#L1) |
| circle-stroke-color | 5 | circle | [metrics/integration/render-tests/circle-stroke-color/default/style.json](../../metrics/integration/render-tests/circle-stroke-color/default/style.json#L1) |
| circle-stroke-opacity | 6 | circle | [metrics/integration/render-tests/circle-stroke-opacity/default/style.json](../../metrics/integration/render-tests/circle-stroke-opacity/default/style.json#L1) |
| circle-stroke-width | 5 | circle | [metrics/integration/render-tests/circle-stroke-width/default/style.json](../../metrics/integration/render-tests/circle-stroke-width/default/style.json#L1) |
| circle-translate | 3 | circle | [metrics/integration/render-tests/circle-translate/default/style.json](../../metrics/integration/render-tests/circle-translate/default/style.json#L1) |
| circle-translate-anchor | 2 | circle | [metrics/integration/render-tests/circle-translate-anchor/map/style.json](../../metrics/integration/render-tests/circle-translate-anchor/map/style.json#L1) |
| collator | 2 | background, symbol | [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json#L1) |
| color-relief | 5 | background, color-relief, hillshade | [metrics/integration/render-tests/color-relief/hillshade/style.json](../../metrics/integration/render-tests/color-relief/hillshade/style.json#L1) |
| combinations | 127 | background, circle, color-relief, fill, fill-extrusion, heatmap, hillshade, line, raster, symbol | [metrics/integration/render-tests/combinations/background-opaque--background-opaque/style.json](../../metrics/integration/render-tests/combinations/background-opaque--background-opaque/style.json#L1) |
| custom-layer-js | 3 | background, fill, fill-extrusion | [metrics/integration/render-tests/custom-layer-js/depth/style.json](../../metrics/integration/render-tests/custom-layer-js/depth/style.json#L1) |
| debug | 19 | background, line, raster, symbol | [metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json](../../metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json#L1) |
| empty | 1 |  | [metrics/integration/render-tests/empty/empty/style.json](../../metrics/integration/render-tests/empty/empty/style.json#L1) |
| extent | 4 | background, circle, fill, line, symbol | [metrics/integration/render-tests/extent/1024-circle/style.json](../../metrics/integration/render-tests/extent/1024-circle/style.json#L1) |
| feature-state | 9 | background, circle, fill, fill-extrusion, line, symbol | [metrics/integration/render-tests/feature-state/composite-expression/style.json](../../metrics/integration/render-tests/feature-state/composite-expression/style.json#L1) |
| fill-antialias | 1 | fill | [metrics/integration/render-tests/fill-antialias/false/style.json](../../metrics/integration/render-tests/fill-antialias/false/style.json#L1) |
| fill-color | 7 | fill | [metrics/integration/render-tests/fill-color/default/style.json](../../metrics/integration/render-tests/fill-color/default/style.json#L1) |
| fill-extrusion-base | 6 | fill, fill-extrusion | [metrics/integration/render-tests/fill-extrusion-base/default/style.json](../../metrics/integration/render-tests/fill-extrusion-base/default/style.json#L1) |
| fill-extrusion-color | 6 | fill-extrusion | [metrics/integration/render-tests/fill-extrusion-color/default/style.json](../../metrics/integration/render-tests/fill-extrusion-color/default/style.json#L1) |
| fill-extrusion-geometry | 1 | fill-extrusion | [metrics/integration/render-tests/fill-extrusion-geometry/linestring/style.json](../../metrics/integration/render-tests/fill-extrusion-geometry/linestring/style.json#L1) |
| fill-extrusion-height | 5 | fill-extrusion | [metrics/integration/render-tests/fill-extrusion-height/default/style.json](../../metrics/integration/render-tests/fill-extrusion-height/default/style.json#L1) |
| fill-extrusion-multiple | 2 | circle, fill-extrusion | [metrics/integration/render-tests/fill-extrusion-multiple/interleaved-layers/style.json](../../metrics/integration/render-tests/fill-extrusion-multiple/interleaved-layers/style.json#L1) |
| fill-extrusion-opacity | 3 | fill, fill-extrusion | [metrics/integration/render-tests/fill-extrusion-opacity/default/style.json](../../metrics/integration/render-tests/fill-extrusion-opacity/default/style.json#L1) |
| fill-extrusion-pattern | 8 | fill-extrusion | [metrics/integration/render-tests/fill-extrusion-pattern/@2x/style.json](../../metrics/integration/render-tests/fill-extrusion-pattern/@2x/style.json#L1) |
| fill-extrusion-translate | 4 | fill, fill-extrusion | [metrics/integration/render-tests/fill-extrusion-translate/default/style.json](../../metrics/integration/render-tests/fill-extrusion-translate/default/style.json#L1) |
| fill-extrusion-translate-anchor | 2 | fill, fill-extrusion | [metrics/integration/render-tests/fill-extrusion-translate-anchor/map/style.json](../../metrics/integration/render-tests/fill-extrusion-translate-anchor/map/style.json#L1) |
| fill-extrusion-vertical-gradient | 2 | fill-extrusion | [metrics/integration/render-tests/fill-extrusion-vertical-gradient/default/style.json](../../metrics/integration/render-tests/fill-extrusion-vertical-gradient/default/style.json#L1) |
| fill-opacity | 9 | background, fill, symbol | [metrics/integration/render-tests/fill-opacity/default/style.json](../../metrics/integration/render-tests/fill-opacity/default/style.json#L1) |
| fill-outline-color | 8 | fill | [metrics/integration/render-tests/fill-outline-color/default/style.json](../../metrics/integration/render-tests/fill-outline-color/default/style.json#L1) |
| fill-pattern | 10 | background, fill | [metrics/integration/render-tests/fill-pattern/@2x/style.json](../../metrics/integration/render-tests/fill-pattern/@2x/style.json#L1) |
| fill-sort-key | 1 | fill | [metrics/integration/render-tests/fill-sort-key/literal/style.json](../../metrics/integration/render-tests/fill-sort-key/literal/style.json#L1) |
| fill-translate | 3 | fill | [metrics/integration/render-tests/fill-translate/default/style.json](../../metrics/integration/render-tests/fill-translate/default/style.json#L1) |
| fill-translate-anchor | 2 | fill | [metrics/integration/render-tests/fill-translate-anchor/map/style.json](../../metrics/integration/render-tests/fill-translate-anchor/map/style.json#L1) |
| fill-visibility | 2 | background, fill | [metrics/integration/render-tests/fill-visibility/none/style.json](../../metrics/integration/render-tests/fill-visibility/none/style.json#L1) |
| filter | 4 | circle, fill | [metrics/integration/render-tests/filter/equality/style.json](../../metrics/integration/render-tests/filter/equality/style.json#L1) |
| geojson | 24 | circle, fill, line, symbol | [metrics/integration/render-tests/geojson/clustered-properties/style.json](../../metrics/integration/render-tests/geojson/clustered-properties/style.json#L1) |
| heatmap-color | 2 | background, heatmap | [metrics/integration/render-tests/heatmap-color/default/style.json](../../metrics/integration/render-tests/heatmap-color/default/style.json#L1) |
| heatmap-intensity | 3 | background, heatmap | [metrics/integration/render-tests/heatmap-intensity/default/style.json](../../metrics/integration/render-tests/heatmap-intensity/default/style.json#L1) |
| heatmap-opacity | 3 | background, heatmap | [metrics/integration/render-tests/heatmap-opacity/default/style.json](../../metrics/integration/render-tests/heatmap-opacity/default/style.json#L1) |
| heatmap-radius | 6 | background, heatmap | [metrics/integration/render-tests/heatmap-radius/antimeridian/style.json](../../metrics/integration/render-tests/heatmap-radius/antimeridian/style.json#L1) |
| heatmap-weight | 3 | background, heatmap | [metrics/integration/render-tests/heatmap-weight/default/style.json](../../metrics/integration/render-tests/heatmap-weight/default/style.json#L1) |
| hillshade-accent-color | 4 | background, hillshade | [metrics/integration/render-tests/hillshade-accent-color/default/style.json](../../metrics/integration/render-tests/hillshade-accent-color/default/style.json#L1) |
| hillshade-alt | 3 | background, hillshade | [metrics/integration/render-tests/hillshade-alt/basic-0/style.json](../../metrics/integration/render-tests/hillshade-alt/basic-0/style.json#L1) |
| hillshade-highlight-color | 3 | background, hillshade | [metrics/integration/render-tests/hillshade-highlight-color/default/style.json](../../metrics/integration/render-tests/hillshade-highlight-color/default/style.json#L1) |
| hillshade-low-zoom | 6 | background, hillshade | [metrics/integration/render-tests/hillshade-low-zoom/basic/style.json](../../metrics/integration/render-tests/hillshade-low-zoom/basic/style.json#L1) |
| hillshade-maxzoom | 2 | background, hillshade | [metrics/integration/render-tests/hillshade-maxzoom/default/style.json](../../metrics/integration/render-tests/hillshade-maxzoom/default/style.json#L1) |
| hillshade-methods | 4 | background, hillshade | [metrics/integration/render-tests/hillshade-methods/basic/style.json](../../metrics/integration/render-tests/hillshade-methods/basic/style.json#L1) |
| hillshade-shadow-color | 3 | background, hillshade | [metrics/integration/render-tests/hillshade-shadow-color/default/style.json](../../metrics/integration/render-tests/hillshade-shadow-color/default/style.json#L1) |
| icon-anchor | 11 | symbol | [metrics/integration/render-tests/icon-anchor/bottom-left/style.json](../../metrics/integration/render-tests/icon-anchor/bottom-left/style.json#L1) |
| icon-color | 4 | symbol | [metrics/integration/render-tests/icon-color/default/style.json](../../metrics/integration/render-tests/icon-color/default/style.json#L1) |
| icon-halo-blur | 4 | symbol | [metrics/integration/render-tests/icon-halo-blur/default/style.json](../../metrics/integration/render-tests/icon-halo-blur/default/style.json#L1) |
| icon-halo-color | 7 | symbol | [metrics/integration/render-tests/icon-halo-color/default/style.json](../../metrics/integration/render-tests/icon-halo-color/default/style.json#L1) |
| icon-halo-width | 4 | symbol | [metrics/integration/render-tests/icon-halo-width/default/style.json](../../metrics/integration/render-tests/icon-halo-width/default/style.json#L1) |
| icon-image | 7 | background, circle, symbol | [metrics/integration/render-tests/icon-image/icon-sdf-non-sdf-one-layer/style.json](../../metrics/integration/render-tests/icon-image/icon-sdf-non-sdf-one-layer/style.json#L1) |
| icon-no-cross-source-collision | 1 | symbol | [metrics/integration/render-tests/icon-no-cross-source-collision/default/style.json](../../metrics/integration/render-tests/icon-no-cross-source-collision/default/style.json#L1) |
| icon-offset | 4 | background, line, symbol | [metrics/integration/render-tests/icon-offset/literal/style.json](../../metrics/integration/render-tests/icon-offset/literal/style.json#L1) |
| icon-opacity | 7 | background, symbol | [metrics/integration/render-tests/icon-opacity/default/style.json](../../metrics/integration/render-tests/icon-opacity/default/style.json#L1) |
| icon-padding | 2 | background, symbol | [metrics/integration/render-tests/icon-padding/databind/style.json](../../metrics/integration/render-tests/icon-padding/databind/style.json#L1) |
| icon-pitch-alignment | 4 | symbol | [metrics/integration/render-tests/icon-pitch-alignment/auto-rotation-alignment-map/style.json](../../metrics/integration/render-tests/icon-pitch-alignment/auto-rotation-alignment-map/style.json#L1) |
| icon-pitch-scaling | 2 | background, symbol | [metrics/integration/render-tests/icon-pitch-scaling/rotation-alignment-map/style.json](../../metrics/integration/render-tests/icon-pitch-scaling/rotation-alignment-map/style.json#L1) |
| icon-pixelratio-mismatch | 1 | background, symbol | [metrics/integration/render-tests/icon-pixelratio-mismatch/default/style.json](../../metrics/integration/render-tests/icon-pixelratio-mismatch/default/style.json#L1) |
| icon-roll-alignment | 4 | symbol | [metrics/integration/render-tests/icon-roll-alignment/auto-rotation-alignment-map/style.json](../../metrics/integration/render-tests/icon-roll-alignment/auto-rotation-alignment-map/style.json#L1) |
| icon-rotate | 3 | background, symbol | [metrics/integration/render-tests/icon-rotate/literal/style.json](../../metrics/integration/render-tests/icon-rotate/literal/style.json#L1) |
| icon-rotation-alignment | 6 | symbol | [metrics/integration/render-tests/icon-rotation-alignment/auto-symbol-placement-line/style.json](../../metrics/integration/render-tests/icon-rotation-alignment/auto-symbol-placement-line/style.json#L1) |
| icon-size | 11 | symbol | [metrics/integration/render-tests/icon-size/camera-function-high-base-plain/style.json](../../metrics/integration/render-tests/icon-size/camera-function-high-base-plain/style.json#L1) |
| icon-text-fit | 45 | background, circle, symbol | [metrics/integration/render-tests/icon-text-fit/both-collision-variable-anchor-text-fit/style.json](../../metrics/integration/render-tests/icon-text-fit/both-collision-variable-anchor-text-fit/style.json#L1) |
| icon-translate | 3 | background, symbol | [metrics/integration/render-tests/icon-translate/default/style.json](../../metrics/integration/render-tests/icon-translate/default/style.json#L1) |
| icon-translate-anchor | 2 | background, symbol | [metrics/integration/render-tests/icon-translate-anchor/map/style.json](../../metrics/integration/render-tests/icon-translate-anchor/map/style.json#L1) |
| icon-visibility | 2 | background, symbol | [metrics/integration/render-tests/icon-visibility/none/style.json](../../metrics/integration/render-tests/icon-visibility/none/style.json#L1) |
| image | 10 | background, raster | [metrics/integration/render-tests/image/default/style.json](../../metrics/integration/render-tests/image/default/style.json#L1) |
| is-supported-script | 2 | background, symbol | [metrics/integration/render-tests/is-supported-script/filter/style.json](../../metrics/integration/render-tests/is-supported-script/filter/style.json#L1) |
| line-blur | 4 | background, line | [metrics/integration/render-tests/line-blur/default/style.json](../../metrics/integration/render-tests/line-blur/default/style.json#L1) |
| line-cap | 3 | background, line | [metrics/integration/render-tests/line-cap/butt/style.json](../../metrics/integration/render-tests/line-cap/butt/style.json#L1) |
| line-color | 5 | background, line | [metrics/integration/render-tests/line-color/default/style.json](../../metrics/integration/render-tests/line-color/default/style.json#L1) |
| line-dasharray | 16 | background, line | [metrics/integration/render-tests/line-dasharray/default/style.json](../../metrics/integration/render-tests/line-dasharray/default/style.json#L1) |
| line-gap-width | 4 | background, line | [metrics/integration/render-tests/line-gap-width/default/style.json](../../metrics/integration/render-tests/line-gap-width/default/style.json#L1) |
| line-gradient | 3 | line | [metrics/integration/render-tests/line-gradient/gradient-tile-boundaries/style.json](../../metrics/integration/render-tests/line-gradient/gradient-tile-boundaries/style.json#L1) |
| line-join | 9 | line | [metrics/integration/render-tests/line-join/bevel-transparent/style.json](../../metrics/integration/render-tests/line-join/bevel-transparent/style.json#L1) |
| line-offset | 5 | background, line | [metrics/integration/render-tests/line-offset/default/style.json](../../metrics/integration/render-tests/line-offset/default/style.json#L1) |
| line-opacity | 5 | background, line | [metrics/integration/render-tests/line-opacity/default/style.json](../../metrics/integration/render-tests/line-opacity/default/style.json#L1) |
| line-pattern | 10 | background, line | [metrics/integration/render-tests/line-pattern/@2x/style.json](../../metrics/integration/render-tests/line-pattern/@2x/style.json#L1) |
| line-pitch | 5 | background, line | [metrics/integration/render-tests/line-pitch/default/style.json](../../metrics/integration/render-tests/line-pitch/default/style.json#L1) |
| line-sort-key | 1 | line | [metrics/integration/render-tests/line-sort-key/literal/style.json](../../metrics/integration/render-tests/line-sort-key/literal/style.json#L1) |
| line-translate | 3 | background, line | [metrics/integration/render-tests/line-translate/default/style.json](../../metrics/integration/render-tests/line-translate/default/style.json#L1) |
| line-translate-anchor | 2 | background, line | [metrics/integration/render-tests/line-translate-anchor/map/style.json](../../metrics/integration/render-tests/line-translate-anchor/map/style.json#L1) |
| line-triangulation | 2 | background, line | [metrics/integration/render-tests/line-triangulation/default/style.json](../../metrics/integration/render-tests/line-triangulation/default/style.json#L1) |
| line-visibility | 2 | background, line | [metrics/integration/render-tests/line-visibility/none/style.json](../../metrics/integration/render-tests/line-visibility/none/style.json#L1) |
| line-width | 7 | background, line | [metrics/integration/render-tests/line-width/default/style.json](../../metrics/integration/render-tests/line-width/default/style.json#L1) |
| linear-filter-opacity-edge | 1 | background, symbol | [metrics/integration/render-tests/linear-filter-opacity-edge/literal/style.json](../../metrics/integration/render-tests/linear-filter-opacity-edge/literal/style.json#L1) |
| map-mode | 3 | background, symbol | [metrics/integration/render-tests/map-mode/static/style.json](../../metrics/integration/render-tests/map-mode/static/style.json#L1) |
| mixed-zoom | 1 |  | [metrics/integration/render-tests/mixed-zoom/z10-z11/style.json](../../metrics/integration/render-tests/mixed-zoom/z10-z11/style.json#L1) |
| projection | 4 | fill-extrusion | [metrics/integration/render-tests/projection/axonometric-multiple/style.json](../../metrics/integration/render-tests/projection/axonometric-multiple/style.json#L1) |
| raster-alpha | 1 | background, raster | [metrics/integration/render-tests/raster-alpha/default/style.json](../../metrics/integration/render-tests/raster-alpha/default/style.json#L1) |
| raster-brightness | 3 | raster | [metrics/integration/render-tests/raster-brightness/default/style.json](../../metrics/integration/render-tests/raster-brightness/default/style.json#L1) |
| raster-contrast | 3 | raster | [metrics/integration/render-tests/raster-contrast/default/style.json](../../metrics/integration/render-tests/raster-contrast/default/style.json#L1) |
| raster-extent | 2 | background, raster | [metrics/integration/render-tests/raster-extent/maxzoom/style.json](../../metrics/integration/render-tests/raster-extent/maxzoom/style.json#L1) |
| raster-hue-rotate | 3 | raster | [metrics/integration/render-tests/raster-hue-rotate/default/style.json](../../metrics/integration/render-tests/raster-hue-rotate/default/style.json#L1) |
| raster-loading | 1 | raster | [metrics/integration/render-tests/raster-loading/missing/style.json](../../metrics/integration/render-tests/raster-loading/missing/style.json#L1) |
| raster-masking | 3 | background, fill, raster | [metrics/integration/render-tests/raster-masking/overlapping-vector/style.json](../../metrics/integration/render-tests/raster-masking/overlapping-vector/style.json#L1) |
| raster-opacity | 3 | raster | [metrics/integration/render-tests/raster-opacity/default/style.json](../../metrics/integration/render-tests/raster-opacity/default/style.json#L1) |
| raster-resampling | 3 | raster | [metrics/integration/render-tests/raster-resampling/default/style.json](../../metrics/integration/render-tests/raster-resampling/default/style.json#L1) |
| raster-rotation | 5 | raster | [metrics/integration/render-tests/raster-rotation/0/style.json](../../metrics/integration/render-tests/raster-rotation/0/style.json#L1) |
| raster-saturation | 3 | raster | [metrics/integration/render-tests/raster-saturation/default/style.json](../../metrics/integration/render-tests/raster-saturation/default/style.json#L1) |
| raster-visibility | 2 | background, raster | [metrics/integration/render-tests/raster-visibility/none/style.json](../../metrics/integration/render-tests/raster-visibility/none/style.json#L1) |
| real-world | 6 |  | [metrics/integration/render-tests/real-world/bangkok/style.json](../../metrics/integration/render-tests/real-world/bangkok/style.json#L1) |
| regressions | 114 | background, circle, fill, fill-extrusion, heatmap, line, raster, symbol | [metrics/integration/render-tests/regressions/mapbox-gl-js#2305/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%232305/style.json#L1) |
| remove-feature-state | 3 | background, circle | [metrics/integration/render-tests/remove-feature-state/composite-expression/style.json](../../metrics/integration/render-tests/remove-feature-state/composite-expression/style.json#L1) |
| retina-raster | 1 | raster | [metrics/integration/render-tests/retina-raster/default/style.json](../../metrics/integration/render-tests/retina-raster/default/style.json#L1) |
| runtime-styling | 179 | background, circle, fill, fill-extrusion, hillshade, line, raster, symbol | [metrics/integration/render-tests/runtime-styling/filter-default-to-false/style.json](../../metrics/integration/render-tests/runtime-styling/filter-default-to-false/style.json#L1) |
| satellite-v9 | 1 |  | [metrics/integration/render-tests/satellite-v9/z0/style.json](../../metrics/integration/render-tests/satellite-v9/z0/style.json#L1) |
| sparse-tileset | 1 | background, fill | [metrics/integration/render-tests/sparse-tileset/overdraw/style.json](../../metrics/integration/render-tests/sparse-tileset/overdraw/style.json#L1) |
| sprites | 10 | background, symbol | [metrics/integration/render-tests/sprites/1x-screen-1x-icon/style.json](../../metrics/integration/render-tests/sprites/1x-screen-1x-icon/style.json#L1) |
| symbol-cross-fade | 1 | background, line, symbol | [metrics/integration/render-tests/symbol-cross-fade/chinese/style.json](../../metrics/integration/render-tests/symbol-cross-fade/chinese/style.json#L1) |
| symbol-geometry | 6 | background, symbol | [metrics/integration/render-tests/symbol-geometry/linestring/style.json](../../metrics/integration/render-tests/symbol-geometry/linestring/style.json#L1) |
| symbol-placement | 8 | background, line, symbol | [metrics/integration/render-tests/symbol-placement/line-center-buffer-tile-map-mode/style.json](../../metrics/integration/render-tests/symbol-placement/line-center-buffer-tile-map-mode/style.json#L1) |
| symbol-sort-key | 6 | symbol | [metrics/integration/render-tests/symbol-sort-key/icon-expression/style.json](../../metrics/integration/render-tests/symbol-sort-key/icon-expression/style.json#L1) |
| symbol-spacing | 5 | background, symbol | [metrics/integration/render-tests/symbol-spacing/line-close/style.json](../../metrics/integration/render-tests/symbol-spacing/line-close/style.json#L1) |
| symbol-visibility | 2 | background, symbol | [metrics/integration/render-tests/symbol-visibility/none/style.json](../../metrics/integration/render-tests/symbol-visibility/none/style.json#L1) |
| symbol-z-order | 5 | symbol | [metrics/integration/render-tests/symbol-z-order/default/style.json](../../metrics/integration/render-tests/symbol-z-order/default/style.json#L1) |
| text-anchor | 10 | background, symbol | [metrics/integration/render-tests/text-anchor/bottom-left/style.json](../../metrics/integration/render-tests/text-anchor/bottom-left/style.json#L1) |
| text-arabic | 5 | symbol | [metrics/integration/render-tests/text-arabic/letter-spacing/style.json](../../metrics/integration/render-tests/text-arabic/letter-spacing/style.json#L1) |
| text-breaking | 2 | symbol | [metrics/integration/render-tests/text-breaking/left-parenthesis/style.json](../../metrics/integration/render-tests/text-breaking/left-parenthesis/style.json#L1) |
| text-color | 7 | background, symbol | [metrics/integration/render-tests/text-color/default/style.json](../../metrics/integration/render-tests/text-color/default/style.json#L1) |
| text-field | 17 | background, line, symbol | [metrics/integration/render-tests/text-field/formatted-arabic/style.json](../../metrics/integration/render-tests/text-field/formatted-arabic/style.json#L1) |
| text-font | 7 | background, symbol | [metrics/integration/render-tests/text-font/burmese/style.json](../../metrics/integration/render-tests/text-font/burmese/style.json#L1) |
| text-halo-blur | 4 | symbol | [metrics/integration/render-tests/text-halo-blur/default/style.json](../../metrics/integration/render-tests/text-halo-blur/default/style.json#L1) |
| text-halo-color | 4 | symbol | [metrics/integration/render-tests/text-halo-color/default/style.json](../../metrics/integration/render-tests/text-halo-color/default/style.json#L1) |
| text-halo-width | 4 | symbol | [metrics/integration/render-tests/text-halo-width/default/style.json](../../metrics/integration/render-tests/text-halo-width/default/style.json#L1) |
| text-justify | 4 | background, symbol | [metrics/integration/render-tests/text-justify/auto/style.json](../../metrics/integration/render-tests/text-justify/auto/style.json#L1) |
| text-keep-upright | 12 | background, line, symbol | [metrics/integration/render-tests/text-keep-upright/line-placement-false/style.json](../../metrics/integration/render-tests/text-keep-upright/line-placement-false/style.json#L1) |
| text-letter-spacing | 5 | background, symbol | [metrics/integration/render-tests/text-letter-spacing/function-close/style.json](../../metrics/integration/render-tests/text-letter-spacing/function-close/style.json#L1) |
| text-line-height | 1 | background, symbol | [metrics/integration/render-tests/text-line-height/literal/style.json](../../metrics/integration/render-tests/text-line-height/literal/style.json#L1) |
| text-max-angle | 2 | background, symbol | [metrics/integration/render-tests/text-max-angle/line-center/style.json](../../metrics/integration/render-tests/text-max-angle/line-center/style.json#L1) |
| text-max-width | 12 | background, symbol | [metrics/integration/render-tests/text-max-width/force-double-newline/style.json](../../metrics/integration/render-tests/text-max-width/force-double-newline/style.json#L1) |
| text-no-cross-source-collision | 1 | symbol | [metrics/integration/render-tests/text-no-cross-source-collision/default/style.json](../../metrics/integration/render-tests/text-no-cross-source-collision/default/style.json#L1) |
| text-offset | 21 | background, symbol | [metrics/integration/render-tests/text-offset/literal-multiline-anchorcenter-justifycenter-offsetnegative/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorcenter-justifycenter-offsetnegative/style.json#L1) |
| text-opacity | 4 | background, symbol | [metrics/integration/render-tests/text-opacity/default/style.json](../../metrics/integration/render-tests/text-opacity/default/style.json#L1) |
| text-pitch-alignment | 10 | background, fill, line, symbol | [metrics/integration/render-tests/text-pitch-alignment/auto-text-rotation-alignment-map/style.json](../../metrics/integration/render-tests/text-pitch-alignment/auto-text-rotation-alignment-map/style.json#L1) |
| text-pitch-scaling | 2 | background, line, symbol | [metrics/integration/render-tests/text-pitch-scaling/line-half-roll/style.json](../../metrics/integration/render-tests/text-pitch-scaling/line-half-roll/style.json#L1) |
| text-radial-offset | 1 | background, circle, symbol | [metrics/integration/render-tests/text-radial-offset/basic/style.json](../../metrics/integration/render-tests/text-radial-offset/basic/style.json#L1) |
| text-roll-alignment | 6 | symbol | [metrics/integration/render-tests/text-roll-alignment/auto-symbol-placement-line/style.json](../../metrics/integration/render-tests/text-roll-alignment/auto-symbol-placement-line/style.json#L1) |
| text-rotate | 8 | background, symbol | [metrics/integration/render-tests/text-rotate/anchor-bottom/style.json](../../metrics/integration/render-tests/text-rotate/anchor-bottom/style.json#L1) |
| text-rotation-alignment | 6 | symbol | [metrics/integration/render-tests/text-rotation-alignment/auto-symbol-placement-line/style.json](../../metrics/integration/render-tests/text-rotation-alignment/auto-symbol-placement-line/style.json#L1) |
| text-size | 11 | symbol | [metrics/integration/render-tests/text-size/camera-function-high-base/style.json](../../metrics/integration/render-tests/text-size/camera-function-high-base/style.json#L1) |
| text-tile-edge-clipping | 1 | background, symbol | [metrics/integration/render-tests/text-tile-edge-clipping/default/style.json](../../metrics/integration/render-tests/text-tile-edge-clipping/default/style.json#L1) |
| text-transform | 3 | background, symbol | [metrics/integration/render-tests/text-transform/lowercase/style.json](../../metrics/integration/render-tests/text-transform/lowercase/style.json#L1) |
| text-translate | 3 | background, symbol | [metrics/integration/render-tests/text-translate/default/style.json](../../metrics/integration/render-tests/text-translate/default/style.json#L1) |
| text-translate-anchor | 2 | background, symbol | [metrics/integration/render-tests/text-translate-anchor/map/style.json](../../metrics/integration/render-tests/text-translate-anchor/map/style.json#L1) |
| text-variable-anchor | 27 | background, circle, symbol | [metrics/integration/render-tests/text-variable-anchor/all-anchors-icon-text-fit/style.json](../../metrics/integration/render-tests/text-variable-anchor/all-anchors-icon-text-fit/style.json#L1) |
| text-variable-anchor-offset | 20 | background, circle, symbol | [metrics/integration/render-tests/text-variable-anchor-offset/all-anchors-icon-text-fit/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/all-anchors-icon-text-fit/style.json#L1) |
| text-visibility | 2 | background, symbol | [metrics/integration/render-tests/text-visibility/none/style.json](../../metrics/integration/render-tests/text-visibility/none/style.json#L1) |
| text-writing-mode | 18 | background, line, symbol | [metrics/integration/render-tests/text-writing-mode/line_label/chinese-punctuation/style.json](../../metrics/integration/render-tests/text-writing-mode/line_label/chinese-punctuation/style.json#L1) |
| tile-lod | 8 | raster | [metrics/integration/render-tests/tile-lod/default/style.json](../../metrics/integration/render-tests/tile-lod/default/style.json#L1) |
| tile-mode | 1 |  | [metrics/integration/render-tests/tile-mode/streets-v11/style.json](../../metrics/integration/render-tests/tile-mode/streets-v11/style.json#L1) |
| tilejson-bounds | 2 | background, line | [metrics/integration/render-tests/tilejson-bounds/default/style.json](../../metrics/integration/render-tests/tilejson-bounds/default/style.json#L1) |
| tms | 1 | background, fill | [metrics/integration/render-tests/tms/tms/style.json](../../metrics/integration/render-tests/tms/tms/style.json#L1) |
| video | 1 | raster | [metrics/integration/render-tests/video/default/style.json](../../metrics/integration/render-tests/video/default/style.json#L1) |
| within | 6 | circle, fill, line, symbol | [metrics/integration/render-tests/within/filter-with-inlined-geojson/style.json](../../metrics/integration/render-tests/within/filter-with-inlined-geojson/style.json#L1) |
| zoom-history | 2 | line | [metrics/integration/render-tests/zoom-history/in/style.json](../../metrics/integration/render-tests/zoom-history/in/style.json#L1) |
| zoom-visibility | 6 | circle | [metrics/integration/render-tests/zoom-visibility/above/style.json](../../metrics/integration/render-tests/zoom-visibility/above/style.json#L1) |
| zoomed-fill | 2 | background, fill | [metrics/integration/render-tests/zoomed-fill/default/style.json](../../metrics/integration/render-tests/zoomed-fill/default/style.json#L1) |
| zoomed-raster | 3 | raster | [metrics/integration/render-tests/zoomed-raster/fractional/style.json](../../metrics/integration/render-tests/zoomed-raster/fractional/style.json#L1) |


## Parser negatives, errors and exclusions

All 30 style-parser fixture documents are retained in counts. 25 have companion expected diagnostic logs; four have explicit empty diagnostic logs and `font_stacks.json` has no companion info file. The empty-string style `non-object.style.json` is valid JSON but an intentionally invalid style, so it is not misreported as a JSON parsing failure.
| Parser fixture | Expected diagnostic entries | Evidence |
| --- | --- | --- |
| center-not-latlong.style.json | 1 | [test/fixtures/style_parser/center-not-latlong.style.json](../../test/fixtures/style_parser/center-not-latlong.style.json#L1) |
| circle-blur.style.json | 1 | [test/fixtures/style_parser/circle-blur.style.json](../../test/fixtures/style_parser/circle-blur.style.json#L1) |
| circle-color.style.json | 1 | [test/fixtures/style_parser/circle-color.style.json](../../test/fixtures/style_parser/circle-color.style.json#L1) |
| circle-opacity.style.json | 1 | [test/fixtures/style_parser/circle-opacity.style.json](../../test/fixtures/style_parser/circle-opacity.style.json#L1) |
| circle-radius.style.json | 1 | [test/fixtures/style_parser/circle-radius.style.json](../../test/fixtures/style_parser/circle-radius.style.json#L1) |
| colors.style.json | 1 | [test/fixtures/style_parser/colors.style.json](../../test/fixtures/style_parser/colors.style.json#L1) |
| expressions.style.json | 6 | [test/fixtures/style_parser/expressions.style.json](../../test/fixtures/style_parser/expressions.style.json#L1) |
| font_stacks.json | 0 | [test/fixtures/style_parser/font_stacks.json](../../test/fixtures/style_parser/font_stacks.json#L1) |
| function-numeric.style.json | 1 | [test/fixtures/style_parser/function-numeric.style.json](../../test/fixtures/style_parser/function-numeric.style.json#L1) |
| function-string-bool-enum.style.json | 0 | [test/fixtures/style_parser/function-string-bool-enum.style.json](../../test/fixtures/style_parser/function-string-bool-enum.style.json#L1) |
| function-type.style.json | 1 | [test/fixtures/style_parser/function-type.style.json](../../test/fixtures/style_parser/function-type.style.json#L1) |
| geojson-data-inline.style.json | 0 | [test/fixtures/style_parser/geojson-data-inline.style.json](../../test/fixtures/style_parser/geojson-data-inline.style.json#L1) |
| geojson-data-url.style.json | 0 | [test/fixtures/style_parser/geojson-data-url.style.json](../../test/fixtures/style_parser/geojson-data-url.style.json#L1) |
| geojson-invalid-data.style.json | 1 | [test/fixtures/style_parser/geojson-invalid-data.style.json](../../test/fixtures/style_parser/geojson-invalid-data.style.json#L1) |
| geojson-missing-data.style.json | 1 | [test/fixtures/style_parser/geojson-missing-data.style.json](../../test/fixtures/style_parser/geojson-missing-data.style.json#L1) |
| geojson-missing-properties.style.json | 0 | [test/fixtures/style_parser/geojson-missing-properties.style.json](../../test/fixtures/style_parser/geojson-missing-properties.style.json#L1) |
| image-coordinates.style.json | 1 | [test/fixtures/style_parser/image-coordinates.style.json](../../test/fixtures/style_parser/image-coordinates.style.json#L1) |
| image-url.style.json | 1 | [test/fixtures/style_parser/image-url.style.json](../../test/fixtures/style_parser/image-url.style.json#L1) |
| line-opacity.style.json | 1 | [test/fixtures/style_parser/line-opacity.style.json](../../test/fixtures/style_parser/line-opacity.style.json#L1) |
| line-translate.style.json | 1 | [test/fixtures/style_parser/line-translate.style.json](../../test/fixtures/style_parser/line-translate.style.json#L1) |
| line-width.style.json | 1 | [test/fixtures/style_parser/line-width.style.json](../../test/fixtures/style_parser/line-width.style.json#L1) |
| non-object.style.json | 1 | [test/fixtures/style_parser/non-object.style.json](../../test/fixtures/style_parser/non-object.style.json#L1) |
| paint-nonobject.style.json | 1 | [test/fixtures/style_parser/paint-nonobject.style.json](../../test/fixtures/style_parser/paint-nonobject.style.json#L1) |
| sprites-missing-fields.style.json | 2 | [test/fixtures/style_parser/sprites-missing-fields.style.json](../../test/fixtures/style_parser/sprites-missing-fields.style.json#L1) |
| sprites-not-same-ids.style.json | 1 | [test/fixtures/style_parser/sprites-not-same-ids.style.json](../../test/fixtures/style_parser/sprites-not-same-ids.style.json#L1) |
| stop-zoom-value.style.json | 2 | [test/fixtures/style_parser/stop-zoom-value.style.json](../../test/fixtures/style_parser/stop-zoom-value.style.json#L1) |
| stops-array.style.json | 1 | [test/fixtures/style_parser/stops-array.style.json](../../test/fixtures/style_parser/stops-array.style.json#L1) |
| text-font.style.json | 8 | [test/fixtures/style_parser/text-font.style.json](../../test/fixtures/style_parser/text-font.style.json#L1) |
| text-size.style.json | 1 | [test/fixtures/style_parser/text-size.style.json](../../test/fixtures/style_parser/text-size.style.json#L1) |
| version-not-number.style.json | 1 | [test/fixtures/style_parser/version-not-number.style.json](../../test/fixtures/style_parser/version-not-number.style.json#L1) |

The sole strict-JSON error is [vendor/maplibre-native-base/deps/geojson.hpp/test/fixtures/invalid.json](../../vendor/maplibre-native-base/deps/geojson.hpp/test/fixtures/invalid.json#L1): `Expecting value: line 1 column 10 (char 9)`. It is dependency GeoJSON parser test data, not a style parse failure. The exclusion accounting below reports every non-style category by count; these are deliberate inventory exclusions, not unexamined candidate styles. Archived metric results, sprite metadata, source data, expected feature outputs, raw expression cases, parser diagnostic outputs and configuration are never counted as render styles. No ignored-test list or historical metric record was interpreted as current successful execution.
Confidence: high in tracked-file enumeration, structure counts and explicit API boundaries; conditional in proposed plugin analogues and framework semantic parity. Runtime correctness, backend coverage, remote style content and full statically constructed platform style expansion remain outside this read-only inventory.

## Exclusion accounting

All 10,450 enumerated files are accounted for: 1,573 style documents, 481 raw expression fixtures, one strict-JSON error, and 8,395 non-style exclusions. The raw expression fixtures remain included in the operator tables. Counts below are exclusive; the earlier scope table is an independent path-based breakdown. No files were omitted because of a failed style render.

| Excluded category | Files | Reason |
| --- | ---: | --- |
| other JSON/data/config/expected | 627 | Configuration, source data, package metadata and other documents without style shape. |
| metrics recorded result/manifest | 7,029 | Historical measurement records/manifests; not current test styles or evidence of passing tests. |
| GeoJSON geometry/data | 584 | Source geometry inputs; their owning styles are counted separately. |
| query expected result | 126 | Expected feature outputs paired with 126 query styles. |
| parser expectations/auxiliary | 29 | Diagnostic and parser auxiliary documents, not style inputs. |

There were no unavailable submodules among the 63 visited. External style/source URLs were not resolved, ignored-test manifests were not treated as execution results, and non-JSON programmatic styles were not evaluated. Untracked user files and the unrelated local GLFW CMake edit were excluded from the review. The seven style-shaped files in the archived-metrics scope remain counted as styles; only the 7,029 documents classified as records/manifests are excluded.

## Complete identified style-file ledger

Each of the 1,573 identified style files appears exactly once. Counts describe initial objects plus supported inline metadata mutations; they are not pass/fail results. Family capability judgments are given above and in the main review. Empty layer/source columns can be intentional parser negatives. Files with duplicate normalized JSON remain separately listed.

### benchmark fixtures (3)

| Style file | Layer families | Source types |
| --- | --- | --- |
| [benchmark/fixtures/api/style.json](../../benchmark/fixtures/api/style.json) | background, fill, fill-extrusion, line, symbol | vector |
| [benchmark/fixtures/api/style_formatted_labels.json](../../benchmark/fixtures/api/style_formatted_labels.json) | background, fill, fill-extrusion, line, symbol | vector |
| [benchmark/fixtures/renderer/liberty.json](../../benchmark/fixtures/renderer/liberty.json) | background, fill, fill-extrusion, line, raster, symbol | raster, vector |

### integration query tests (126)

| Style file | Layer families | Source types |
| --- | --- | --- |
| [metrics/integration/query-tests/circle-pitch-scale/map-inside-align-map/style.json](../../metrics/integration/query-tests/circle-pitch-scale/map-inside-align-map/style.json) | circle | geojson |
| [metrics/integration/query-tests/circle-pitch-scale/map-inside-align-viewport/style.json](../../metrics/integration/query-tests/circle-pitch-scale/map-inside-align-viewport/style.json) | circle | geojson |
| [metrics/integration/query-tests/circle-pitch-scale/map-outside-align-map/style.json](../../metrics/integration/query-tests/circle-pitch-scale/map-outside-align-map/style.json) | circle | geojson |
| [metrics/integration/query-tests/circle-pitch-scale/map-outside-align-viewport/style.json](../../metrics/integration/query-tests/circle-pitch-scale/map-outside-align-viewport/style.json) | circle | geojson |
| [metrics/integration/query-tests/circle-pitch-scale/viewport-inside-align-map/style.json](../../metrics/integration/query-tests/circle-pitch-scale/viewport-inside-align-map/style.json) | circle | geojson |
| [metrics/integration/query-tests/circle-pitch-scale/viewport-inside-align-viewport/style.json](../../metrics/integration/query-tests/circle-pitch-scale/viewport-inside-align-viewport/style.json) | circle | geojson |
| [metrics/integration/query-tests/circle-pitch-scale/viewport-outside-align-map/style.json](../../metrics/integration/query-tests/circle-pitch-scale/viewport-outside-align-map/style.json) | circle | geojson |
| [metrics/integration/query-tests/circle-pitch-scale/viewport-outside-align-viewport/style.json](../../metrics/integration/query-tests/circle-pitch-scale/viewport-outside-align-viewport/style.json) | circle | geojson |
| [metrics/integration/query-tests/circle-radius-features-in/inside/style.json](../../metrics/integration/query-tests/circle-radius-features-in/inside/style.json) | circle | vector |
| [metrics/integration/query-tests/circle-radius-features-in/outside/style.json](../../metrics/integration/query-tests/circle-radius-features-in/outside/style.json) | circle | vector |
| [metrics/integration/query-tests/circle-radius/feature-state/style.json](../../metrics/integration/query-tests/circle-radius/feature-state/style.json) | circle | geojson |
| [metrics/integration/query-tests/circle-radius/inside/style.json](../../metrics/integration/query-tests/circle-radius/inside/style.json) | circle | vector |
| [metrics/integration/query-tests/circle-radius/multiple-layers/style.json](../../metrics/integration/query-tests/circle-radius/multiple-layers/style.json) | circle | geojson |
| [metrics/integration/query-tests/circle-radius/outside/style.json](../../metrics/integration/query-tests/circle-radius/outside/style.json) | circle | vector |
| [metrics/integration/query-tests/circle-radius/property-function/style.json](../../metrics/integration/query-tests/circle-radius/property-function/style.json) | circle | geojson |
| [metrics/integration/query-tests/circle-radius/tile-boundary/style.json](../../metrics/integration/query-tests/circle-radius/tile-boundary/style.json) | circle | geojson |
| [metrics/integration/query-tests/circle-radius/zoom-and-property-function/style.json](../../metrics/integration/query-tests/circle-radius/zoom-and-property-function/style.json) | circle | geojson |
| [metrics/integration/query-tests/circle-stroke-width/feature-state/style.json](../../metrics/integration/query-tests/circle-stroke-width/feature-state/style.json) | circle | geojson |
| [metrics/integration/query-tests/circle-stroke-width/inside/style.json](../../metrics/integration/query-tests/circle-stroke-width/inside/style.json) | circle | geojson |
| [metrics/integration/query-tests/circle-stroke-width/outside/style.json](../../metrics/integration/query-tests/circle-stroke-width/outside/style.json) | circle | geojson |
| [metrics/integration/query-tests/circle-translate-anchor/map/style.json](../../metrics/integration/query-tests/circle-translate-anchor/map/style.json) | circle | vector |
| [metrics/integration/query-tests/circle-translate-anchor/viewport/style.json](../../metrics/integration/query-tests/circle-translate-anchor/viewport/style.json) | circle | vector |
| [metrics/integration/query-tests/circle-translate/box/style.json](../../metrics/integration/query-tests/circle-translate/box/style.json) | circle | vector |
| [metrics/integration/query-tests/circle-translate/inside/style.json](../../metrics/integration/query-tests/circle-translate/inside/style.json) | circle | vector |
| [metrics/integration/query-tests/circle-translate/outside/style.json](../../metrics/integration/query-tests/circle-translate/outside/style.json) | circle | vector |
| [metrics/integration/query-tests/edge-cases/box-cutting-antimeridian-z0/style.json](../../metrics/integration/query-tests/edge-cases/box-cutting-antimeridian-z0/style.json) | circle | geojson |
| [metrics/integration/query-tests/edge-cases/box-cutting-antimeridian-z1/style.json](../../metrics/integration/query-tests/edge-cases/box-cutting-antimeridian-z1/style.json) | circle | geojson |
| [metrics/integration/query-tests/edge-cases/null-island/style.json](../../metrics/integration/query-tests/edge-cases/null-island/style.json) | circle | geojson |
| [metrics/integration/query-tests/edge-cases/unsorted-keys/style.json](../../metrics/integration/query-tests/edge-cases/unsorted-keys/style.json) | symbol | vector |
| [metrics/integration/query-tests/evaluated/line-width/style.json](../../metrics/integration/query-tests/evaluated/line-width/style.json) | line | geojson |
| [metrics/integration/query-tests/feature-state/default/style.json](../../metrics/integration/query-tests/feature-state/default/style.json) | circle | vector |
| [metrics/integration/query-tests/fill-extrusion-translate/multiple-layers/style.json](../../metrics/integration/query-tests/fill-extrusion-translate/multiple-layers/style.json) | fill-extrusion | geojson |
| [metrics/integration/query-tests/fill-extrusion/base-in/style.json](../../metrics/integration/query-tests/fill-extrusion/base-in/style.json) | fill-extrusion | geojson |
| [metrics/integration/query-tests/fill-extrusion/base-out/style.json](../../metrics/integration/query-tests/fill-extrusion/base-out/style.json) | fill-extrusion | geojson |
| [metrics/integration/query-tests/fill-extrusion/box-in/style.json](../../metrics/integration/query-tests/fill-extrusion/box-in/style.json) | fill-extrusion | geojson |
| [metrics/integration/query-tests/fill-extrusion/box-out/style.json](../../metrics/integration/query-tests/fill-extrusion/box-out/style.json) | fill-extrusion | geojson |
| [metrics/integration/query-tests/fill-extrusion/side-in/style.json](../../metrics/integration/query-tests/fill-extrusion/side-in/style.json) | fill-extrusion | geojson |
| [metrics/integration/query-tests/fill-extrusion/side-out/style.json](../../metrics/integration/query-tests/fill-extrusion/side-out/style.json) | fill-extrusion | geojson |
| [metrics/integration/query-tests/fill-extrusion/sort-concave-inner/style.json](../../metrics/integration/query-tests/fill-extrusion/sort-concave-inner/style.json) | fill-extrusion | geojson |
| [metrics/integration/query-tests/fill-extrusion/sort-concave-outer/style.json](../../metrics/integration/query-tests/fill-extrusion/sort-concave-outer/style.json) | fill-extrusion | geojson |
| [metrics/integration/query-tests/fill-extrusion/sort-rotated/style.json](../../metrics/integration/query-tests/fill-extrusion/sort-rotated/style.json) | fill-extrusion | geojson |
| [metrics/integration/query-tests/fill-extrusion/sort/style.json](../../metrics/integration/query-tests/fill-extrusion/sort/style.json) | fill-extrusion | geojson |
| [metrics/integration/query-tests/fill-extrusion/top-in/style.json](../../metrics/integration/query-tests/fill-extrusion/top-in/style.json) | fill-extrusion | geojson |
| [metrics/integration/query-tests/fill-extrusion/top-out/style.json](../../metrics/integration/query-tests/fill-extrusion/top-out/style.json) | fill-extrusion | geojson |
| [metrics/integration/query-tests/fill-features-in/default/style.json](../../metrics/integration/query-tests/fill-features-in/default/style.json) | fill | vector |
| [metrics/integration/query-tests/fill-features-in/rotated/style.json](../../metrics/integration/query-tests/fill-features-in/rotated/style.json) | fill | vector |
| [metrics/integration/query-tests/fill-features-in/tilted/style.json](../../metrics/integration/query-tests/fill-features-in/tilted/style.json) | fill | vector |
| [metrics/integration/query-tests/fill-translate-anchor/map/style.json](../../metrics/integration/query-tests/fill-translate-anchor/map/style.json) | fill | vector |
| [metrics/integration/query-tests/fill-translate-anchor/viewport/style.json](../../metrics/integration/query-tests/fill-translate-anchor/viewport/style.json) | fill | vector |
| [metrics/integration/query-tests/fill-translate/literal/style.json](../../metrics/integration/query-tests/fill-translate/literal/style.json) | fill | vector |
| [metrics/integration/query-tests/fill-translate/multiple-layers/style.json](../../metrics/integration/query-tests/fill-translate/multiple-layers/style.json) | fill | geojson |
| [metrics/integration/query-tests/fill/default/style.json](../../metrics/integration/query-tests/fill/default/style.json) | fill | vector |
| [metrics/integration/query-tests/fill/fill-pattern/style.json](../../metrics/integration/query-tests/fill/fill-pattern/style.json) | fill | vector |
| [metrics/integration/query-tests/fill/overscaled/style.json](../../metrics/integration/query-tests/fill/overscaled/style.json) | fill | vector |
| [metrics/integration/query-tests/geometry/linestring/style.json](../../metrics/integration/query-tests/geometry/linestring/style.json) | line | geojson |
| [metrics/integration/query-tests/geometry/multilinestring/style.json](../../metrics/integration/query-tests/geometry/multilinestring/style.json) | line | geojson |
| [metrics/integration/query-tests/geometry/multipoint/style.json](../../metrics/integration/query-tests/geometry/multipoint/style.json) | circle | geojson |
| [metrics/integration/query-tests/geometry/multipolygon/style.json](../../metrics/integration/query-tests/geometry/multipolygon/style.json) | fill | geojson |
| [metrics/integration/query-tests/geometry/point/style.json](../../metrics/integration/query-tests/geometry/point/style.json) | circle | geojson |
| [metrics/integration/query-tests/geometry/polygon/style.json](../../metrics/integration/query-tests/geometry/polygon/style.json) | fill | geojson |
| [metrics/integration/query-tests/invisible-features/visibility-none/style.json](../../metrics/integration/query-tests/invisible-features/visibility-none/style.json) | fill | geojson |
| [metrics/integration/query-tests/invisible-features/zero-opacity/style.json](../../metrics/integration/query-tests/invisible-features/zero-opacity/style.json) | fill | geojson |
| [metrics/integration/query-tests/line-gap-width/feature-state/style.json](../../metrics/integration/query-tests/line-gap-width/feature-state/style.json) | line | geojson |
| [metrics/integration/query-tests/line-gap-width/inside-fractional/style.json](../../metrics/integration/query-tests/line-gap-width/inside-fractional/style.json) | line | vector |
| [metrics/integration/query-tests/line-gap-width/inside/style.json](../../metrics/integration/query-tests/line-gap-width/inside/style.json) | line | vector |
| [metrics/integration/query-tests/line-gap-width/outside-fractional/style.json](../../metrics/integration/query-tests/line-gap-width/outside-fractional/style.json) | line | vector |
| [metrics/integration/query-tests/line-gap-width/outside/style.json](../../metrics/integration/query-tests/line-gap-width/outside/style.json) | line | vector |
| [metrics/integration/query-tests/line-gap-width/property-function/style.json](../../metrics/integration/query-tests/line-gap-width/property-function/style.json) | line | geojson |
| [metrics/integration/query-tests/line-offset/feature-state/style.json](../../metrics/integration/query-tests/line-offset/feature-state/style.json) | line | geojson |
| [metrics/integration/query-tests/line-offset/inside-fractional/style.json](../../metrics/integration/query-tests/line-offset/inside-fractional/style.json) | line | vector |
| [metrics/integration/query-tests/line-offset/inside/style.json](../../metrics/integration/query-tests/line-offset/inside/style.json) | line | vector |
| [metrics/integration/query-tests/line-offset/outside-fractional/style.json](../../metrics/integration/query-tests/line-offset/outside-fractional/style.json) | line | vector |
| [metrics/integration/query-tests/line-offset/outside/style.json](../../metrics/integration/query-tests/line-offset/outside/style.json) | line | vector |
| [metrics/integration/query-tests/line-offset/pattern-feature-state/style.json](../../metrics/integration/query-tests/line-offset/pattern-feature-state/style.json) | line | geojson |
| [metrics/integration/query-tests/line-offset/property-function/style.json](../../metrics/integration/query-tests/line-offset/property-function/style.json) | line | geojson |
| [metrics/integration/query-tests/line-translate-anchor/map/style.json](../../metrics/integration/query-tests/line-translate-anchor/map/style.json) | line | vector |
| [metrics/integration/query-tests/line-translate-anchor/viewport/style.json](../../metrics/integration/query-tests/line-translate-anchor/viewport/style.json) | line | vector |
| [metrics/integration/query-tests/line-translate/inside/style.json](../../metrics/integration/query-tests/line-translate/inside/style.json) | line | vector |
| [metrics/integration/query-tests/line-translate/outside/style.json](../../metrics/integration/query-tests/line-translate/outside/style.json) | line | vector |
| [metrics/integration/query-tests/line-width-features-in/inside/style.json](../../metrics/integration/query-tests/line-width-features-in/inside/style.json) | line | vector |
| [metrics/integration/query-tests/line-width-features-in/outside/style.json](../../metrics/integration/query-tests/line-width-features-in/outside/style.json) | line | vector |
| [metrics/integration/query-tests/line-width-features-in/tilt-inside/style.json](../../metrics/integration/query-tests/line-width-features-in/tilt-inside/style.json) | line | vector |
| [metrics/integration/query-tests/line-width-features-in/tilt-outside/style.json](../../metrics/integration/query-tests/line-width-features-in/tilt-outside/style.json) | line | vector |
| [metrics/integration/query-tests/line-width/feature-state/style.json](../../metrics/integration/query-tests/line-width/feature-state/style.json) | line | geojson |
| [metrics/integration/query-tests/line-width/inside-fractional/style.json](../../metrics/integration/query-tests/line-width/inside-fractional/style.json) | line | vector |
| [metrics/integration/query-tests/line-width/inside/style.json](../../metrics/integration/query-tests/line-width/inside/style.json) | line | vector |
| [metrics/integration/query-tests/line-width/multiple-layers/style.json](../../metrics/integration/query-tests/line-width/multiple-layers/style.json) | line | geojson |
| [metrics/integration/query-tests/line-width/outside-fractional/style.json](../../metrics/integration/query-tests/line-width/outside-fractional/style.json) | line | vector |
| [metrics/integration/query-tests/line-width/outside/style.json](../../metrics/integration/query-tests/line-width/outside/style.json) | line | vector |
| [metrics/integration/query-tests/options/filter-false/style.json](../../metrics/integration/query-tests/options/filter-false/style.json) | circle | geojson |
| [metrics/integration/query-tests/options/filter-true/style.json](../../metrics/integration/query-tests/options/filter-true/style.json) | circle | geojson |
| [metrics/integration/query-tests/options/layers-multiple/style.json](../../metrics/integration/query-tests/options/layers-multiple/style.json) | circle | geojson |
| [metrics/integration/query-tests/options/layers-one/style.json](../../metrics/integration/query-tests/options/layers-one/style.json) | circle | geojson |
| [metrics/integration/query-tests/regressions/mapbox-gl-js#3534/style.json](../../metrics/integration/query-tests/regressions/mapbox-gl-js%233534/style.json) | fill, symbol | vector |
| [metrics/integration/query-tests/regressions/mapbox-gl-js#4417/style.json](../../metrics/integration/query-tests/regressions/mapbox-gl-js%234417/style.json) | circle | geojson |
| [metrics/integration/query-tests/regressions/mapbox-gl-js#4494/style.json](../../metrics/integration/query-tests/regressions/mapbox-gl-js%234494/style.json) | circle | geojson |
| [metrics/integration/query-tests/regressions/mapbox-gl-js#5172/style.json](../../metrics/integration/query-tests/regressions/mapbox-gl-js%235172/style.json) | background, symbol | geojson |
| [metrics/integration/query-tests/regressions/mapbox-gl-js#5473/style.json](../../metrics/integration/query-tests/regressions/mapbox-gl-js%235473/style.json) | background, heatmap | geojson |
| [metrics/integration/query-tests/regressions/mapbox-gl-js#5554/style.json](../../metrics/integration/query-tests/regressions/mapbox-gl-js%235554/style.json) | background, symbol | geojson |
| [metrics/integration/query-tests/regressions/mapbox-gl-js#6075/style.json](../../metrics/integration/query-tests/regressions/mapbox-gl-js%236075/style.json) | background, symbol | geojson |
| [metrics/integration/query-tests/regressions/mapbox-gl-js#6555/style.json](../../metrics/integration/query-tests/regressions/mapbox-gl-js%236555/style.json) | background, symbol | geojson |
| [metrics/integration/query-tests/regressions/mapbox-gl-js#7883/style.json](../../metrics/integration/query-tests/regressions/mapbox-gl-js%237883/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/query-tests/regressions/mapbox-gl-js#8999/style.json](../../metrics/integration/query-tests/regressions/mapbox-gl-js%238999/style.json) | fill-extrusion | geojson |
| [metrics/integration/query-tests/remove-feature-state/default/style.json](../../metrics/integration/query-tests/remove-feature-state/default/style.json) | circle | vector |
| [metrics/integration/query-tests/symbol-features-in/fractional-outside/style.json](../../metrics/integration/query-tests/symbol-features-in/fractional-outside/style.json) | symbol | vector |
| [metrics/integration/query-tests/symbol-features-in/hidden/style.json](../../metrics/integration/query-tests/symbol-features-in/hidden/style.json) | symbol | vector |
| [metrics/integration/query-tests/symbol-features-in/inside/style.json](../../metrics/integration/query-tests/symbol-features-in/inside/style.json) | symbol | vector |
| [metrics/integration/query-tests/symbol-features-in/outside/style.json](../../metrics/integration/query-tests/symbol-features-in/outside/style.json) | symbol | vector |
| [metrics/integration/query-tests/symbol-features-in/pitched-screen/style.json](../../metrics/integration/query-tests/symbol-features-in/pitched-screen/style.json) | symbol | geojson |
| [metrics/integration/query-tests/symbol-features-in/tilted-inside/style.json](../../metrics/integration/query-tests/symbol-features-in/tilted-inside/style.json) | symbol | vector |
| [metrics/integration/query-tests/symbol-features-in/tilted-outside/style.json](../../metrics/integration/query-tests/symbol-features-in/tilted-outside/style.json) | symbol | vector |
| [metrics/integration/query-tests/symbol-ignore-placement/inside/style.json](../../metrics/integration/query-tests/symbol-ignore-placement/inside/style.json) | symbol | vector |
| [metrics/integration/query-tests/symbol/filtered-rotated-after-insert/style.json](../../metrics/integration/query-tests/symbol/filtered-rotated-after-insert/style.json) | symbol | geojson |
| [metrics/integration/query-tests/symbol/filtered/style.json](../../metrics/integration/query-tests/symbol/filtered/style.json) | symbol | vector |
| [metrics/integration/query-tests/symbol/fractional-outside/style.json](../../metrics/integration/query-tests/symbol/fractional-outside/style.json) | symbol | vector |
| [metrics/integration/query-tests/symbol/hidden/style.json](../../metrics/integration/query-tests/symbol/hidden/style.json) | symbol | vector |
| [metrics/integration/query-tests/symbol/inside/style.json](../../metrics/integration/query-tests/symbol/inside/style.json) | symbol | vector |
| [metrics/integration/query-tests/symbol/outside/style.json](../../metrics/integration/query-tests/symbol/outside/style.json) | symbol | vector |
| [metrics/integration/query-tests/symbol/panned-after-insert/style.json](../../metrics/integration/query-tests/symbol/panned-after-insert/style.json) | symbol | geojson |
| [metrics/integration/query-tests/symbol/rotated-after-insert/style.json](../../metrics/integration/query-tests/symbol/rotated-after-insert/style.json) | symbol | geojson |
| [metrics/integration/query-tests/symbol/rotated-inside/style.json](../../metrics/integration/query-tests/symbol/rotated-inside/style.json) | symbol | vector |
| [metrics/integration/query-tests/symbol/rotated-outside/style.json](../../metrics/integration/query-tests/symbol/rotated-outside/style.json) | symbol | vector |
| [metrics/integration/query-tests/symbol/rotated-sort/style.json](../../metrics/integration/query-tests/symbol/rotated-sort/style.json) | symbol | geojson |
| [metrics/integration/query-tests/symbol/tile-boundary/style.json](../../metrics/integration/query-tests/symbol/tile-boundary/style.json) | symbol | geojson |
| [metrics/integration/query-tests/world-wrapping/box/style.json](../../metrics/integration/query-tests/world-wrapping/box/style.json) | circle | vector |
| [metrics/integration/query-tests/world-wrapping/point/style.json](../../metrics/integration/query-tests/world-wrapping/point/style.json) | circle | vector |

### integration render tests (1299)

| Style file | Layer families | Source types |
| --- | --- | --- |
| [metrics/integration/render-tests/background-color/colorSpace-hcl/style.json](../../metrics/integration/render-tests/background-color/colorSpace-hcl/style.json) | background | — |
| [metrics/integration/render-tests/background-color/colorSpace-lab/style.json](../../metrics/integration/render-tests/background-color/colorSpace-lab/style.json) | background | — |
| [metrics/integration/render-tests/background-color/default/style.json](../../metrics/integration/render-tests/background-color/default/style.json) | background | — |
| [metrics/integration/render-tests/background-color/function/style.json](../../metrics/integration/render-tests/background-color/function/style.json) | background | — |
| [metrics/integration/render-tests/background-color/literal/style.json](../../metrics/integration/render-tests/background-color/literal/style.json) | background | — |
| [metrics/integration/render-tests/background-color/transition/style.json](../../metrics/integration/render-tests/background-color/transition/style.json) | background | — |
| [metrics/integration/render-tests/background-opacity/color/style.json](../../metrics/integration/render-tests/background-opacity/color/style.json) | background | — |
| [metrics/integration/render-tests/background-opacity/image/style.json](../../metrics/integration/render-tests/background-opacity/image/style.json) | background | — |
| [metrics/integration/render-tests/background-opacity/overlay/style.json](../../metrics/integration/render-tests/background-opacity/overlay/style.json) | background, raster | raster |
| [metrics/integration/render-tests/background-pattern/@2x/style.json](../../metrics/integration/render-tests/background-pattern/@2x/style.json) | background | — |
| [metrics/integration/render-tests/background-pattern/literal/style.json](../../metrics/integration/render-tests/background-pattern/literal/style.json) | background | — |
| [metrics/integration/render-tests/background-pattern/missing/style.json](../../metrics/integration/render-tests/background-pattern/missing/style.json) | background | — |
| [metrics/integration/render-tests/background-pattern/pitch/style.json](../../metrics/integration/render-tests/background-pattern/pitch/style.json) | background | — |
| [metrics/integration/render-tests/background-pattern/rotated/style.json](../../metrics/integration/render-tests/background-pattern/rotated/style.json) | background | — |
| [metrics/integration/render-tests/background-pattern/zoomed/style.json](../../metrics/integration/render-tests/background-pattern/zoomed/style.json) | background | — |
| [metrics/integration/render-tests/background-visibility/none/style.json](../../metrics/integration/render-tests/background-visibility/none/style.json) | background | — |
| [metrics/integration/render-tests/background-visibility/visible/style.json](../../metrics/integration/render-tests/background-visibility/visible/style.json) | background | — |
| [metrics/integration/render-tests/basic-v9/z0-narrow-y/style.json](../../metrics/integration/render-tests/basic-v9/z0-narrow-y/style.json) | — | — |
| [metrics/integration/render-tests/basic-v9/z0-wide-x/style.json](../../metrics/integration/render-tests/basic-v9/z0-wide-x/style.json) | — | — |
| [metrics/integration/render-tests/basic-v9/z0/style.json](../../metrics/integration/render-tests/basic-v9/z0/style.json) | — | — |
| [metrics/integration/render-tests/bright-v9/z0/style.json](../../metrics/integration/render-tests/bright-v9/z0/style.json) | — | — |
| [metrics/integration/render-tests/canvas/default/style.json](../../metrics/integration/render-tests/canvas/default/style.json) | raster | canvas |
| [metrics/integration/render-tests/canvas/update/style.json](../../metrics/integration/render-tests/canvas/update/style.json) | raster | canvas |
| [metrics/integration/render-tests/circle-blur/blending/style.json](../../metrics/integration/render-tests/circle-blur/blending/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-blur/default/style.json](../../metrics/integration/render-tests/circle-blur/default/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-blur/function/style.json](../../metrics/integration/render-tests/circle-blur/function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-blur/literal-stroke/style.json](../../metrics/integration/render-tests/circle-blur/literal-stroke/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-blur/literal/style.json](../../metrics/integration/render-tests/circle-blur/literal/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-blur/property-function/style.json](../../metrics/integration/render-tests/circle-blur/property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-blur/zoom-and-property-function/style.json](../../metrics/integration/render-tests/circle-blur/zoom-and-property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-color/default/style.json](../../metrics/integration/render-tests/circle-color/default/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-color/function/style.json](../../metrics/integration/render-tests/circle-color/function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-color/literal/style.json](../../metrics/integration/render-tests/circle-color/literal/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-color/property-function/style.json](../../metrics/integration/render-tests/circle-color/property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-color/zoom-and-property-function/style.json](../../metrics/integration/render-tests/circle-color/zoom-and-property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-geometry/linestring/style.json](../../metrics/integration/render-tests/circle-geometry/linestring/style.json) | background, circle | geojson |
| [metrics/integration/render-tests/circle-geometry/multilinestring/style.json](../../metrics/integration/render-tests/circle-geometry/multilinestring/style.json) | background, circle | geojson |
| [metrics/integration/render-tests/circle-geometry/multipoint/style.json](../../metrics/integration/render-tests/circle-geometry/multipoint/style.json) | background, circle | geojson |
| [metrics/integration/render-tests/circle-geometry/multipolygon/style.json](../../metrics/integration/render-tests/circle-geometry/multipolygon/style.json) | background, circle | geojson |
| [metrics/integration/render-tests/circle-geometry/point/style.json](../../metrics/integration/render-tests/circle-geometry/point/style.json) | background, circle | geojson |
| [metrics/integration/render-tests/circle-geometry/polygon/style.json](../../metrics/integration/render-tests/circle-geometry/polygon/style.json) | background, circle | geojson |
| [metrics/integration/render-tests/circle-opacity/blending/style.json](../../metrics/integration/render-tests/circle-opacity/blending/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-opacity/default/style.json](../../metrics/integration/render-tests/circle-opacity/default/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-opacity/function/style.json](../../metrics/integration/render-tests/circle-opacity/function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-opacity/literal/style.json](../../metrics/integration/render-tests/circle-opacity/literal/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-opacity/property-function/style.json](../../metrics/integration/render-tests/circle-opacity/property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-opacity/zoom-and-property-function/style.json](../../metrics/integration/render-tests/circle-opacity/zoom-and-property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-pitch-alignment/map-scale-map/style.json](../../metrics/integration/render-tests/circle-pitch-alignment/map-scale-map/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-pitch-alignment/map-scale-viewport/style.json](../../metrics/integration/render-tests/circle-pitch-alignment/map-scale-viewport/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-pitch-alignment/viewport-scale-map/style.json](../../metrics/integration/render-tests/circle-pitch-alignment/viewport-scale-map/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-pitch-alignment/viewport-scale-viewport/style.json](../../metrics/integration/render-tests/circle-pitch-alignment/viewport-scale-viewport/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-pitch-scale/default/style.json](../../metrics/integration/render-tests/circle-pitch-scale/default/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-pitch-scale/map/style.json](../../metrics/integration/render-tests/circle-pitch-scale/map/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-pitch-scale/viewport/style.json](../../metrics/integration/render-tests/circle-pitch-scale/viewport/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-radius/antimeridian/style.json](../../metrics/integration/render-tests/circle-radius/antimeridian/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-radius/default/style.json](../../metrics/integration/render-tests/circle-radius/default/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-radius/function/style.json](../../metrics/integration/render-tests/circle-radius/function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-radius/literal/style.json](../../metrics/integration/render-tests/circle-radius/literal/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-radius/property-function/style.json](../../metrics/integration/render-tests/circle-radius/property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-radius/zoom-and-property-function/style.json](../../metrics/integration/render-tests/circle-radius/zoom-and-property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-sort-key/literal/style.json](../../metrics/integration/render-tests/circle-sort-key/literal/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-stroke-color/default/style.json](../../metrics/integration/render-tests/circle-stroke-color/default/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-stroke-color/function/style.json](../../metrics/integration/render-tests/circle-stroke-color/function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-stroke-color/literal/style.json](../../metrics/integration/render-tests/circle-stroke-color/literal/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-stroke-color/property-function/style.json](../../metrics/integration/render-tests/circle-stroke-color/property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-stroke-color/zoom-and-property-function/style.json](../../metrics/integration/render-tests/circle-stroke-color/zoom-and-property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-stroke-opacity/default/style.json](../../metrics/integration/render-tests/circle-stroke-opacity/default/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-stroke-opacity/function/style.json](../../metrics/integration/render-tests/circle-stroke-opacity/function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-stroke-opacity/literal/style.json](../../metrics/integration/render-tests/circle-stroke-opacity/literal/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-stroke-opacity/property-function/style.json](../../metrics/integration/render-tests/circle-stroke-opacity/property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-stroke-opacity/stroke-only/style.json](../../metrics/integration/render-tests/circle-stroke-opacity/stroke-only/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-stroke-opacity/zoom-and-property-function/style.json](../../metrics/integration/render-tests/circle-stroke-opacity/zoom-and-property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-stroke-width/default/style.json](../../metrics/integration/render-tests/circle-stroke-width/default/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-stroke-width/function/style.json](../../metrics/integration/render-tests/circle-stroke-width/function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-stroke-width/literal/style.json](../../metrics/integration/render-tests/circle-stroke-width/literal/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-stroke-width/property-function/style.json](../../metrics/integration/render-tests/circle-stroke-width/property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-stroke-width/zoom-and-property-function/style.json](../../metrics/integration/render-tests/circle-stroke-width/zoom-and-property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-translate-anchor/map/style.json](../../metrics/integration/render-tests/circle-translate-anchor/map/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-translate-anchor/viewport/style.json](../../metrics/integration/render-tests/circle-translate-anchor/viewport/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-translate/default/style.json](../../metrics/integration/render-tests/circle-translate/default/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-translate/function/style.json](../../metrics/integration/render-tests/circle-translate/function/style.json) | circle | geojson |
| [metrics/integration/render-tests/circle-translate/literal/style.json](../../metrics/integration/render-tests/circle-translate/literal/style.json) | circle | geojson |
| [metrics/integration/render-tests/collator/default/style.json](../../metrics/integration/render-tests/collator/default/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/collator/resolved-locale/style.json](../../metrics/integration/render-tests/collator/resolved-locale/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/color-relief/hillshade/style.json](../../metrics/integration/render-tests/color-relief/hillshade/style.json) | color-relief, hillshade | raster-dem |
| [metrics/integration/render-tests/color-relief/low-zoom/style.json](../../metrics/integration/render-tests/color-relief/low-zoom/style.json) | background, color-relief | raster-dem |
| [metrics/integration/render-tests/color-relief/opacity/style.json](../../metrics/integration/render-tests/color-relief/opacity/style.json) | color-relief | raster-dem |
| [metrics/integration/render-tests/color-relief/rainbow/style.json](../../metrics/integration/render-tests/color-relief/rainbow/style.json) | color-relief | raster-dem |
| [metrics/integration/render-tests/color-relief/transparency/style.json](../../metrics/integration/render-tests/color-relief/transparency/style.json) | color-relief | raster-dem |
| [metrics/integration/render-tests/combinations/background-opaque--background-opaque/style.json](../../metrics/integration/render-tests/combinations/background-opaque--background-opaque/style.json) | background | — |
| [metrics/integration/render-tests/combinations/background-opaque--background-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--background-translucent/style.json) | background | — |
| [metrics/integration/render-tests/combinations/background-opaque--circle-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--circle-translucent/style.json) | background, circle | geojson |
| [metrics/integration/render-tests/combinations/background-opaque--fill-extrusion-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--fill-extrusion-translucent/style.json) | background, fill-extrusion | geojson |
| [metrics/integration/render-tests/combinations/background-opaque--fill-opaque/style.json](../../metrics/integration/render-tests/combinations/background-opaque--fill-opaque/style.json) | background, fill | geojson |
| [metrics/integration/render-tests/combinations/background-opaque--fill-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--fill-translucent/style.json) | background, fill | geojson |
| [metrics/integration/render-tests/combinations/background-opaque--heatmap-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--heatmap-translucent/style.json) | background, heatmap | geojson |
| [metrics/integration/render-tests/combinations/background-opaque--hillshade-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--hillshade-translucent/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/combinations/background-opaque--line-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--line-translucent/style.json) | background, line | geojson |
| [metrics/integration/render-tests/combinations/background-opaque--raster-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--raster-translucent/style.json) | background, raster | raster |
| [metrics/integration/render-tests/combinations/background-opaque--symbol-translucent/style.json](../../metrics/integration/render-tests/combinations/background-opaque--symbol-translucent/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/combinations/background-translucent--background-opaque/style.json](../../metrics/integration/render-tests/combinations/background-translucent--background-opaque/style.json) | background | — |
| [metrics/integration/render-tests/combinations/background-translucent--background-translucent/style.json](../../metrics/integration/render-tests/combinations/background-translucent--background-translucent/style.json) | background | — |
| [metrics/integration/render-tests/combinations/background-translucent--circle-translucent/style.json](../../metrics/integration/render-tests/combinations/background-translucent--circle-translucent/style.json) | background, circle | geojson |
| [metrics/integration/render-tests/combinations/background-translucent--fill-extrusion-translucent/style.json](../../metrics/integration/render-tests/combinations/background-translucent--fill-extrusion-translucent/style.json) | background, fill-extrusion | geojson |
| [metrics/integration/render-tests/combinations/background-translucent--fill-opaque/style.json](../../metrics/integration/render-tests/combinations/background-translucent--fill-opaque/style.json) | background, fill | geojson |
| [metrics/integration/render-tests/combinations/background-translucent--fill-translucent/style.json](../../metrics/integration/render-tests/combinations/background-translucent--fill-translucent/style.json) | background, fill | geojson |
| [metrics/integration/render-tests/combinations/background-translucent--heatmap-translucent/style.json](../../metrics/integration/render-tests/combinations/background-translucent--heatmap-translucent/style.json) | background, heatmap | geojson |
| [metrics/integration/render-tests/combinations/background-translucent--hillshade-translucent/style.json](../../metrics/integration/render-tests/combinations/background-translucent--hillshade-translucent/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/combinations/background-translucent--line-translucent/style.json](../../metrics/integration/render-tests/combinations/background-translucent--line-translucent/style.json) | background, line | geojson |
| [metrics/integration/render-tests/combinations/background-translucent--raster-translucent/style.json](../../metrics/integration/render-tests/combinations/background-translucent--raster-translucent/style.json) | background, raster | raster |
| [metrics/integration/render-tests/combinations/background-translucent--symbol-translucent/style.json](../../metrics/integration/render-tests/combinations/background-translucent--symbol-translucent/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/combinations/circle-translucent--background-opaque/style.json](../../metrics/integration/render-tests/combinations/circle-translucent--background-opaque/style.json) | background, circle | geojson |
| [metrics/integration/render-tests/combinations/circle-translucent--background-translucent/style.json](../../metrics/integration/render-tests/combinations/circle-translucent--background-translucent/style.json) | background, circle | geojson |
| [metrics/integration/render-tests/combinations/circle-translucent--circle-translucent/style.json](../../metrics/integration/render-tests/combinations/circle-translucent--circle-translucent/style.json) | circle | geojson |
| [metrics/integration/render-tests/combinations/circle-translucent--fill-extrusion-translucent/style.json](../../metrics/integration/render-tests/combinations/circle-translucent--fill-extrusion-translucent/style.json) | circle, fill-extrusion | geojson |
| [metrics/integration/render-tests/combinations/circle-translucent--fill-opaque/style.json](../../metrics/integration/render-tests/combinations/circle-translucent--fill-opaque/style.json) | circle, fill | geojson |
| [metrics/integration/render-tests/combinations/circle-translucent--fill-translucent/style.json](../../metrics/integration/render-tests/combinations/circle-translucent--fill-translucent/style.json) | circle, fill | geojson |
| [metrics/integration/render-tests/combinations/circle-translucent--heatmap-translucent/style.json](../../metrics/integration/render-tests/combinations/circle-translucent--heatmap-translucent/style.json) | circle, heatmap | geojson |
| [metrics/integration/render-tests/combinations/circle-translucent--hillshade-translucent/style.json](../../metrics/integration/render-tests/combinations/circle-translucent--hillshade-translucent/style.json) | circle, hillshade | geojson, raster-dem |
| [metrics/integration/render-tests/combinations/circle-translucent--line-translucent/style.json](../../metrics/integration/render-tests/combinations/circle-translucent--line-translucent/style.json) | circle, line | geojson |
| [metrics/integration/render-tests/combinations/circle-translucent--raster-translucent/style.json](../../metrics/integration/render-tests/combinations/circle-translucent--raster-translucent/style.json) | circle, raster | geojson, raster |
| [metrics/integration/render-tests/combinations/circle-translucent--symbol-translucent/style.json](../../metrics/integration/render-tests/combinations/circle-translucent--symbol-translucent/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/combinations/color-relief--hillshade--color-relief--hillshade/style.json](../../metrics/integration/render-tests/combinations/color-relief--hillshade--color-relief--hillshade/style.json) | background, color-relief, hillshade | raster-dem |
| [metrics/integration/render-tests/combinations/color-relief--hillshade--vector-fill--color-relief--hillshade/style.json](../../metrics/integration/render-tests/combinations/color-relief--hillshade--vector-fill--color-relief--hillshade/style.json) | background, color-relief, fill, hillshade | raster-dem, vector |
| [metrics/integration/render-tests/combinations/color-relief-translucent--hillshade-translucent-low-zoom/style.json](../../metrics/integration/render-tests/combinations/color-relief-translucent--hillshade-translucent-low-zoom/style.json) | background, color-relief, hillshade | raster-dem |
| [metrics/integration/render-tests/combinations/fill-extrusion--fill-opaque/style.json](../../metrics/integration/render-tests/combinations/fill-extrusion--fill-opaque/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/combinations/fill-extrusion--fill-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-extrusion--fill-translucent/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/combinations/fill-extrusion-translucent--background-opaque/style.json](../../metrics/integration/render-tests/combinations/fill-extrusion-translucent--background-opaque/style.json) | background, fill-extrusion | geojson |
| [metrics/integration/render-tests/combinations/fill-extrusion-translucent--background-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-extrusion-translucent--background-translucent/style.json) | background, fill-extrusion | geojson |
| [metrics/integration/render-tests/combinations/fill-extrusion-translucent--circle-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-extrusion-translucent--circle-translucent/style.json) | circle, fill-extrusion | geojson |
| [metrics/integration/render-tests/combinations/fill-extrusion-translucent--fill-extrusion-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-extrusion-translucent--fill-extrusion-translucent/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/combinations/fill-extrusion-translucent--fill-opaque/style.json](../../metrics/integration/render-tests/combinations/fill-extrusion-translucent--fill-opaque/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/combinations/fill-extrusion-translucent--fill-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-extrusion-translucent--fill-translucent/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/combinations/fill-extrusion-translucent--heatmap-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-extrusion-translucent--heatmap-translucent/style.json) | fill-extrusion, heatmap | geojson |
| [metrics/integration/render-tests/combinations/fill-extrusion-translucent--hillshade-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-extrusion-translucent--hillshade-translucent/style.json) | fill-extrusion, hillshade | geojson, raster-dem |
| [metrics/integration/render-tests/combinations/fill-extrusion-translucent--line-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-extrusion-translucent--line-translucent/style.json) | fill-extrusion, line | geojson |
| [metrics/integration/render-tests/combinations/fill-extrusion-translucent--raster-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-extrusion-translucent--raster-translucent/style.json) | fill-extrusion, raster | geojson, raster |
| [metrics/integration/render-tests/combinations/fill-extrusion-translucent--symbol-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-extrusion-translucent--symbol-translucent/style.json) | fill-extrusion, symbol | geojson |
| [metrics/integration/render-tests/combinations/fill-opaque--background-opaque/style.json](../../metrics/integration/render-tests/combinations/fill-opaque--background-opaque/style.json) | background, fill | geojson |
| [metrics/integration/render-tests/combinations/fill-opaque--background-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-opaque--background-translucent/style.json) | background, fill | geojson |
| [metrics/integration/render-tests/combinations/fill-opaque--circle-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-opaque--circle-translucent/style.json) | circle, fill | geojson |
| [metrics/integration/render-tests/combinations/fill-opaque--fill-extrusion-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-opaque--fill-extrusion-translucent/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/combinations/fill-opaque--fill-opaque/style.json](../../metrics/integration/render-tests/combinations/fill-opaque--fill-opaque/style.json) | fill | geojson |
| [metrics/integration/render-tests/combinations/fill-opaque--fill-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-opaque--fill-translucent/style.json) | fill | geojson |
| [metrics/integration/render-tests/combinations/fill-opaque--heatmap-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-opaque--heatmap-translucent/style.json) | fill, heatmap | geojson |
| [metrics/integration/render-tests/combinations/fill-opaque--hillshade-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-opaque--hillshade-translucent/style.json) | fill, hillshade | geojson, raster-dem |
| [metrics/integration/render-tests/combinations/fill-opaque--image-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-opaque--image-translucent/style.json) | fill, raster | geojson, image |
| [metrics/integration/render-tests/combinations/fill-opaque--line-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-opaque--line-translucent/style.json) | fill, line | geojson |
| [metrics/integration/render-tests/combinations/fill-opaque--raster-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-opaque--raster-translucent/style.json) | fill, raster | geojson, raster |
| [metrics/integration/render-tests/combinations/fill-opaque--symbol-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-opaque--symbol-translucent/style.json) | fill, symbol | geojson |
| [metrics/integration/render-tests/combinations/fill-translucent--background-opaque/style.json](../../metrics/integration/render-tests/combinations/fill-translucent--background-opaque/style.json) | background, fill | geojson |
| [metrics/integration/render-tests/combinations/fill-translucent--background-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-translucent--background-translucent/style.json) | background, fill | geojson |
| [metrics/integration/render-tests/combinations/fill-translucent--circle-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-translucent--circle-translucent/style.json) | circle, fill | geojson |
| [metrics/integration/render-tests/combinations/fill-translucent--fill-extrusion-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-translucent--fill-extrusion-translucent/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/combinations/fill-translucent--fill-opaque/style.json](../../metrics/integration/render-tests/combinations/fill-translucent--fill-opaque/style.json) | fill | geojson |
| [metrics/integration/render-tests/combinations/fill-translucent--fill-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-translucent--fill-translucent/style.json) | fill | geojson |
| [metrics/integration/render-tests/combinations/fill-translucent--heatmap-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-translucent--heatmap-translucent/style.json) | fill, heatmap | geojson |
| [metrics/integration/render-tests/combinations/fill-translucent--hillshade-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-translucent--hillshade-translucent/style.json) | fill, hillshade | geojson, raster-dem |
| [metrics/integration/render-tests/combinations/fill-translucent--line-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-translucent--line-translucent/style.json) | fill, line | geojson |
| [metrics/integration/render-tests/combinations/fill-translucent--raster-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-translucent--raster-translucent/style.json) | fill, raster | geojson, raster |
| [metrics/integration/render-tests/combinations/fill-translucent--symbol-translucent/style.json](../../metrics/integration/render-tests/combinations/fill-translucent--symbol-translucent/style.json) | fill, symbol | geojson |
| [metrics/integration/render-tests/combinations/heatmap-translucent--background-opaque/style.json](../../metrics/integration/render-tests/combinations/heatmap-translucent--background-opaque/style.json) | background, heatmap | geojson |
| [metrics/integration/render-tests/combinations/heatmap-translucent--background-translucent/style.json](../../metrics/integration/render-tests/combinations/heatmap-translucent--background-translucent/style.json) | background, heatmap | geojson |
| [metrics/integration/render-tests/combinations/heatmap-translucent--circle-translucent/style.json](../../metrics/integration/render-tests/combinations/heatmap-translucent--circle-translucent/style.json) | circle, heatmap | geojson |
| [metrics/integration/render-tests/combinations/heatmap-translucent--fill-extrusion-translucent/style.json](../../metrics/integration/render-tests/combinations/heatmap-translucent--fill-extrusion-translucent/style.json) | fill-extrusion, heatmap | geojson |
| [metrics/integration/render-tests/combinations/heatmap-translucent--fill-opaque/style.json](../../metrics/integration/render-tests/combinations/heatmap-translucent--fill-opaque/style.json) | fill, heatmap | geojson |
| [metrics/integration/render-tests/combinations/heatmap-translucent--fill-translucent/style.json](../../metrics/integration/render-tests/combinations/heatmap-translucent--fill-translucent/style.json) | fill, heatmap | geojson |
| [metrics/integration/render-tests/combinations/heatmap-translucent--heatmap-translucent/style.json](../../metrics/integration/render-tests/combinations/heatmap-translucent--heatmap-translucent/style.json) | heatmap | geojson |
| [metrics/integration/render-tests/combinations/heatmap-translucent--hillshade-translucent/style.json](../../metrics/integration/render-tests/combinations/heatmap-translucent--hillshade-translucent/style.json) | heatmap, hillshade | geojson, raster-dem |
| [metrics/integration/render-tests/combinations/heatmap-translucent--line-translucent/style.json](../../metrics/integration/render-tests/combinations/heatmap-translucent--line-translucent/style.json) | heatmap, line | geojson |
| [metrics/integration/render-tests/combinations/heatmap-translucent--raster-translucent/style.json](../../metrics/integration/render-tests/combinations/heatmap-translucent--raster-translucent/style.json) | heatmap, raster | geojson, raster |
| [metrics/integration/render-tests/combinations/heatmap-translucent--symbol-translucent/style.json](../../metrics/integration/render-tests/combinations/heatmap-translucent--symbol-translucent/style.json) | heatmap, symbol | geojson |
| [metrics/integration/render-tests/combinations/hillshade-translucent--background-opaque/style.json](../../metrics/integration/render-tests/combinations/hillshade-translucent--background-opaque/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/combinations/hillshade-translucent--background-translucent/style.json](../../metrics/integration/render-tests/combinations/hillshade-translucent--background-translucent/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/combinations/hillshade-translucent--circle-translucent/style.json](../../metrics/integration/render-tests/combinations/hillshade-translucent--circle-translucent/style.json) | circle, hillshade | geojson, raster-dem |
| [metrics/integration/render-tests/combinations/hillshade-translucent--fill-extrusion-translucent/style.json](../../metrics/integration/render-tests/combinations/hillshade-translucent--fill-extrusion-translucent/style.json) | fill-extrusion, hillshade | geojson, raster-dem |
| [metrics/integration/render-tests/combinations/hillshade-translucent--fill-opaque/style.json](../../metrics/integration/render-tests/combinations/hillshade-translucent--fill-opaque/style.json) | fill, hillshade | geojson, raster-dem |
| [metrics/integration/render-tests/combinations/hillshade-translucent--fill-translucent/style.json](../../metrics/integration/render-tests/combinations/hillshade-translucent--fill-translucent/style.json) | fill, hillshade | geojson, raster-dem |
| [metrics/integration/render-tests/combinations/hillshade-translucent--heatmap-translucent/style.json](../../metrics/integration/render-tests/combinations/hillshade-translucent--heatmap-translucent/style.json) | heatmap, hillshade | geojson, raster-dem |
| [metrics/integration/render-tests/combinations/hillshade-translucent--hillshade-translucent/style.json](../../metrics/integration/render-tests/combinations/hillshade-translucent--hillshade-translucent/style.json) | hillshade | raster-dem |
| [metrics/integration/render-tests/combinations/hillshade-translucent--line-translucent/style.json](../../metrics/integration/render-tests/combinations/hillshade-translucent--line-translucent/style.json) | hillshade, line | geojson, raster-dem |
| [metrics/integration/render-tests/combinations/hillshade-translucent--raster-translucent/style.json](../../metrics/integration/render-tests/combinations/hillshade-translucent--raster-translucent/style.json) | hillshade, raster | raster, raster-dem |
| [metrics/integration/render-tests/combinations/hillshade-translucent--symbol-translucent/style.json](../../metrics/integration/render-tests/combinations/hillshade-translucent--symbol-translucent/style.json) | hillshade, symbol | geojson, raster-dem |
| [metrics/integration/render-tests/combinations/line-translucent--background-opaque/style.json](../../metrics/integration/render-tests/combinations/line-translucent--background-opaque/style.json) | background, line | geojson |
| [metrics/integration/render-tests/combinations/line-translucent--background-translucent/style.json](../../metrics/integration/render-tests/combinations/line-translucent--background-translucent/style.json) | background, line | geojson |
| [metrics/integration/render-tests/combinations/line-translucent--circle-translucent/style.json](../../metrics/integration/render-tests/combinations/line-translucent--circle-translucent/style.json) | circle, line | geojson |
| [metrics/integration/render-tests/combinations/line-translucent--fill-extrusion-translucent/style.json](../../metrics/integration/render-tests/combinations/line-translucent--fill-extrusion-translucent/style.json) | fill-extrusion, line | geojson |
| [metrics/integration/render-tests/combinations/line-translucent--fill-opaque/style.json](../../metrics/integration/render-tests/combinations/line-translucent--fill-opaque/style.json) | fill, line | geojson |
| [metrics/integration/render-tests/combinations/line-translucent--fill-translucent/style.json](../../metrics/integration/render-tests/combinations/line-translucent--fill-translucent/style.json) | fill, line | geojson |
| [metrics/integration/render-tests/combinations/line-translucent--heatmap-translucent/style.json](../../metrics/integration/render-tests/combinations/line-translucent--heatmap-translucent/style.json) | heatmap, line | geojson |
| [metrics/integration/render-tests/combinations/line-translucent--hillshade-translucent/style.json](../../metrics/integration/render-tests/combinations/line-translucent--hillshade-translucent/style.json) | hillshade, line | geojson, raster-dem |
| [metrics/integration/render-tests/combinations/line-translucent--line-translucent/style.json](../../metrics/integration/render-tests/combinations/line-translucent--line-translucent/style.json) | line | geojson |
| [metrics/integration/render-tests/combinations/line-translucent--raster-translucent/style.json](../../metrics/integration/render-tests/combinations/line-translucent--raster-translucent/style.json) | line, raster | geojson, raster |
| [metrics/integration/render-tests/combinations/line-translucent--symbol-translucent/style.json](../../metrics/integration/render-tests/combinations/line-translucent--symbol-translucent/style.json) | line, symbol | geojson |
| [metrics/integration/render-tests/combinations/raster-translucent--background-opaque/style.json](../../metrics/integration/render-tests/combinations/raster-translucent--background-opaque/style.json) | background, raster | raster |
| [metrics/integration/render-tests/combinations/raster-translucent--background-translucent/style.json](../../metrics/integration/render-tests/combinations/raster-translucent--background-translucent/style.json) | background, raster | raster |
| [metrics/integration/render-tests/combinations/raster-translucent--circle-translucent/style.json](../../metrics/integration/render-tests/combinations/raster-translucent--circle-translucent/style.json) | circle, raster | geojson, raster |
| [metrics/integration/render-tests/combinations/raster-translucent--fill-extrusion-translucent/style.json](../../metrics/integration/render-tests/combinations/raster-translucent--fill-extrusion-translucent/style.json) | fill-extrusion, raster | geojson, raster |
| [metrics/integration/render-tests/combinations/raster-translucent--fill-opaque/style.json](../../metrics/integration/render-tests/combinations/raster-translucent--fill-opaque/style.json) | fill, raster | geojson, raster |
| [metrics/integration/render-tests/combinations/raster-translucent--fill-translucent/style.json](../../metrics/integration/render-tests/combinations/raster-translucent--fill-translucent/style.json) | fill, raster | geojson, raster |
| [metrics/integration/render-tests/combinations/raster-translucent--heatmap-translucent/style.json](../../metrics/integration/render-tests/combinations/raster-translucent--heatmap-translucent/style.json) | heatmap, raster | geojson, raster |
| [metrics/integration/render-tests/combinations/raster-translucent--hillshade-translucent/style.json](../../metrics/integration/render-tests/combinations/raster-translucent--hillshade-translucent/style.json) | hillshade, raster | raster, raster-dem |
| [metrics/integration/render-tests/combinations/raster-translucent--line-translucent/style.json](../../metrics/integration/render-tests/combinations/raster-translucent--line-translucent/style.json) | line, raster | geojson, raster |
| [metrics/integration/render-tests/combinations/raster-translucent--raster-translucent/style.json](../../metrics/integration/render-tests/combinations/raster-translucent--raster-translucent/style.json) | raster | raster |
| [metrics/integration/render-tests/combinations/raster-translucent--symbol-translucent/style.json](../../metrics/integration/render-tests/combinations/raster-translucent--symbol-translucent/style.json) | raster, symbol | geojson, raster |
| [metrics/integration/render-tests/combinations/symbol-translucent--background-opaque/style.json](../../metrics/integration/render-tests/combinations/symbol-translucent--background-opaque/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/combinations/symbol-translucent--background-translucent/style.json](../../metrics/integration/render-tests/combinations/symbol-translucent--background-translucent/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/combinations/symbol-translucent--circle-translucent/style.json](../../metrics/integration/render-tests/combinations/symbol-translucent--circle-translucent/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/combinations/symbol-translucent--fill-extrusion-translucent/style.json](../../metrics/integration/render-tests/combinations/symbol-translucent--fill-extrusion-translucent/style.json) | fill-extrusion, symbol | geojson |
| [metrics/integration/render-tests/combinations/symbol-translucent--fill-opaque/style.json](../../metrics/integration/render-tests/combinations/symbol-translucent--fill-opaque/style.json) | fill, symbol | geojson |
| [metrics/integration/render-tests/combinations/symbol-translucent--fill-translucent/style.json](../../metrics/integration/render-tests/combinations/symbol-translucent--fill-translucent/style.json) | fill, symbol | geojson |
| [metrics/integration/render-tests/combinations/symbol-translucent--heatmap-translucent/style.json](../../metrics/integration/render-tests/combinations/symbol-translucent--heatmap-translucent/style.json) | heatmap, symbol | geojson |
| [metrics/integration/render-tests/combinations/symbol-translucent--hillshade-translucent/style.json](../../metrics/integration/render-tests/combinations/symbol-translucent--hillshade-translucent/style.json) | hillshade, symbol | geojson, raster-dem |
| [metrics/integration/render-tests/combinations/symbol-translucent--line-translucent/style.json](../../metrics/integration/render-tests/combinations/symbol-translucent--line-translucent/style.json) | line, symbol | geojson |
| [metrics/integration/render-tests/combinations/symbol-translucent--raster-translucent/style.json](../../metrics/integration/render-tests/combinations/symbol-translucent--raster-translucent/style.json) | raster, symbol | geojson, raster |
| [metrics/integration/render-tests/combinations/symbol-translucent--symbol-translucent/style.json](../../metrics/integration/render-tests/combinations/symbol-translucent--symbol-translucent/style.json) | symbol | geojson |
| [metrics/integration/render-tests/custom-layer-js/depth/style.json](../../metrics/integration/render-tests/custom-layer-js/depth/style.json) | background, fill | geojson |
| [metrics/integration/render-tests/custom-layer-js/null-island/style.json](../../metrics/integration/render-tests/custom-layer-js/null-island/style.json) | — | — |
| [metrics/integration/render-tests/custom-layer-js/tent-3d/style.json](../../metrics/integration/render-tests/custom-layer-js/tent-3d/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json](../../metrics/integration/render-tests/debug/collision-icon-text-line-translate/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/debug/collision-icon-text-point-translate/style.json](../../metrics/integration/render-tests/debug/collision-icon-text-point-translate/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/debug/collision-lines-overscaled/style.json](../../metrics/integration/render-tests/debug/collision-lines-overscaled/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/debug/collision-lines-pitched/style.json](../../metrics/integration/render-tests/debug/collision-lines-pitched/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/debug/collision-lines/style.json](../../metrics/integration/render-tests/debug/collision-lines/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/debug/collision-overscaled/style.json](../../metrics/integration/render-tests/debug/collision-overscaled/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/debug/collision-pitched-wrapped/style.json](../../metrics/integration/render-tests/debug/collision-pitched-wrapped/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/debug/collision-pitched/style.json](../../metrics/integration/render-tests/debug/collision-pitched/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/debug/collision/style.json](../../metrics/integration/render-tests/debug/collision/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/debug/overdraw/style.json](../../metrics/integration/render-tests/debug/overdraw/style.json) | background, line | vector |
| [metrics/integration/render-tests/debug/padding/ease-to-btm-distort/style.json](../../metrics/integration/render-tests/debug/padding/ease-to-btm-distort/style.json) | background, line | geojson |
| [metrics/integration/render-tests/debug/padding/ease-to-left-distort/style.json](../../metrics/integration/render-tests/debug/padding/ease-to-left-distort/style.json) | background, line | geojson |
| [metrics/integration/render-tests/debug/padding/ease-to-no-distort/style.json](../../metrics/integration/render-tests/debug/padding/ease-to-no-distort/style.json) | background, line | geojson |
| [metrics/integration/render-tests/debug/padding/ease-to-right-distort/style.json](../../metrics/integration/render-tests/debug/padding/ease-to-right-distort/style.json) | background, line | geojson |
| [metrics/integration/render-tests/debug/padding/ease-to-top-distort/style.json](../../metrics/integration/render-tests/debug/padding/ease-to-top-distort/style.json) | background, line | geojson |
| [metrics/integration/render-tests/debug/padding/set-padding/style.json](../../metrics/integration/render-tests/debug/padding/set-padding/style.json) | background, line | geojson |
| [metrics/integration/render-tests/debug/raster/style.json](../../metrics/integration/render-tests/debug/raster/style.json) | background, raster | raster |
| [metrics/integration/render-tests/debug/tile-overscaled/style.json](../../metrics/integration/render-tests/debug/tile-overscaled/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/debug/tile/style.json](../../metrics/integration/render-tests/debug/tile/style.json) | background, raster, symbol | raster, vector |
| [metrics/integration/render-tests/empty/empty/style.json](../../metrics/integration/render-tests/empty/empty/style.json) | — | — |
| [metrics/integration/render-tests/extent/1024-circle/style.json](../../metrics/integration/render-tests/extent/1024-circle/style.json) | background, circle | vector |
| [metrics/integration/render-tests/extent/1024-fill/style.json](../../metrics/integration/render-tests/extent/1024-fill/style.json) | background, fill | vector |
| [metrics/integration/render-tests/extent/1024-line/style.json](../../metrics/integration/render-tests/extent/1024-line/style.json) | background, line | vector |
| [metrics/integration/render-tests/extent/1024-symbol/style.json](../../metrics/integration/render-tests/extent/1024-symbol/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/feature-state/composite-expression/style.json](../../metrics/integration/render-tests/feature-state/composite-expression/style.json) | circle | geojson |
| [metrics/integration/render-tests/feature-state/data-expression/style.json](../../metrics/integration/render-tests/feature-state/data-expression/style.json) | circle | geojson |
| [metrics/integration/render-tests/feature-state/promote-id-circle/style.json](../../metrics/integration/render-tests/feature-state/promote-id-circle/style.json) | circle | geojson |
| [metrics/integration/render-tests/feature-state/promote-id-fill-extrusion/style.json](../../metrics/integration/render-tests/feature-state/promote-id-fill-extrusion/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/feature-state/promote-id-fill/style.json](../../metrics/integration/render-tests/feature-state/promote-id-fill/style.json) | fill | geojson |
| [metrics/integration/render-tests/feature-state/promote-id-line/style.json](../../metrics/integration/render-tests/feature-state/promote-id-line/style.json) | line | geojson |
| [metrics/integration/render-tests/feature-state/promote-id-symbol/style.json](../../metrics/integration/render-tests/feature-state/promote-id-symbol/style.json) | symbol | geojson |
| [metrics/integration/render-tests/feature-state/symbol-paint/style.json](../../metrics/integration/render-tests/feature-state/symbol-paint/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/feature-state/vector-source/style.json](../../metrics/integration/render-tests/feature-state/vector-source/style.json) | background, circle | vector |
| [metrics/integration/render-tests/fill-antialias/false/style.json](../../metrics/integration/render-tests/fill-antialias/false/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-color/default/style.json](../../metrics/integration/render-tests/fill-color/default/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-color/function/style.json](../../metrics/integration/render-tests/fill-color/function/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-color/literal/style.json](../../metrics/integration/render-tests/fill-color/literal/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-color/multiply/style.json](../../metrics/integration/render-tests/fill-color/multiply/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-color/opacity/style.json](../../metrics/integration/render-tests/fill-color/opacity/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-color/property-function/style.json](../../metrics/integration/render-tests/fill-color/property-function/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-color/zoom-and-property-function/style.json](../../metrics/integration/render-tests/fill-color/zoom-and-property-function/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-extrusion-base/default/style.json](../../metrics/integration/render-tests/fill-extrusion-base/default/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-base/function/style.json](../../metrics/integration/render-tests/fill-extrusion-base/function/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-base/literal/style.json](../../metrics/integration/render-tests/fill-extrusion-base/literal/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-base/negative/style.json](../../metrics/integration/render-tests/fill-extrusion-base/negative/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-base/property-function/style.json](../../metrics/integration/render-tests/fill-extrusion-base/property-function/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-base/zoom-and-property-function/style.json](../../metrics/integration/render-tests/fill-extrusion-base/zoom-and-property-function/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-color/default/style.json](../../metrics/integration/render-tests/fill-extrusion-color/default/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-color/function/style.json](../../metrics/integration/render-tests/fill-extrusion-color/function/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-color/literal/style.json](../../metrics/integration/render-tests/fill-extrusion-color/literal/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-color/no-alpha-no-multiply/style.json](../../metrics/integration/render-tests/fill-extrusion-color/no-alpha-no-multiply/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-color/property-function/style.json](../../metrics/integration/render-tests/fill-extrusion-color/property-function/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-color/zoom-and-property-function/style.json](../../metrics/integration/render-tests/fill-extrusion-color/zoom-and-property-function/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-geometry/linestring/style.json](../../metrics/integration/render-tests/fill-extrusion-geometry/linestring/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-height/default/style.json](../../metrics/integration/render-tests/fill-extrusion-height/default/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-height/function/style.json](../../metrics/integration/render-tests/fill-extrusion-height/function/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-height/negative/style.json](../../metrics/integration/render-tests/fill-extrusion-height/negative/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-height/property-function/style.json](../../metrics/integration/render-tests/fill-extrusion-height/property-function/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-height/zoom-and-property-function/style.json](../../metrics/integration/render-tests/fill-extrusion-height/zoom-and-property-function/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-multiple/interleaved-layers/style.json](../../metrics/integration/render-tests/fill-extrusion-multiple/interleaved-layers/style.json) | circle, fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-multiple/multiple/style.json](../../metrics/integration/render-tests/fill-extrusion-multiple/multiple/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-opacity/default/style.json](../../metrics/integration/render-tests/fill-extrusion-opacity/default/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-opacity/function/style.json](../../metrics/integration/render-tests/fill-extrusion-opacity/function/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-opacity/literal/style.json](../../metrics/integration/render-tests/fill-extrusion-opacity/literal/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-pattern/@2x/style.json](../../metrics/integration/render-tests/fill-extrusion-pattern/@2x/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-pattern/feature-expression/style.json](../../metrics/integration/render-tests/fill-extrusion-pattern/feature-expression/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-pattern/function-2/style.json](../../metrics/integration/render-tests/fill-extrusion-pattern/function-2/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-pattern/function/style.json](../../metrics/integration/render-tests/fill-extrusion-pattern/function/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-pattern/literal/style.json](../../metrics/integration/render-tests/fill-extrusion-pattern/literal/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-pattern/missing/style.json](../../metrics/integration/render-tests/fill-extrusion-pattern/missing/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-pattern/opacity/style.json](../../metrics/integration/render-tests/fill-extrusion-pattern/opacity/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-pattern/tile-buffer/style.json](../../metrics/integration/render-tests/fill-extrusion-pattern/tile-buffer/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-translate-anchor/map/style.json](../../metrics/integration/render-tests/fill-extrusion-translate-anchor/map/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-translate-anchor/viewport/style.json](../../metrics/integration/render-tests/fill-extrusion-translate-anchor/viewport/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-translate/default/style.json](../../metrics/integration/render-tests/fill-extrusion-translate/default/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-translate/function/style.json](../../metrics/integration/render-tests/fill-extrusion-translate/function/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-translate/literal-opacity/style.json](../../metrics/integration/render-tests/fill-extrusion-translate/literal-opacity/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-translate/literal/style.json](../../metrics/integration/render-tests/fill-extrusion-translate/literal/style.json) | fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-vertical-gradient/default/style.json](../../metrics/integration/render-tests/fill-extrusion-vertical-gradient/default/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-extrusion-vertical-gradient/false/style.json](../../metrics/integration/render-tests/fill-extrusion-vertical-gradient/false/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/fill-opacity/default/style.json](../../metrics/integration/render-tests/fill-opacity/default/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-opacity/function/style.json](../../metrics/integration/render-tests/fill-opacity/function/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-opacity/literal/style.json](../../metrics/integration/render-tests/fill-opacity/literal/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-opacity/opaque-fill-over-symbol-layer/style.json](../../metrics/integration/render-tests/fill-opacity/opaque-fill-over-symbol-layer/style.json) | fill, symbol | geojson |
| [metrics/integration/render-tests/fill-opacity/overlapping/style.json](../../metrics/integration/render-tests/fill-opacity/overlapping/style.json) | background, fill | geojson |
| [metrics/integration/render-tests/fill-opacity/property-function-pattern/style.json](../../metrics/integration/render-tests/fill-opacity/property-function-pattern/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-opacity/property-function/style.json](../../metrics/integration/render-tests/fill-opacity/property-function/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-opacity/zoom-and-property-function-pattern/style.json](../../metrics/integration/render-tests/fill-opacity/zoom-and-property-function-pattern/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-opacity/zoom-and-property-function/style.json](../../metrics/integration/render-tests/fill-opacity/zoom-and-property-function/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-outline-color/default/style.json](../../metrics/integration/render-tests/fill-outline-color/default/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-outline-color/fill/style.json](../../metrics/integration/render-tests/fill-outline-color/fill/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-outline-color/function/style.json](../../metrics/integration/render-tests/fill-outline-color/function/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-outline-color/literal/style.json](../../metrics/integration/render-tests/fill-outline-color/literal/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-outline-color/multiply/style.json](../../metrics/integration/render-tests/fill-outline-color/multiply/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-outline-color/opacity/style.json](../../metrics/integration/render-tests/fill-outline-color/opacity/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-outline-color/property-function/style.json](../../metrics/integration/render-tests/fill-outline-color/property-function/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-outline-color/zoom-and-property-function/style.json](../../metrics/integration/render-tests/fill-outline-color/zoom-and-property-function/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-pattern/@2x/style.json](../../metrics/integration/render-tests/fill-pattern/@2x/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-pattern/case-data-expression/style.json](../../metrics/integration/render-tests/fill-pattern/case-data-expression/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-pattern/invalid-feature-expression/style.json](../../metrics/integration/render-tests/fill-pattern/invalid-feature-expression/style.json) | background, fill | geojson |
| [metrics/integration/render-tests/fill-pattern/literal/style.json](../../metrics/integration/render-tests/fill-pattern/literal/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-pattern/missing/style.json](../../metrics/integration/render-tests/fill-pattern/missing/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-pattern/opacity/style.json](../../metrics/integration/render-tests/fill-pattern/opacity/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-pattern/uneven-pattern/style.json](../../metrics/integration/render-tests/fill-pattern/uneven-pattern/style.json) | background, fill | vector |
| [metrics/integration/render-tests/fill-pattern/update-feature-state/style.json](../../metrics/integration/render-tests/fill-pattern/update-feature-state/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-pattern/wrapping-with-interpolation/style.json](../../metrics/integration/render-tests/fill-pattern/wrapping-with-interpolation/style.json) | background, fill | vector |
| [metrics/integration/render-tests/fill-pattern/zoomed/style.json](../../metrics/integration/render-tests/fill-pattern/zoomed/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-sort-key/literal/style.json](../../metrics/integration/render-tests/fill-sort-key/literal/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-translate-anchor/map/style.json](../../metrics/integration/render-tests/fill-translate-anchor/map/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-translate-anchor/viewport/style.json](../../metrics/integration/render-tests/fill-translate-anchor/viewport/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-translate/default/style.json](../../metrics/integration/render-tests/fill-translate/default/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-translate/function/style.json](../../metrics/integration/render-tests/fill-translate/function/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-translate/literal/style.json](../../metrics/integration/render-tests/fill-translate/literal/style.json) | fill | geojson |
| [metrics/integration/render-tests/fill-visibility/none/style.json](../../metrics/integration/render-tests/fill-visibility/none/style.json) | background, fill | vector |
| [metrics/integration/render-tests/fill-visibility/visible/style.json](../../metrics/integration/render-tests/fill-visibility/visible/style.json) | background, fill | vector |
| [metrics/integration/render-tests/filter/equality/style.json](../../metrics/integration/render-tests/filter/equality/style.json) | circle | geojson |
| [metrics/integration/render-tests/filter/in/style.json](../../metrics/integration/render-tests/filter/in/style.json) | fill | geojson |
| [metrics/integration/render-tests/filter/legacy-equality/style.json](../../metrics/integration/render-tests/filter/legacy-equality/style.json) | circle | geojson |
| [metrics/integration/render-tests/filter/none/style.json](../../metrics/integration/render-tests/filter/none/style.json) | circle | geojson |
| [metrics/integration/render-tests/geojson/clustered-properties/style.json](../../metrics/integration/render-tests/geojson/clustered-properties/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/geojson/clustered/style.json](../../metrics/integration/render-tests/geojson/clustered/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/geojson/external-feature/style.json](../../metrics/integration/render-tests/geojson/external-feature/style.json) | line | geojson |
| [metrics/integration/render-tests/geojson/external-invalid/style.json](../../metrics/integration/render-tests/geojson/external-invalid/style.json) | line | geojson |
| [metrics/integration/render-tests/geojson/external-linestring/style.json](../../metrics/integration/render-tests/geojson/external-linestring/style.json) | line | geojson |
| [metrics/integration/render-tests/geojson/external-malformed/style.json](../../metrics/integration/render-tests/geojson/external-malformed/style.json) | line | geojson |
| [metrics/integration/render-tests/geojson/inconsistent-winding-order/style.json](../../metrics/integration/render-tests/geojson/inconsistent-winding-order/style.json) | fill | geojson |
| [metrics/integration/render-tests/geojson/inline-feature/style.json](../../metrics/integration/render-tests/geojson/inline-feature/style.json) | line | geojson |
| [metrics/integration/render-tests/geojson/inline-invalid/style.json](../../metrics/integration/render-tests/geojson/inline-invalid/style.json) | line | geojson |
| [metrics/integration/render-tests/geojson/inline-linestring-circle/style.json](../../metrics/integration/render-tests/geojson/inline-linestring-circle/style.json) | circle | geojson |
| [metrics/integration/render-tests/geojson/inline-linestring-fill/style.json](../../metrics/integration/render-tests/geojson/inline-linestring-fill/style.json) | fill | geojson |
| [metrics/integration/render-tests/geojson/inline-linestring-line/style.json](../../metrics/integration/render-tests/geojson/inline-linestring-line/style.json) | line | geojson |
| [metrics/integration/render-tests/geojson/inline-linestring-symbol/style.json](../../metrics/integration/render-tests/geojson/inline-linestring-symbol/style.json) | symbol | geojson |
| [metrics/integration/render-tests/geojson/inline-malformed/style.json](../../metrics/integration/render-tests/geojson/inline-malformed/style.json) | line | geojson |
| [metrics/integration/render-tests/geojson/inline-point-circle/style.json](../../metrics/integration/render-tests/geojson/inline-point-circle/style.json) | circle | geojson |
| [metrics/integration/render-tests/geojson/inline-point-fill/style.json](../../metrics/integration/render-tests/geojson/inline-point-fill/style.json) | fill | geojson |
| [metrics/integration/render-tests/geojson/inline-point-line/style.json](../../metrics/integration/render-tests/geojson/inline-point-line/style.json) | line | geojson |
| [metrics/integration/render-tests/geojson/inline-point-symbol/style.json](../../metrics/integration/render-tests/geojson/inline-point-symbol/style.json) | symbol | geojson |
| [metrics/integration/render-tests/geojson/inline-polygon-circle/style.json](../../metrics/integration/render-tests/geojson/inline-polygon-circle/style.json) | circle | geojson |
| [metrics/integration/render-tests/geojson/inline-polygon-fill/style.json](../../metrics/integration/render-tests/geojson/inline-polygon-fill/style.json) | fill | geojson |
| [metrics/integration/render-tests/geojson/inline-polygon-line/style.json](../../metrics/integration/render-tests/geojson/inline-polygon-line/style.json) | line | geojson |
| [metrics/integration/render-tests/geojson/inline-polygon-symbol/style.json](../../metrics/integration/render-tests/geojson/inline-polygon-symbol/style.json) | symbol | geojson |
| [metrics/integration/render-tests/geojson/missing/style.json](../../metrics/integration/render-tests/geojson/missing/style.json) | line | geojson |
| [metrics/integration/render-tests/geojson/reparse-overscaled/style.json](../../metrics/integration/render-tests/geojson/reparse-overscaled/style.json) | line | geojson |
| [metrics/integration/render-tests/heatmap-color/default/style.json](../../metrics/integration/render-tests/heatmap-color/default/style.json) | background, heatmap | vector |
| [metrics/integration/render-tests/heatmap-color/expression/style.json](../../metrics/integration/render-tests/heatmap-color/expression/style.json) | background, heatmap | vector |
| [metrics/integration/render-tests/heatmap-intensity/default/style.json](../../metrics/integration/render-tests/heatmap-intensity/default/style.json) | background, heatmap | vector |
| [metrics/integration/render-tests/heatmap-intensity/function/style.json](../../metrics/integration/render-tests/heatmap-intensity/function/style.json) | background, heatmap | vector |
| [metrics/integration/render-tests/heatmap-intensity/literal/style.json](../../metrics/integration/render-tests/heatmap-intensity/literal/style.json) | background, heatmap | vector |
| [metrics/integration/render-tests/heatmap-opacity/default/style.json](../../metrics/integration/render-tests/heatmap-opacity/default/style.json) | background, heatmap | vector |
| [metrics/integration/render-tests/heatmap-opacity/function/style.json](../../metrics/integration/render-tests/heatmap-opacity/function/style.json) | background, heatmap | vector |
| [metrics/integration/render-tests/heatmap-opacity/literal/style.json](../../metrics/integration/render-tests/heatmap-opacity/literal/style.json) | background, heatmap | vector |
| [metrics/integration/render-tests/heatmap-radius/antimeridian/style.json](../../metrics/integration/render-tests/heatmap-radius/antimeridian/style.json) | heatmap | geojson |
| [metrics/integration/render-tests/heatmap-radius/data-expression/style.json](../../metrics/integration/render-tests/heatmap-radius/data-expression/style.json) | heatmap | geojson |
| [metrics/integration/render-tests/heatmap-radius/default/style.json](../../metrics/integration/render-tests/heatmap-radius/default/style.json) | background, heatmap | vector |
| [metrics/integration/render-tests/heatmap-radius/function/style.json](../../metrics/integration/render-tests/heatmap-radius/function/style.json) | background, heatmap | vector |
| [metrics/integration/render-tests/heatmap-radius/literal/style.json](../../metrics/integration/render-tests/heatmap-radius/literal/style.json) | background, heatmap | vector |
| [metrics/integration/render-tests/heatmap-radius/pitch30/style.json](../../metrics/integration/render-tests/heatmap-radius/pitch30/style.json) | background, heatmap | vector |
| [metrics/integration/render-tests/heatmap-weight/default/style.json](../../metrics/integration/render-tests/heatmap-weight/default/style.json) | background, heatmap | vector |
| [metrics/integration/render-tests/heatmap-weight/identity-property-function/style.json](../../metrics/integration/render-tests/heatmap-weight/identity-property-function/style.json) | background, heatmap | vector |
| [metrics/integration/render-tests/heatmap-weight/literal/style.json](../../metrics/integration/render-tests/heatmap-weight/literal/style.json) | background, heatmap | vector |
| [metrics/integration/render-tests/hillshade-accent-color/default/style.json](../../metrics/integration/render-tests/hillshade-accent-color/default/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-accent-color/literal/style.json](../../metrics/integration/render-tests/hillshade-accent-color/literal/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-accent-color/terrarium/style.json](../../metrics/integration/render-tests/hillshade-accent-color/terrarium/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-accent-color/zoom-function/style.json](../../metrics/integration/render-tests/hillshade-accent-color/zoom-function/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-alt/basic-0/style.json](../../metrics/integration/render-tests/hillshade-alt/basic-0/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-alt/basic-90/style.json](../../metrics/integration/render-tests/hillshade-alt/basic-90/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-alt/combined-60/style.json](../../metrics/integration/render-tests/hillshade-alt/combined-60/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-highlight-color/default/style.json](../../metrics/integration/render-tests/hillshade-highlight-color/default/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-highlight-color/literal/style.json](../../metrics/integration/render-tests/hillshade-highlight-color/literal/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-highlight-color/zoom-function/style.json](../../metrics/integration/render-tests/hillshade-highlight-color/zoom-function/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-low-zoom/basic/style.json](../../metrics/integration/render-tests/hillshade-low-zoom/basic/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-low-zoom/combined/style.json](../../metrics/integration/render-tests/hillshade-low-zoom/combined/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-low-zoom/default/style.json](../../metrics/integration/render-tests/hillshade-low-zoom/default/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-low-zoom/igor/style.json](../../metrics/integration/render-tests/hillshade-low-zoom/igor/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-low-zoom/multidirectional/style.json](../../metrics/integration/render-tests/hillshade-low-zoom/multidirectional/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-low-zoom/standard/style.json](../../metrics/integration/render-tests/hillshade-low-zoom/standard/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-maxzoom/default/style.json](../../metrics/integration/render-tests/hillshade-maxzoom/default/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-maxzoom/overzoom/style.json](../../metrics/integration/render-tests/hillshade-maxzoom/overzoom/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-methods/basic/style.json](../../metrics/integration/render-tests/hillshade-methods/basic/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-methods/combined/style.json](../../metrics/integration/render-tests/hillshade-methods/combined/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-methods/igor/style.json](../../metrics/integration/render-tests/hillshade-methods/igor/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-methods/multidirectional/style.json](../../metrics/integration/render-tests/hillshade-methods/multidirectional/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-shadow-color/default/style.json](../../metrics/integration/render-tests/hillshade-shadow-color/default/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-shadow-color/literal/style.json](../../metrics/integration/render-tests/hillshade-shadow-color/literal/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/hillshade-shadow-color/zoom-function/style.json](../../metrics/integration/render-tests/hillshade-shadow-color/zoom-function/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/icon-anchor/bottom-left/style.json](../../metrics/integration/render-tests/icon-anchor/bottom-left/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-anchor/bottom-right/style.json](../../metrics/integration/render-tests/icon-anchor/bottom-right/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-anchor/bottom/style.json](../../metrics/integration/render-tests/icon-anchor/bottom/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-anchor/center/style.json](../../metrics/integration/render-tests/icon-anchor/center/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-anchor/default/style.json](../../metrics/integration/render-tests/icon-anchor/default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-anchor/left/style.json](../../metrics/integration/render-tests/icon-anchor/left/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-anchor/property-function/style.json](../../metrics/integration/render-tests/icon-anchor/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-anchor/right/style.json](../../metrics/integration/render-tests/icon-anchor/right/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-anchor/top-left/style.json](../../metrics/integration/render-tests/icon-anchor/top-left/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-anchor/top-right/style.json](../../metrics/integration/render-tests/icon-anchor/top-right/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-anchor/top/style.json](../../metrics/integration/render-tests/icon-anchor/top/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-color/default/style.json](../../metrics/integration/render-tests/icon-color/default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-color/function/style.json](../../metrics/integration/render-tests/icon-color/function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-color/literal/style.json](../../metrics/integration/render-tests/icon-color/literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-color/property-function/style.json](../../metrics/integration/render-tests/icon-color/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-halo-blur/default/style.json](../../metrics/integration/render-tests/icon-halo-blur/default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-halo-blur/function/style.json](../../metrics/integration/render-tests/icon-halo-blur/function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-halo-blur/literal/style.json](../../metrics/integration/render-tests/icon-halo-blur/literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-halo-blur/property-function/style.json](../../metrics/integration/render-tests/icon-halo-blur/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-halo-color/default/style.json](../../metrics/integration/render-tests/icon-halo-color/default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-halo-color/function/style.json](../../metrics/integration/render-tests/icon-halo-color/function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-halo-color/literal/style.json](../../metrics/integration/render-tests/icon-halo-color/literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-halo-color/multiply/style.json](../../metrics/integration/render-tests/icon-halo-color/multiply/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-halo-color/opacity/style.json](../../metrics/integration/render-tests/icon-halo-color/opacity/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-halo-color/property-function/style.json](../../metrics/integration/render-tests/icon-halo-color/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-halo-color/transparent/style.json](../../metrics/integration/render-tests/icon-halo-color/transparent/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-halo-width/default/style.json](../../metrics/integration/render-tests/icon-halo-width/default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-halo-width/function/style.json](../../metrics/integration/render-tests/icon-halo-width/function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-halo-width/literal/style.json](../../metrics/integration/render-tests/icon-halo-width/literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-halo-width/property-function/style.json](../../metrics/integration/render-tests/icon-halo-width/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-image/icon-sdf-non-sdf-one-layer/style.json](../../metrics/integration/render-tests/icon-image/icon-sdf-non-sdf-one-layer/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/icon-image/image-expression/style.json](../../metrics/integration/render-tests/icon-image/image-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-image/literal/style.json](../../metrics/integration/render-tests/icon-image/literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-image/property-function/style.json](../../metrics/integration/render-tests/icon-image/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-image/stretchable-content/style.json](../../metrics/integration/render-tests/icon-image/stretchable-content/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-image/stretchable/style.json](../../metrics/integration/render-tests/icon-image/stretchable/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-image/token/style.json](../../metrics/integration/render-tests/icon-image/token/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-no-cross-source-collision/default/style.json](../../metrics/integration/render-tests/icon-no-cross-source-collision/default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-offset/literal/style.json](../../metrics/integration/render-tests/icon-offset/literal/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-offset/pitched-offset/style.json](../../metrics/integration/render-tests/icon-offset/pitched-offset/style.json) | background, line, symbol | geojson |
| [metrics/integration/render-tests/icon-offset/property-function/style.json](../../metrics/integration/render-tests/icon-offset/property-function/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-offset/zoom-and-property-function/style.json](../../metrics/integration/render-tests/icon-offset/zoom-and-property-function/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-opacity/default/style.json](../../metrics/integration/render-tests/icon-opacity/default/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-opacity/function/style.json](../../metrics/integration/render-tests/icon-opacity/function/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-opacity/icon-only/style.json](../../metrics/integration/render-tests/icon-opacity/icon-only/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-opacity/literal/style.json](../../metrics/integration/render-tests/icon-opacity/literal/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-opacity/property-function/style.json](../../metrics/integration/render-tests/icon-opacity/property-function/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/icon-opacity/text-and-icon/style.json](../../metrics/integration/render-tests/icon-opacity/text-and-icon/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-opacity/text-only/style.json](../../metrics/integration/render-tests/icon-opacity/text-only/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-padding/databind/style.json](../../metrics/integration/render-tests/icon-padding/databind/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/icon-padding/default/style.json](../../metrics/integration/render-tests/icon-padding/default/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/icon-pitch-alignment/auto-rotation-alignment-map/style.json](../../metrics/integration/render-tests/icon-pitch-alignment/auto-rotation-alignment-map/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-pitch-alignment/auto-rotation-alignment-viewport/style.json](../../metrics/integration/render-tests/icon-pitch-alignment/auto-rotation-alignment-viewport/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-pitch-alignment/map-rotation-alignment-viewport/style.json](../../metrics/integration/render-tests/icon-pitch-alignment/map-rotation-alignment-viewport/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-pitch-alignment/viewport-rotation-alignment-map/style.json](../../metrics/integration/render-tests/icon-pitch-alignment/viewport-rotation-alignment-map/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-pitch-scaling/rotation-alignment-map/style.json](../../metrics/integration/render-tests/icon-pitch-scaling/rotation-alignment-map/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-pitch-scaling/rotation-alignment-viewport/style.json](../../metrics/integration/render-tests/icon-pitch-scaling/rotation-alignment-viewport/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-pixelratio-mismatch/default/style.json](../../metrics/integration/render-tests/icon-pixelratio-mismatch/default/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-roll-alignment/auto-rotation-alignment-map/style.json](../../metrics/integration/render-tests/icon-roll-alignment/auto-rotation-alignment-map/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-roll-alignment/auto-rotation-alignment-viewport/style.json](../../metrics/integration/render-tests/icon-roll-alignment/auto-rotation-alignment-viewport/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-roll-alignment/map-rotation-alignment-viewport/style.json](../../metrics/integration/render-tests/icon-roll-alignment/map-rotation-alignment-viewport/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-roll-alignment/viewport-rotation-alignment-map/style.json](../../metrics/integration/render-tests/icon-roll-alignment/viewport-rotation-alignment-map/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-rotate/literal/style.json](../../metrics/integration/render-tests/icon-rotate/literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-rotate/property-function/style.json](../../metrics/integration/render-tests/icon-rotate/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-rotate/with-offset/style.json](../../metrics/integration/render-tests/icon-rotate/with-offset/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/icon-rotation-alignment/auto-symbol-placement-line/style.json](../../metrics/integration/render-tests/icon-rotation-alignment/auto-symbol-placement-line/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-rotation-alignment/auto-symbol-placement-point/style.json](../../metrics/integration/render-tests/icon-rotation-alignment/auto-symbol-placement-point/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-rotation-alignment/map-symbol-placement-line/style.json](../../metrics/integration/render-tests/icon-rotation-alignment/map-symbol-placement-line/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-rotation-alignment/map-symbol-placement-point/style.json](../../metrics/integration/render-tests/icon-rotation-alignment/map-symbol-placement-point/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-rotation-alignment/viewport-symbol-placement-line/style.json](../../metrics/integration/render-tests/icon-rotation-alignment/viewport-symbol-placement-line/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-rotation-alignment/viewport-symbol-placement-point/style.json](../../metrics/integration/render-tests/icon-rotation-alignment/viewport-symbol-placement-point/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-size/camera-function-high-base-plain/style.json](../../metrics/integration/render-tests/icon-size/camera-function-high-base-plain/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-size/camera-function-high-base-sdf/style.json](../../metrics/integration/render-tests/icon-size/camera-function-high-base-sdf/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-size/camera-function-plain/style.json](../../metrics/integration/render-tests/icon-size/camera-function-plain/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-size/camera-function-sdf/style.json](../../metrics/integration/render-tests/icon-size/camera-function-sdf/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-size/composite-function-plain/style.json](../../metrics/integration/render-tests/icon-size/composite-function-plain/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-size/composite-function-sdf/style.json](../../metrics/integration/render-tests/icon-size/composite-function-sdf/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-size/default/style.json](../../metrics/integration/render-tests/icon-size/default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-size/function/style.json](../../metrics/integration/render-tests/icon-size/function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-size/literal/style.json](../../metrics/integration/render-tests/icon-size/literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-size/property-function-plain/style.json](../../metrics/integration/render-tests/icon-size/property-function-plain/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-size/property-function-sdf/style.json](../../metrics/integration/render-tests/icon-size/property-function-sdf/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/both-collision-variable-anchor-text-fit/style.json](../../metrics/integration/render-tests/icon-text-fit/both-collision-variable-anchor-text-fit/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/both-collision-variable-anchor/style.json](../../metrics/integration/render-tests/icon-text-fit/both-collision-variable-anchor/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/both-collision/style.json](../../metrics/integration/render-tests/icon-text-fit/both-collision/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/both-padding/style.json](../../metrics/integration/render-tests/icon-text-fit/both-padding/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/both-text-anchor-1x-image-2x-screen/style.json](../../metrics/integration/render-tests/icon-text-fit/both-text-anchor-1x-image-2x-screen/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/both-text-anchor-2x-image-1x-screen/style.json](../../metrics/integration/render-tests/icon-text-fit/both-text-anchor-2x-image-1x-screen/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/both-text-anchor-2x-image-2x-screen/style.json](../../metrics/integration/render-tests/icon-text-fit/both-text-anchor-2x-image-2x-screen/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/both-text-anchor-icon-anchor/style.json](../../metrics/integration/render-tests/icon-text-fit/both-text-anchor-icon-anchor/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/both-text-anchor-icon-offset/style.json](../../metrics/integration/render-tests/icon-text-fit/both-text-anchor-icon-offset/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/both-text-anchor-padding/style.json](../../metrics/integration/render-tests/icon-text-fit/both-text-anchor-padding/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/both-text-anchor/style.json](../../metrics/integration/render-tests/icon-text-fit/both-text-anchor/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/both/style.json](../../metrics/integration/render-tests/icon-text-fit/both/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/enlargen-both-padding/style.json](../../metrics/integration/render-tests/icon-text-fit/enlargen-both-padding/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/enlargen-both/style.json](../../metrics/integration/render-tests/icon-text-fit/enlargen-both/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/enlargen-height/style.json](../../metrics/integration/render-tests/icon-text-fit/enlargen-height/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/enlargen-width/style.json](../../metrics/integration/render-tests/icon-text-fit/enlargen-width/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/height-padding/style.json](../../metrics/integration/render-tests/icon-text-fit/height-padding/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/height-text-anchor-padding/style.json](../../metrics/integration/render-tests/icon-text-fit/height-text-anchor-padding/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/height-text-anchor/style.json](../../metrics/integration/render-tests/icon-text-fit/height-text-anchor/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/height/style.json](../../metrics/integration/render-tests/icon-text-fit/height/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/none/style.json](../../metrics/integration/render-tests/icon-text-fit/none/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/placement-line/style.json](../../metrics/integration/render-tests/icon-text-fit/placement-line/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-text-fit/stretch-fifteen-part/style.json](../../metrics/integration/render-tests/icon-text-fit/stretch-fifteen-part/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/stretch-nine-part-@2x/style.json](../../metrics/integration/render-tests/icon-text-fit/stretch-nine-part-@2x/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/stretch-nine-part-content-collision/style.json](../../metrics/integration/render-tests/icon-text-fit/stretch-nine-part-content-collision/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/stretch-nine-part-content/style.json](../../metrics/integration/render-tests/icon-text-fit/stretch-nine-part-content/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/stretch-nine-part-just-height/style.json](../../metrics/integration/render-tests/icon-text-fit/stretch-nine-part-just-height/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/stretch-nine-part-just-width/style.json](../../metrics/integration/render-tests/icon-text-fit/stretch-nine-part-just-width/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/stretch-nine-part/style.json](../../metrics/integration/render-tests/icon-text-fit/stretch-nine-part/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/stretch-three-part/style.json](../../metrics/integration/render-tests/icon-text-fit/stretch-three-part/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/stretch-two-part/style.json](../../metrics/integration/render-tests/icon-text-fit/stretch-two-part/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/stretch-underscale/style.json](../../metrics/integration/render-tests/icon-text-fit/stretch-underscale/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/text-variable-anchor-overlap/style.json](../../metrics/integration/render-tests/icon-text-fit/text-variable-anchor-overlap/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-text-fit/text-variable-anchor/style.json](../../metrics/integration/render-tests/icon-text-fit/text-variable-anchor/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-text-fit/textFit-anchors-long/style.json](../../metrics/integration/render-tests/icon-text-fit/textFit-anchors-long/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/textFit-anchors-short/style.json](../../metrics/integration/render-tests/icon-text-fit/textFit-anchors-short/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/textFit-collision/style.json](../../metrics/integration/render-tests/icon-text-fit/textFit-collision/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/textFit-grid-long-vertical/style.json](../../metrics/integration/render-tests/icon-text-fit/textFit-grid-long-vertical/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/textFit-grid-long/style.json](../../metrics/integration/render-tests/icon-text-fit/textFit-grid-long/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/textFit-grid-short-vertical/style.json](../../metrics/integration/render-tests/icon-text-fit/textFit-grid-short-vertical/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/textFit-grid-short/style.json](../../metrics/integration/render-tests/icon-text-fit/textFit-grid-short/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/width-padding/style.json](../../metrics/integration/render-tests/icon-text-fit/width-padding/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/width-text-anchor-padding/style.json](../../metrics/integration/render-tests/icon-text-fit/width-text-anchor-padding/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/width-text-anchor/style.json](../../metrics/integration/render-tests/icon-text-fit/width-text-anchor/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/icon-text-fit/width/style.json](../../metrics/integration/render-tests/icon-text-fit/width/style.json) | symbol | geojson |
| [metrics/integration/render-tests/icon-translate-anchor/map/style.json](../../metrics/integration/render-tests/icon-translate-anchor/map/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-translate-anchor/viewport/style.json](../../metrics/integration/render-tests/icon-translate-anchor/viewport/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-translate/default/style.json](../../metrics/integration/render-tests/icon-translate/default/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-translate/function/style.json](../../metrics/integration/render-tests/icon-translate/function/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-translate/literal/style.json](../../metrics/integration/render-tests/icon-translate/literal/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-visibility/none/style.json](../../metrics/integration/render-tests/icon-visibility/none/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/icon-visibility/visible/style.json](../../metrics/integration/render-tests/icon-visibility/visible/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/image/default/style.json](../../metrics/integration/render-tests/image/default/style.json) | raster | image |
| [metrics/integration/render-tests/image/pitched/style.json](../../metrics/integration/render-tests/image/pitched/style.json) | raster | image |
| [metrics/integration/render-tests/image/raster-brightness/style.json](../../metrics/integration/render-tests/image/raster-brightness/style.json) | raster | image |
| [metrics/integration/render-tests/image/raster-contrast/style.json](../../metrics/integration/render-tests/image/raster-contrast/style.json) | raster | image |
| [metrics/integration/render-tests/image/raster-hue-rotate/style.json](../../metrics/integration/render-tests/image/raster-hue-rotate/style.json) | raster | image |
| [metrics/integration/render-tests/image/raster-opacity/style.json](../../metrics/integration/render-tests/image/raster-opacity/style.json) | raster | image |
| [metrics/integration/render-tests/image/raster-resampling/style.json](../../metrics/integration/render-tests/image/raster-resampling/style.json) | raster | image |
| [metrics/integration/render-tests/image/raster-saturation/style.json](../../metrics/integration/render-tests/image/raster-saturation/style.json) | raster | image |
| [metrics/integration/render-tests/image/raster-visibility/style.json](../../metrics/integration/render-tests/image/raster-visibility/style.json) | raster | image |
| [metrics/integration/render-tests/image/world-wrap/style.json](../../metrics/integration/render-tests/image/world-wrap/style.json) | background, raster | image |
| [metrics/integration/render-tests/is-supported-script/filter/style.json](../../metrics/integration/render-tests/is-supported-script/filter/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/is-supported-script/layout/style.json](../../metrics/integration/render-tests/is-supported-script/layout/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/line-blur/default/style.json](../../metrics/integration/render-tests/line-blur/default/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-blur/function/style.json](../../metrics/integration/render-tests/line-blur/function/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-blur/literal/style.json](../../metrics/integration/render-tests/line-blur/literal/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-blur/property-function/style.json](../../metrics/integration/render-tests/line-blur/property-function/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-cap/butt/style.json](../../metrics/integration/render-tests/line-cap/butt/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-cap/round/style.json](../../metrics/integration/render-tests/line-cap/round/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-cap/square/style.json](../../metrics/integration/render-tests/line-cap/square/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-color/default/style.json](../../metrics/integration/render-tests/line-color/default/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-color/function/style.json](../../metrics/integration/render-tests/line-color/function/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-color/literal/style.json](../../metrics/integration/render-tests/line-color/literal/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-color/property-function-identity/style.json](../../metrics/integration/render-tests/line-color/property-function-identity/style.json) | line | geojson |
| [metrics/integration/render-tests/line-color/property-function/style.json](../../metrics/integration/render-tests/line-color/property-function/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-dasharray/default/style.json](../../metrics/integration/render-tests/line-dasharray/default/style.json) | line | geojson |
| [metrics/integration/render-tests/line-dasharray/fractional-zoom/style.json](../../metrics/integration/render-tests/line-dasharray/fractional-zoom/style.json) | line | geojson |
| [metrics/integration/render-tests/line-dasharray/function/line-width-composite-function/style.json](../../metrics/integration/render-tests/line-dasharray/function/line-width-composite-function/style.json) | line | geojson |
| [metrics/integration/render-tests/line-dasharray/function/line-width-constant/style.json](../../metrics/integration/render-tests/line-dasharray/function/line-width-constant/style.json) | line | geojson |
| [metrics/integration/render-tests/line-dasharray/function/line-width-property-function/style.json](../../metrics/integration/render-tests/line-dasharray/function/line-width-property-function/style.json) | line | geojson |
| [metrics/integration/render-tests/line-dasharray/literal/line-width-composite-function/style.json](../../metrics/integration/render-tests/line-dasharray/literal/line-width-composite-function/style.json) | line | geojson |
| [metrics/integration/render-tests/line-dasharray/literal/line-width-constant/style.json](../../metrics/integration/render-tests/line-dasharray/literal/line-width-constant/style.json) | line | geojson |
| [metrics/integration/render-tests/line-dasharray/literal/line-width-property-function/style.json](../../metrics/integration/render-tests/line-dasharray/literal/line-width-property-function/style.json) | line | geojson |
| [metrics/integration/render-tests/line-dasharray/literal/line-width-zoom-function/style.json](../../metrics/integration/render-tests/line-dasharray/literal/line-width-zoom-function/style.json) | line | geojson |
| [metrics/integration/render-tests/line-dasharray/long-segment/style.json](../../metrics/integration/render-tests/line-dasharray/long-segment/style.json) | background, line | geojson |
| [metrics/integration/render-tests/line-dasharray/overscaled/style.json](../../metrics/integration/render-tests/line-dasharray/overscaled/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-dasharray/round/segments/style.json](../../metrics/integration/render-tests/line-dasharray/round/segments/style.json) | line | geojson |
| [metrics/integration/render-tests/line-dasharray/round/zero-gap-width/style.json](../../metrics/integration/render-tests/line-dasharray/round/zero-gap-width/style.json) | line | geojson |
| [metrics/integration/render-tests/line-dasharray/slant/style.json](../../metrics/integration/render-tests/line-dasharray/slant/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-dasharray/zero-length-gap/style.json](../../metrics/integration/render-tests/line-dasharray/zero-length-gap/style.json) | line | geojson |
| [metrics/integration/render-tests/line-dasharray/zoom-history/style.json](../../metrics/integration/render-tests/line-dasharray/zoom-history/style.json) | line | geojson |
| [metrics/integration/render-tests/line-gap-width/default/style.json](../../metrics/integration/render-tests/line-gap-width/default/style.json) | line | geojson |
| [metrics/integration/render-tests/line-gap-width/function/style.json](../../metrics/integration/render-tests/line-gap-width/function/style.json) | line | geojson |
| [metrics/integration/render-tests/line-gap-width/literal/style.json](../../metrics/integration/render-tests/line-gap-width/literal/style.json) | line | geojson |
| [metrics/integration/render-tests/line-gap-width/property-function/style.json](../../metrics/integration/render-tests/line-gap-width/property-function/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-gradient/gradient-tile-boundaries/style.json](../../metrics/integration/render-tests/line-gradient/gradient-tile-boundaries/style.json) | line | geojson |
| [metrics/integration/render-tests/line-gradient/gradient/style.json](../../metrics/integration/render-tests/line-gradient/gradient/style.json) | line | geojson |
| [metrics/integration/render-tests/line-gradient/translucent/style.json](../../metrics/integration/render-tests/line-gradient/translucent/style.json) | line | geojson |
| [metrics/integration/render-tests/line-join/bevel-transparent/style.json](../../metrics/integration/render-tests/line-join/bevel-transparent/style.json) | line | geojson |
| [metrics/integration/render-tests/line-join/bevel/style.json](../../metrics/integration/render-tests/line-join/bevel/style.json) | line | geojson |
| [metrics/integration/render-tests/line-join/default/style.json](../../metrics/integration/render-tests/line-join/default/style.json) | line | geojson |
| [metrics/integration/render-tests/line-join/miter-transparent/style.json](../../metrics/integration/render-tests/line-join/miter-transparent/style.json) | line | geojson |
| [metrics/integration/render-tests/line-join/miter/style.json](../../metrics/integration/render-tests/line-join/miter/style.json) | line | geojson |
| [metrics/integration/render-tests/line-join/property-function-dasharray/style.json](../../metrics/integration/render-tests/line-join/property-function-dasharray/style.json) | line | geojson |
| [metrics/integration/render-tests/line-join/property-function/style.json](../../metrics/integration/render-tests/line-join/property-function/style.json) | line | geojson |
| [metrics/integration/render-tests/line-join/round-transparent/style.json](../../metrics/integration/render-tests/line-join/round-transparent/style.json) | line | geojson |
| [metrics/integration/render-tests/line-join/round/style.json](../../metrics/integration/render-tests/line-join/round/style.json) | line | geojson |
| [metrics/integration/render-tests/line-offset/default/style.json](../../metrics/integration/render-tests/line-offset/default/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-offset/function/style.json](../../metrics/integration/render-tests/line-offset/function/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-offset/literal-negative/style.json](../../metrics/integration/render-tests/line-offset/literal-negative/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-offset/literal/style.json](../../metrics/integration/render-tests/line-offset/literal/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-offset/property-function/style.json](../../metrics/integration/render-tests/line-offset/property-function/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-opacity/default/style.json](../../metrics/integration/render-tests/line-opacity/default/style.json) | line | geojson |
| [metrics/integration/render-tests/line-opacity/function/style.json](../../metrics/integration/render-tests/line-opacity/function/style.json) | line | geojson |
| [metrics/integration/render-tests/line-opacity/literal/style.json](../../metrics/integration/render-tests/line-opacity/literal/style.json) | line | geojson |
| [metrics/integration/render-tests/line-opacity/property-function/style.json](../../metrics/integration/render-tests/line-opacity/property-function/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-opacity/step-curve/style.json](../../metrics/integration/render-tests/line-opacity/step-curve/style.json) | background, line | geojson |
| [metrics/integration/render-tests/line-pattern/@2x/style.json](../../metrics/integration/render-tests/line-pattern/@2x/style.json) | line | geojson |
| [metrics/integration/render-tests/line-pattern/literal/style.json](../../metrics/integration/render-tests/line-pattern/literal/style.json) | line | geojson |
| [metrics/integration/render-tests/line-pattern/mixed/style.json](../../metrics/integration/render-tests/line-pattern/mixed/style.json) | background, line | geojson |
| [metrics/integration/render-tests/line-pattern/opacity/style.json](../../metrics/integration/render-tests/line-pattern/opacity/style.json) | line | geojson |
| [metrics/integration/render-tests/line-pattern/overscaled/style.json](../../metrics/integration/render-tests/line-pattern/overscaled/style.json) | background, line | geojson |
| [metrics/integration/render-tests/line-pattern/pitch/style.json](../../metrics/integration/render-tests/line-pattern/pitch/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-pattern/property-function/style.json](../../metrics/integration/render-tests/line-pattern/property-function/style.json) | background, line | geojson |
| [metrics/integration/render-tests/line-pattern/step-curve/style.json](../../metrics/integration/render-tests/line-pattern/step-curve/style.json) | background, line | geojson |
| [metrics/integration/render-tests/line-pattern/with-dasharray/style.json](../../metrics/integration/render-tests/line-pattern/with-dasharray/style.json) | line | geojson |
| [metrics/integration/render-tests/line-pattern/zoom-expression/style.json](../../metrics/integration/render-tests/line-pattern/zoom-expression/style.json) | background, line | geojson |
| [metrics/integration/render-tests/line-pitch/default/style.json](../../metrics/integration/render-tests/line-pitch/default/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-pitch/pitch0/style.json](../../metrics/integration/render-tests/line-pitch/pitch0/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-pitch/pitch15/style.json](../../metrics/integration/render-tests/line-pitch/pitch15/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-pitch/pitch30/style.json](../../metrics/integration/render-tests/line-pitch/pitch30/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-pitch/pitchAndBearing/style.json](../../metrics/integration/render-tests/line-pitch/pitchAndBearing/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-sort-key/literal/style.json](../../metrics/integration/render-tests/line-sort-key/literal/style.json) | line | geojson |
| [metrics/integration/render-tests/line-translate-anchor/map/style.json](../../metrics/integration/render-tests/line-translate-anchor/map/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-translate-anchor/viewport/style.json](../../metrics/integration/render-tests/line-translate-anchor/viewport/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-translate/default/style.json](../../metrics/integration/render-tests/line-translate/default/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-translate/function/style.json](../../metrics/integration/render-tests/line-translate/function/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-translate/literal/style.json](../../metrics/integration/render-tests/line-translate/literal/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-triangulation/default/style.json](../../metrics/integration/render-tests/line-triangulation/default/style.json) | background, line | geojson |
| [metrics/integration/render-tests/line-triangulation/round/style.json](../../metrics/integration/render-tests/line-triangulation/round/style.json) | background, line | geojson |
| [metrics/integration/render-tests/line-visibility/none/style.json](../../metrics/integration/render-tests/line-visibility/none/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-visibility/visible/style.json](../../metrics/integration/render-tests/line-visibility/visible/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-width/default/style.json](../../metrics/integration/render-tests/line-width/default/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-width/function/style.json](../../metrics/integration/render-tests/line-width/function/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-width/literal/style.json](../../metrics/integration/render-tests/line-width/literal/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-width/property-function/style.json](../../metrics/integration/render-tests/line-width/property-function/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-width/very-overscaled/style.json](../../metrics/integration/render-tests/line-width/very-overscaled/style.json) | background, line | vector |
| [metrics/integration/render-tests/line-width/zero-width-function/style.json](../../metrics/integration/render-tests/line-width/zero-width-function/style.json) | line | geojson |
| [metrics/integration/render-tests/line-width/zero-width/style.json](../../metrics/integration/render-tests/line-width/zero-width/style.json) | line | geojson |
| [metrics/integration/render-tests/linear-filter-opacity-edge/literal/style.json](../../metrics/integration/render-tests/linear-filter-opacity-edge/literal/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/map-mode/static/style.json](../../metrics/integration/render-tests/map-mode/static/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/map-mode/tile-avoid-edges/style.json](../../metrics/integration/render-tests/map-mode/tile-avoid-edges/style.json) | — | — |
| [metrics/integration/render-tests/map-mode/tile/style.json](../../metrics/integration/render-tests/map-mode/tile/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/mixed-zoom/z10-z11/style.json](../../metrics/integration/render-tests/mixed-zoom/z10-z11/style.json) | — | — |
| [metrics/integration/render-tests/projection/axonometric-multiple/style.json](../../metrics/integration/render-tests/projection/axonometric-multiple/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/projection/axonometric/style.json](../../metrics/integration/render-tests/projection/axonometric/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/projection/perspective/style.json](../../metrics/integration/render-tests/projection/perspective/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/projection/skew/style.json](../../metrics/integration/render-tests/projection/skew/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/raster-alpha/default/style.json](../../metrics/integration/render-tests/raster-alpha/default/style.json) | background, raster | raster |
| [metrics/integration/render-tests/raster-brightness/default/style.json](../../metrics/integration/render-tests/raster-brightness/default/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-brightness/function/style.json](../../metrics/integration/render-tests/raster-brightness/function/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-brightness/literal/style.json](../../metrics/integration/render-tests/raster-brightness/literal/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-contrast/default/style.json](../../metrics/integration/render-tests/raster-contrast/default/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-contrast/function/style.json](../../metrics/integration/render-tests/raster-contrast/function/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-contrast/literal/style.json](../../metrics/integration/render-tests/raster-contrast/literal/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-extent/maxzoom/style.json](../../metrics/integration/render-tests/raster-extent/maxzoom/style.json) | background, raster | raster |
| [metrics/integration/render-tests/raster-extent/minzoom/style.json](../../metrics/integration/render-tests/raster-extent/minzoom/style.json) | background, raster | raster |
| [metrics/integration/render-tests/raster-hue-rotate/default/style.json](../../metrics/integration/render-tests/raster-hue-rotate/default/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-hue-rotate/function/style.json](../../metrics/integration/render-tests/raster-hue-rotate/function/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-hue-rotate/literal/style.json](../../metrics/integration/render-tests/raster-hue-rotate/literal/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-loading/missing/style.json](../../metrics/integration/render-tests/raster-loading/missing/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-masking/overlapping-vector/style.json](../../metrics/integration/render-tests/raster-masking/overlapping-vector/style.json) | background, fill, raster | geojson, raster |
| [metrics/integration/render-tests/raster-masking/overlapping-zoom/style.json](../../metrics/integration/render-tests/raster-masking/overlapping-zoom/style.json) | background, raster | raster |
| [metrics/integration/render-tests/raster-masking/overlapping/style.json](../../metrics/integration/render-tests/raster-masking/overlapping/style.json) | background, raster | raster |
| [metrics/integration/render-tests/raster-opacity/default/style.json](../../metrics/integration/render-tests/raster-opacity/default/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-opacity/function/style.json](../../metrics/integration/render-tests/raster-opacity/function/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-opacity/literal/style.json](../../metrics/integration/render-tests/raster-opacity/literal/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-resampling/default/style.json](../../metrics/integration/render-tests/raster-resampling/default/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-resampling/function/style.json](../../metrics/integration/render-tests/raster-resampling/function/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-resampling/literal/style.json](../../metrics/integration/render-tests/raster-resampling/literal/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-rotation/0/style.json](../../metrics/integration/render-tests/raster-rotation/0/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-rotation/180/style.json](../../metrics/integration/render-tests/raster-rotation/180/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-rotation/270/style.json](../../metrics/integration/render-tests/raster-rotation/270/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-rotation/45/style.json](../../metrics/integration/render-tests/raster-rotation/45/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-rotation/90/style.json](../../metrics/integration/render-tests/raster-rotation/90/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-saturation/default/style.json](../../metrics/integration/render-tests/raster-saturation/default/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-saturation/function/style.json](../../metrics/integration/render-tests/raster-saturation/function/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-saturation/literal/style.json](../../metrics/integration/render-tests/raster-saturation/literal/style.json) | raster | raster |
| [metrics/integration/render-tests/raster-visibility/none/style.json](../../metrics/integration/render-tests/raster-visibility/none/style.json) | background, raster | raster |
| [metrics/integration/render-tests/raster-visibility/visible/style.json](../../metrics/integration/render-tests/raster-visibility/visible/style.json) | background, raster | raster |
| [metrics/integration/render-tests/real-world/bangkok/style.json](../../metrics/integration/render-tests/real-world/bangkok/style.json) | — | — |
| [metrics/integration/render-tests/real-world/chicago/style.json](../../metrics/integration/render-tests/real-world/chicago/style.json) | — | — |
| [metrics/integration/render-tests/real-world/nepal/style.json](../../metrics/integration/render-tests/real-world/nepal/style.json) | — | — |
| [metrics/integration/render-tests/real-world/norway/style.json](../../metrics/integration/render-tests/real-world/norway/style.json) | — | — |
| [metrics/integration/render-tests/real-world/sanfrancisco/style.json](../../metrics/integration/render-tests/real-world/sanfrancisco/style.json) | — | — |
| [metrics/integration/render-tests/real-world/uruguay/style.json](../../metrics/integration/render-tests/real-world/uruguay/style.json) | — | — |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#2305/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%232305/style.json) | background, fill | vector |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#2467/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%232467/style.json) | raster | raster |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#2523/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%232523/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#2533/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%232533/style.json) | background, fill | vector |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#2534/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%232534/style.json) | background, fill | vector |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#2762/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%232762/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#2769/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%232769/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#2787/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%232787/style.json) | background | — |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#2846/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%232846/style.json) | fill | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#2929/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%232929/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3010/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233010/style.json) | raster | image |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3107/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233107/style.json) | fill | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3320/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233320/style.json) | fill | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3365/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233365/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3394/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233394/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3426/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233426/style.json) | line | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3548/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233548/style.json) | background, fill, fill-extrusion | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3612/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233612/style.json) | background, fill, symbol | vector |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3614/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233614/style.json) | raster | raster |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3623/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233623/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3633/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233633/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3682/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233682/style.json) | line | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3702/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233702/style.json) | fill | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3723/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233723/style.json) | raster | raster |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3819/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233819/style.json) | background, circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3903/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233903/style.json) | background | — |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3910/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233910/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#3949/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%233949/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#4124/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234124/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#4144/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234144/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#4146/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234146/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#4150/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234150/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#4172/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234172/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#4235/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234235/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#4550/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234550/style.json) | raster | image |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#4551/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234551/style.json) | raster | image |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#4564/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234564/style.json) | background, line | vector |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#4573/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234573/style.json) | raster | image |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#4579/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234579/style.json) | line | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#4605/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234605/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#4617/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234617/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#4647/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234647/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#4651/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234651/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#4860/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234860/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#4928/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%234928/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5171/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235171/style.json) | line | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5370/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235370/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5466/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235466/style.json) | background, fill, raster | geojson, raster |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5496/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235496/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5544/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235544/style.json) | background, line, symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5546/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235546/style.json) | background, line, symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5576/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235576/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5599/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235599/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5631/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235631/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5642/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235642/style.json) | fill | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5740/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235740/style.json) | fill | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5776/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235776/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5911/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235911/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5911a/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235911a/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5947/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235947/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5953/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235953/style.json) | fill | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5978/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235978/style.json) | line | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#5982/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%235982/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#6160/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%236160/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#6238/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%236238/style.json) | fill | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#6548/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%236548/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#6649/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%236649/style.json) | line | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#6655/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%236655/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#6660/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%236660/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#6706/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%236706/style.json) | background | — |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#6806/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%236806/style.json) | background, fill, heatmap | vector |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#6919/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%236919/style.json) | background, line, symbol | vector |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#7032/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%237032/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#7066/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%237066/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#7172/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%237172/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#7271/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%237271/style.json) | raster | image |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#7302/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%237302/style.json) | raster | canvas |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#7708/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%237708/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#8026/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%238026/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#8273/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%238273/style.json) | line | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#8817/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%238817/style.json) | fill | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-js#9009/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-js%239009/style.json) | background, line | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#10849/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%2310849/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#11451/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%2311451/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#11729/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%2311729/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#12812/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%2312812/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#14402/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%2314402/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#15139/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%2315139/style.json) | background, line, symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#3292/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%233292/style.json) | background, fill | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#5648/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%235648/style.json) | background, fill | vector |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#5701/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%235701/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#5754/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%235754/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#6063/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%236063/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#6233/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%236233/style.json) | line | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#6820/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%236820/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#6903/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%236903/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#7241/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%237241/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#7357/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%237357/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#7572/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%237572/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#7714/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%237714/style.json) | line | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#7792/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%237792/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#8078/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%238078/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#8303/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%238303/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#8460/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%238460/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#8505/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%238505/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#8871/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%238871/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#8952/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%238952/style.json) | background | — |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#9406/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%239406/style.json) | circle | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#9557/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%239557/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#9792/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%239792/style.json) | symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#9900/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%239900/style.json) | line, symbol | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#9976/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%239976/style.json) | background, fill, line | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-native#9979/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-native%239979/style.json) | line | geojson |
| [metrics/integration/render-tests/regressions/mapbox-gl-shaders#37/style.json](../../metrics/integration/render-tests/regressions/mapbox-gl-shaders%2337/style.json) | line | geojson |
| [metrics/integration/render-tests/remove-feature-state/composite-expression/style.json](../../metrics/integration/render-tests/remove-feature-state/composite-expression/style.json) | circle | geojson |
| [metrics/integration/render-tests/remove-feature-state/data-expression/style.json](../../metrics/integration/render-tests/remove-feature-state/data-expression/style.json) | circle | geojson |
| [metrics/integration/render-tests/remove-feature-state/vector-source/style.json](../../metrics/integration/render-tests/remove-feature-state/vector-source/style.json) | background, circle | vector |
| [metrics/integration/render-tests/retina-raster/default/style.json](../../metrics/integration/render-tests/retina-raster/default/style.json) | raster | raster |
| [metrics/integration/render-tests/runtime-styling/filter-default-to-false/style.json](../../metrics/integration/render-tests/runtime-styling/filter-default-to-false/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/filter-default-to-true/style.json](../../metrics/integration/render-tests/runtime-styling/filter-default-to-true/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/filter-false-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/filter-false-to-default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/filter-false-to-true/style.json](../../metrics/integration/render-tests/runtime-styling/filter-false-to-true/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/filter-true-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/filter-true-to-default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/filter-true-to-false/style.json](../../metrics/integration/render-tests/runtime-styling/filter-true-to-false/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/hillshade-array-property-mismatch/style.json](../../metrics/integration/render-tests/runtime-styling/hillshade-array-property-mismatch/style.json) | background, hillshade | raster-dem |
| [metrics/integration/render-tests/runtime-styling/image-add-1.5x-image-1x-screen/style.json](../../metrics/integration/render-tests/runtime-styling/image-add-1.5x-image-1x-screen/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/image-add-1.5x-image-2x-screen/style.json](../../metrics/integration/render-tests/runtime-styling/image-add-1.5x-image-2x-screen/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/image-add-1x-image-1x-screen/style.json](../../metrics/integration/render-tests/runtime-styling/image-add-1x-image-1x-screen/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/image-add-1x-image-2x-screen/style.json](../../metrics/integration/render-tests/runtime-styling/image-add-1x-image-2x-screen/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/image-add-2x-image-1x-screen/style.json](../../metrics/integration/render-tests/runtime-styling/image-add-2x-image-1x-screen/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/image-add-2x-image-2x-screen/style.json](../../metrics/integration/render-tests/runtime-styling/image-add-2x-image-2x-screen/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/image-add-alpha/style.json](../../metrics/integration/render-tests/runtime-styling/image-add-alpha/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/image-add-nonsdf/style.json](../../metrics/integration/render-tests/runtime-styling/image-add-nonsdf/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/image-add-pattern/style.json](../../metrics/integration/render-tests/runtime-styling/image-add-pattern/style.json) | fill | geojson |
| [metrics/integration/render-tests/runtime-styling/image-add-remove-add/style.json](../../metrics/integration/render-tests/runtime-styling/image-add-remove-add/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/image-add-sdf/style.json](../../metrics/integration/render-tests/runtime-styling/image-add-sdf/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/image-remove/style.json](../../metrics/integration/render-tests/runtime-styling/image-remove/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/image-update-icon/style.json](../../metrics/integration/render-tests/runtime-styling/image-update-icon/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/image-update-pattern/style.json](../../metrics/integration/render-tests/runtime-styling/image-update-pattern/style.json) | fill | geojson |
| [metrics/integration/render-tests/runtime-styling/layer-add-background/style.json](../../metrics/integration/render-tests/runtime-styling/layer-add-background/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/layer-add-circle/style.json](../../metrics/integration/render-tests/runtime-styling/layer-add-circle/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/layer-add-fill/style.json](../../metrics/integration/render-tests/runtime-styling/layer-add-fill/style.json) | fill | geojson |
| [metrics/integration/render-tests/runtime-styling/layer-add-line/style.json](../../metrics/integration/render-tests/runtime-styling/layer-add-line/style.json) | line | geojson |
| [metrics/integration/render-tests/runtime-styling/layer-add-raster/style.json](../../metrics/integration/render-tests/runtime-styling/layer-add-raster/style.json) | raster | raster |
| [metrics/integration/render-tests/runtime-styling/layer-add-symbol/style.json](../../metrics/integration/render-tests/runtime-styling/layer-add-symbol/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layer-remove-background/style.json](../../metrics/integration/render-tests/runtime-styling/layer-remove-background/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/layer-remove-circle/style.json](../../metrics/integration/render-tests/runtime-styling/layer-remove-circle/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/layer-remove-fill/style.json](../../metrics/integration/render-tests/runtime-styling/layer-remove-fill/style.json) | fill | geojson |
| [metrics/integration/render-tests/runtime-styling/layer-remove-line/style.json](../../metrics/integration/render-tests/runtime-styling/layer-remove-line/style.json) | line | geojson |
| [metrics/integration/render-tests/runtime-styling/layer-remove-raster/style.json](../../metrics/integration/render-tests/runtime-styling/layer-remove-raster/style.json) | raster | raster |
| [metrics/integration/render-tests/runtime-styling/layer-remove-symbol/style.json](../../metrics/integration/render-tests/runtime-styling/layer-remove-symbol/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-default-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-default-to-literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-default-to-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-default-to-property-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-default-to-property-function/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-default-to-property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-default-to-zoom-expression/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-default-to-zoom-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-default-to-zoom-function/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-default-to-zoom-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-literal-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-literal-to-default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-literal-to-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-literal-to-property-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-literal-to-property-function/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-literal-to-property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-literal-to-zoom-expression/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-literal-to-zoom-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-literal-to-zoom-function/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-literal-to-zoom-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-override-paint-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-override-paint-property-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-override-paint-property-literal/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-override-paint-property-literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-property-expression-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-property-expression-to-default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-property-expression-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-property-expression-to-literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-property-expression-to-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-property-expression-to-property-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-property-expression-to-zoom-expression/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-property-expression-to-zoom-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-property-function-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-property-function-to-default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-property-function-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-property-function-to-literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-text-variable-anchor/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-text-variable-anchor/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-zoom-and-property-expression-to-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-zoom-and-property-expression-to-property-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-zoom-and-property-expression-to-zoom-and-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-zoom-and-property-expression-to-zoom-and-property-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-zoom-and-property-expression-to-zoom-expression/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-zoom-and-property-expression-to-zoom-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-zoom-expression-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-zoom-expression-to-default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-zoom-expression-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-zoom-expression-to-literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-zoom-expression-to-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-zoom-expression-to-property-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-zoom-expression-to-zoom-and-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-zoom-expression-to-zoom-and-property-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-zoom-expression-to-zoom-expression/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-zoom-expression-to-zoom-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-zoom-function-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-zoom-function-to-default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/layout-property-zoom-function-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/layout-property-zoom-function-to-literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-default-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-default-to-literal/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/paint-property-default-to-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-default-to-property-expression/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-default-to-property-function/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-default-to-property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-default-to-zoom-expression/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-default-to-zoom-expression/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/paint-property-default-to-zoom-function/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-default-to-zoom-function/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/paint-property-fill-flat-to-extrude/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-fill-flat-to-extrude/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-literal-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-literal-to-default/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/paint-property-literal-to-expression/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-literal-to-expression/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/paint-property-literal-to-function/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-literal-to-function/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/paint-property-literal-to-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-literal-to-property-expression/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-literal-to-property-function/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-literal-to-property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-overriden-default-to-expression/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-overriden-default-to-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-overriden-default-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-overriden-default-to-literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-overriden-expression-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-overriden-expression-to-literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-property-expression-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-property-expression-to-default/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-property-expression-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-property-expression-to-literal/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-property-expression-to-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-property-expression-to-property-expression/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-property-expression-to-zoom-expression/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-property-expression-to-zoom-expression/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-property-function-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-property-function-to-default/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-property-function-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-property-function-to-literal/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-zoom-and-property-expression-to-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-zoom-and-property-expression-to-property-expression/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-zoom-and-property-expression-to-zoom-and-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-zoom-and-property-expression-to-zoom-and-property-expression/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-zoom-and-property-expression-to-zoom-expression/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-zoom-and-property-expression-to-zoom-expression/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-zoom-expression-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-zoom-expression-to-default/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/paint-property-zoom-expression-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-zoom-expression-to-literal/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/paint-property-zoom-expression-to-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-zoom-expression-to-property-expression/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-zoom-expression-to-zoom-and-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-zoom-expression-to-zoom-and-property-expression/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/paint-property-zoom-expression-to-zoom-expression/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-zoom-expression-to-zoom-expression/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/paint-property-zoom-function-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-zoom-function-to-default/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/paint-property-zoom-function-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/paint-property-zoom-function-to-literal/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/pattern-add-remove-add/style.json](../../metrics/integration/render-tests/runtime-styling/pattern-add-remove-add/style.json) | fill | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-filter-default-to-false/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-filter-default-to-false/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-filter-default-to-true/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-filter-default-to-true/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-filter-false-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-filter-false-to-default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-filter-false-to-true/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-filter-false-to-true/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-filter-true-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-filter-true-to-default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-filter-true-to-false/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-filter-true-to-false/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-glyphs/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-glyphs/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layer-add-background/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layer-add-background/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/set-style-layer-add-circle/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layer-add-circle/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layer-add-fill/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layer-add-fill/style.json) | fill | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layer-add-line/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layer-add-line/style.json) | line | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layer-add-raster/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layer-add-raster/style.json) | raster | raster |
| [metrics/integration/render-tests/runtime-styling/set-style-layer-add-symbol/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layer-add-symbol/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layer-change-source-layer/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layer-change-source-layer/style.json) | fill | vector |
| [metrics/integration/render-tests/runtime-styling/set-style-layer-change-source-type/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layer-change-source-type/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layer-change-source/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layer-change-source/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layer-remove-background/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layer-remove-background/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/set-style-layer-remove-circle/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layer-remove-circle/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layer-remove-fill/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layer-remove-fill/style.json) | fill | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layer-remove-line/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layer-remove-line/style.json) | line | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layer-remove-raster/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layer-remove-raster/style.json) | raster | raster |
| [metrics/integration/render-tests/runtime-styling/set-style-layer-remove-symbol/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layer-remove-symbol/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layer-reorder/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layer-reorder/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-default-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-default-to-literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-default-to-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-default-to-property-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-default-to-property-function/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-default-to-property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-default-to-zoom-expression/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-default-to-zoom-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-default-to-zoom-function/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-default-to-zoom-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-literal-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-literal-to-default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-literal-to-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-literal-to-property-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-literal-to-property-function/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-literal-to-property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-literal-to-zoom-expression/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-literal-to-zoom-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-literal-to-zoom-function/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-literal-to-zoom-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-property-expression-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-property-expression-to-default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-property-expression-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-property-expression-to-literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-property-function-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-property-function-to-default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-property-function-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-property-function-to-literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-zoom-expression-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-zoom-expression-to-default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-zoom-expression-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-zoom-expression-to-literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-zoom-function-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-zoom-function-to-default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-layout-property-zoom-function-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-layout-property-zoom-function-to-literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-default-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-default-to-literal/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-default-to-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-default-to-property-expression/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-default-to-property-function/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-default-to-property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-default-to-zoom-expression/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-default-to-zoom-expression/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-default-to-zoom-function/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-default-to-zoom-function/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-fill-flat-to-extrude/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-fill-flat-to-extrude/style.json) | fill-extrusion | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-literal-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-literal-to-default/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-literal-to-expression/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-literal-to-expression/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-literal-to-function/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-literal-to-function/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-literal-to-property-expression/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-literal-to-property-expression/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-literal-to-property-function/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-literal-to-property-function/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-property-expression-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-property-expression-to-default/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-property-expression-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-property-expression-to-literal/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-property-function-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-property-function-to-default/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-property-function-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-property-function-to-literal/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-zoom-expression-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-zoom-expression-to-default/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-zoom-expression-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-zoom-expression-to-literal/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-zoom-function-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-zoom-function-to-default/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/set-style-paint-property-zoom-function-to-literal/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-paint-property-zoom-function-to-literal/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/set-style-source-add-geojson-inline/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-source-add-geojson-inline/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-source-add-geojson-url/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-source-add-geojson-url/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-source-add-raster-inline/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-source-add-raster-inline/style.json) | raster | raster |
| [metrics/integration/render-tests/runtime-styling/set-style-source-add-raster-url/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-source-add-raster-url/style.json) | raster | raster |
| [metrics/integration/render-tests/runtime-styling/set-style-source-add-vector-inline/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-source-add-vector-inline/style.json) | fill | vector |
| [metrics/integration/render-tests/runtime-styling/set-style-source-add-vector-url/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-source-add-vector-url/style.json) | fill | vector |
| [metrics/integration/render-tests/runtime-styling/set-style-source-update/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-source-update/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/set-style-sprite/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-sprite/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/runtime-styling/set-style-visibility-default-to-none/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-visibility-default-to-none/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/set-style-visibility-default-to-visible/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-visibility-default-to-visible/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/set-style-visibility-none-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-visibility-none-to-default/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/set-style-visibility-none-to-visible/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-visibility-none-to-visible/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/set-style-visibility-visible-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-visibility-visible-to-default/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/set-style-visibility-visible-to-none/style.json](../../metrics/integration/render-tests/runtime-styling/set-style-visibility-visible-to-none/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/source-add-geojson-inline/style.json](../../metrics/integration/render-tests/runtime-styling/source-add-geojson-inline/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/source-add-geojson-url/style.json](../../metrics/integration/render-tests/runtime-styling/source-add-geojson-url/style.json) | circle | geojson |
| [metrics/integration/render-tests/runtime-styling/source-add-raster-inline/style.json](../../metrics/integration/render-tests/runtime-styling/source-add-raster-inline/style.json) | raster | raster |
| [metrics/integration/render-tests/runtime-styling/source-add-raster-url/style.json](../../metrics/integration/render-tests/runtime-styling/source-add-raster-url/style.json) | raster | raster |
| [metrics/integration/render-tests/runtime-styling/source-add-vector-inline/style.json](../../metrics/integration/render-tests/runtime-styling/source-add-vector-inline/style.json) | fill | vector |
| [metrics/integration/render-tests/runtime-styling/source-add-vector-url/style.json](../../metrics/integration/render-tests/runtime-styling/source-add-vector-url/style.json) | fill | vector |
| [metrics/integration/render-tests/runtime-styling/visibility-default-to-none/style.json](../../metrics/integration/render-tests/runtime-styling/visibility-default-to-none/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/visibility-default-to-visible/style.json](../../metrics/integration/render-tests/runtime-styling/visibility-default-to-visible/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/visibility-none-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/visibility-none-to-default/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/visibility-none-to-visible/style.json](../../metrics/integration/render-tests/runtime-styling/visibility-none-to-visible/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/visibility-visible-to-default/style.json](../../metrics/integration/render-tests/runtime-styling/visibility-visible-to-default/style.json) | background | — |
| [metrics/integration/render-tests/runtime-styling/visibility-visible-to-none/style.json](../../metrics/integration/render-tests/runtime-styling/visibility-visible-to-none/style.json) | background | — |
| [metrics/integration/render-tests/satellite-v9/z0/style.json](../../metrics/integration/render-tests/satellite-v9/z0/style.json) | — | — |
| [metrics/integration/render-tests/sparse-tileset/overdraw/style.json](../../metrics/integration/render-tests/sparse-tileset/overdraw/style.json) | background, fill | vector |
| [metrics/integration/render-tests/sprites/1x-screen-1x-icon/style.json](../../metrics/integration/render-tests/sprites/1x-screen-1x-icon/style.json) | symbol | geojson |
| [metrics/integration/render-tests/sprites/1x-screen-1x-pattern/style.json](../../metrics/integration/render-tests/sprites/1x-screen-1x-pattern/style.json) | background | — |
| [metrics/integration/render-tests/sprites/1x-screen-2x-icon/style.json](../../metrics/integration/render-tests/sprites/1x-screen-2x-icon/style.json) | symbol | geojson |
| [metrics/integration/render-tests/sprites/1x-screen-2x-pattern/style.json](../../metrics/integration/render-tests/sprites/1x-screen-2x-pattern/style.json) | background | — |
| [metrics/integration/render-tests/sprites/2x-screen-1x-icon/style.json](../../metrics/integration/render-tests/sprites/2x-screen-1x-icon/style.json) | symbol | geojson |
| [metrics/integration/render-tests/sprites/2x-screen-1x-pattern/style.json](../../metrics/integration/render-tests/sprites/2x-screen-1x-pattern/style.json) | background | — |
| [metrics/integration/render-tests/sprites/2x-screen-2x-icon/style.json](../../metrics/integration/render-tests/sprites/2x-screen-2x-icon/style.json) | symbol | geojson |
| [metrics/integration/render-tests/sprites/2x-screen-2x-pattern/style.json](../../metrics/integration/render-tests/sprites/2x-screen-2x-pattern/style.json) | background | — |
| [metrics/integration/render-tests/sprites/array-default-only/style.json](../../metrics/integration/render-tests/sprites/array-default-only/style.json) | symbol | geojson |
| [metrics/integration/render-tests/sprites/array-multiple/style.json](../../metrics/integration/render-tests/sprites/array-multiple/style.json) | symbol | geojson |
| [metrics/integration/render-tests/symbol-cross-fade/chinese/style.json](../../metrics/integration/render-tests/symbol-cross-fade/chinese/style.json) | background, line, symbol | geojson |
| [metrics/integration/render-tests/symbol-geometry/linestring/style.json](../../metrics/integration/render-tests/symbol-geometry/linestring/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/symbol-geometry/multilinestring/style.json](../../metrics/integration/render-tests/symbol-geometry/multilinestring/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/symbol-geometry/multipoint/style.json](../../metrics/integration/render-tests/symbol-geometry/multipoint/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/symbol-geometry/multipolygon/style.json](../../metrics/integration/render-tests/symbol-geometry/multipolygon/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/symbol-geometry/point/style.json](../../metrics/integration/render-tests/symbol-geometry/point/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/symbol-geometry/polygon/style.json](../../metrics/integration/render-tests/symbol-geometry/polygon/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/symbol-placement/line-center-buffer-tile-map-mode/style.json](../../metrics/integration/render-tests/symbol-placement/line-center-buffer-tile-map-mode/style.json) | background, line, symbol | vector |
| [metrics/integration/render-tests/symbol-placement/line-center-buffer/style.json](../../metrics/integration/render-tests/symbol-placement/line-center-buffer/style.json) | background, line, symbol | vector |
| [metrics/integration/render-tests/symbol-placement/line-center-tile-map-mode/style.json](../../metrics/integration/render-tests/symbol-placement/line-center-tile-map-mode/style.json) | background, line, symbol | vector |
| [metrics/integration/render-tests/symbol-placement/line-center/style.json](../../metrics/integration/render-tests/symbol-placement/line-center/style.json) | background, line, symbol | vector |
| [metrics/integration/render-tests/symbol-placement/line-overscaled/style.json](../../metrics/integration/render-tests/symbol-placement/line-overscaled/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/symbol-placement/line/style.json](../../metrics/integration/render-tests/symbol-placement/line/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/symbol-placement/point-polygon/style.json](../../metrics/integration/render-tests/symbol-placement/point-polygon/style.json) | background, line, symbol | vector |
| [metrics/integration/render-tests/symbol-placement/point/style.json](../../metrics/integration/render-tests/symbol-placement/point/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/symbol-sort-key/icon-expression/style.json](../../metrics/integration/render-tests/symbol-sort-key/icon-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/symbol-sort-key/placement-tile-boundary-left-then-right/style.json](../../metrics/integration/render-tests/symbol-sort-key/placement-tile-boundary-left-then-right/style.json) | symbol | geojson |
| [metrics/integration/render-tests/symbol-sort-key/placement-tile-boundary-right-then-left/style.json](../../metrics/integration/render-tests/symbol-sort-key/placement-tile-boundary-right-then-left/style.json) | symbol | geojson |
| [metrics/integration/render-tests/symbol-sort-key/text-expression/style.json](../../metrics/integration/render-tests/symbol-sort-key/text-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/symbol-sort-key/text-ignore-placement/style.json](../../metrics/integration/render-tests/symbol-sort-key/text-ignore-placement/style.json) | symbol | geojson |
| [metrics/integration/render-tests/symbol-sort-key/text-placement/style.json](../../metrics/integration/render-tests/symbol-sort-key/text-placement/style.json) | symbol | geojson |
| [metrics/integration/render-tests/symbol-spacing/line-close/style.json](../../metrics/integration/render-tests/symbol-spacing/line-close/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/symbol-spacing/line-far/style.json](../../metrics/integration/render-tests/symbol-spacing/line-far/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/symbol-spacing/line-overscaled/style.json](../../metrics/integration/render-tests/symbol-spacing/line-overscaled/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/symbol-spacing/point-close/style.json](../../metrics/integration/render-tests/symbol-spacing/point-close/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/symbol-spacing/point-far/style.json](../../metrics/integration/render-tests/symbol-spacing/point-far/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/symbol-visibility/none/style.json](../../metrics/integration/render-tests/symbol-visibility/none/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/symbol-visibility/visible/style.json](../../metrics/integration/render-tests/symbol-visibility/visible/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/symbol-z-order/default/style.json](../../metrics/integration/render-tests/symbol-z-order/default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/symbol-z-order/disabled/style.json](../../metrics/integration/render-tests/symbol-z-order/disabled/style.json) | symbol | geojson |
| [metrics/integration/render-tests/symbol-z-order/icon-with-text/style.json](../../metrics/integration/render-tests/symbol-z-order/icon-with-text/style.json) | symbol | geojson |
| [metrics/integration/render-tests/symbol-z-order/pitched/style.json](../../metrics/integration/render-tests/symbol-z-order/pitched/style.json) | symbol | geojson |
| [metrics/integration/render-tests/symbol-z-order/viewport-y/style.json](../../metrics/integration/render-tests/symbol-z-order/viewport-y/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-anchor/bottom-left/style.json](../../metrics/integration/render-tests/text-anchor/bottom-left/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-anchor/bottom-right/style.json](../../metrics/integration/render-tests/text-anchor/bottom-right/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-anchor/bottom/style.json](../../metrics/integration/render-tests/text-anchor/bottom/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-anchor/center/style.json](../../metrics/integration/render-tests/text-anchor/center/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-anchor/left/style.json](../../metrics/integration/render-tests/text-anchor/left/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-anchor/property-function/style.json](../../metrics/integration/render-tests/text-anchor/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-anchor/right/style.json](../../metrics/integration/render-tests/text-anchor/right/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-anchor/top-left/style.json](../../metrics/integration/render-tests/text-anchor/top-left/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-anchor/top-right/style.json](../../metrics/integration/render-tests/text-anchor/top-right/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-anchor/top/style.json](../../metrics/integration/render-tests/text-anchor/top/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-arabic/letter-spacing/style.json](../../metrics/integration/render-tests/text-arabic/letter-spacing/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-arabic/line-break-mixed/style.json](../../metrics/integration/render-tests/text-arabic/line-break-mixed/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-arabic/line-break/style.json](../../metrics/integration/render-tests/text-arabic/line-break/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-arabic/mixed-numeric/style.json](../../metrics/integration/render-tests/text-arabic/mixed-numeric/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-arabic/multi-paragraph/style.json](../../metrics/integration/render-tests/text-arabic/multi-paragraph/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-breaking/left-parenthesis/style.json](../../metrics/integration/render-tests/text-breaking/left-parenthesis/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-breaking/whitespace/style.json](../../metrics/integration/render-tests/text-breaking/whitespace/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-color/default/style.json](../../metrics/integration/render-tests/text-color/default/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-color/function/style.json](../../metrics/integration/render-tests/text-color/function/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-color/literal/style.json](../../metrics/integration/render-tests/text-color/literal/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-color/property-function/style.json](../../metrics/integration/render-tests/text-color/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-color/translucent-icon/style.json](../../metrics/integration/render-tests/text-color/translucent-icon/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-color/translucent/style.json](../../metrics/integration/render-tests/text-color/translucent/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-color/transparent/style.json](../../metrics/integration/render-tests/text-color/transparent/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-field/formatted-arabic/style.json](../../metrics/integration/render-tests/text-field/formatted-arabic/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-field/formatted-images-constant-size/style.json](../../metrics/integration/render-tests/text-field/formatted-images-constant-size/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-field/formatted-images-line/style.json](../../metrics/integration/render-tests/text-field/formatted-images-line/style.json) | line, symbol | geojson |
| [metrics/integration/render-tests/text-field/formatted-images-mixed/style.json](../../metrics/integration/render-tests/text-field/formatted-images-mixed/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-field/formatted-images-multiline/style.json](../../metrics/integration/render-tests/text-field/formatted-images-multiline/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-field/formatted-images-variable-anchors-justification/style.json](../../metrics/integration/render-tests/text-field/formatted-images-variable-anchors-justification/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-field/formatted-images-vertical/style.json](../../metrics/integration/render-tests/text-field/formatted-images-vertical/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-field/formatted-images-zoom-dependent-size/style.json](../../metrics/integration/render-tests/text-field/formatted-images-zoom-dependent-size/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-field/formatted-images/style.json](../../metrics/integration/render-tests/text-field/formatted-images/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-field/formatted-line/style.json](../../metrics/integration/render-tests/text-field/formatted-line/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-field/formatted-text-color-overrides-nested-expression/style.json](../../metrics/integration/render-tests/text-field/formatted-text-color-overrides-nested-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-field/formatted-text-color-overrides/style.json](../../metrics/integration/render-tests/text-field/formatted-text-color-overrides/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-field/formatted-text-color/style.json](../../metrics/integration/render-tests/text-field/formatted-text-color/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-field/formatted/style.json](../../metrics/integration/render-tests/text-field/formatted/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-field/literal/style.json](../../metrics/integration/render-tests/text-field/literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-field/property-function/style.json](../../metrics/integration/render-tests/text-field/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-field/token/style.json](../../metrics/integration/render-tests/text-field/token/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-font/burmese/style.json](../../metrics/integration/render-tests/text-font/burmese/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-font/camera-function/style.json](../../metrics/integration/render-tests/text-font/camera-function/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-font/chinese/style.json](../../metrics/integration/render-tests/text-font/chinese/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-font/data-expression/style.json](../../metrics/integration/render-tests/text-font/data-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-font/devanagari/style.json](../../metrics/integration/render-tests/text-font/devanagari/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-font/khmer/style.json](../../metrics/integration/render-tests/text-font/khmer/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-font/literal/style.json](../../metrics/integration/render-tests/text-font/literal/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-halo-blur/default/style.json](../../metrics/integration/render-tests/text-halo-blur/default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-halo-blur/function/style.json](../../metrics/integration/render-tests/text-halo-blur/function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-halo-blur/literal/style.json](../../metrics/integration/render-tests/text-halo-blur/literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-halo-blur/property-function/style.json](../../metrics/integration/render-tests/text-halo-blur/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-halo-color/default/style.json](../../metrics/integration/render-tests/text-halo-color/default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-halo-color/function/style.json](../../metrics/integration/render-tests/text-halo-color/function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-halo-color/literal/style.json](../../metrics/integration/render-tests/text-halo-color/literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-halo-color/property-function/style.json](../../metrics/integration/render-tests/text-halo-color/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-halo-width/default/style.json](../../metrics/integration/render-tests/text-halo-width/default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-halo-width/function/style.json](../../metrics/integration/render-tests/text-halo-width/function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-halo-width/literal/style.json](../../metrics/integration/render-tests/text-halo-width/literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-halo-width/property-function/style.json](../../metrics/integration/render-tests/text-halo-width/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-justify/auto/style.json](../../metrics/integration/render-tests/text-justify/auto/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-justify/left/style.json](../../metrics/integration/render-tests/text-justify/left/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-justify/property-function/style.json](../../metrics/integration/render-tests/text-justify/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-justify/right/style.json](../../metrics/integration/render-tests/text-justify/right/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-keep-upright/line-placement-false/style.json](../../metrics/integration/render-tests/text-keep-upright/line-placement-false/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-keep-upright/line-placement-true-offset/style.json](../../metrics/integration/render-tests/text-keep-upright/line-placement-true-offset/style.json) | line, symbol | geojson |
| [metrics/integration/render-tests/text-keep-upright/line-placement-true-pitched/style.json](../../metrics/integration/render-tests/text-keep-upright/line-placement-true-pitched/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-keep-upright/line-placement-true-roll-pitch-bearing/style.json](../../metrics/integration/render-tests/text-keep-upright/line-placement-true-roll-pitch-bearing/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-keep-upright/line-placement-true-rolled/style.json](../../metrics/integration/render-tests/text-keep-upright/line-placement-true-rolled/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-keep-upright/line-placement-true-rotated/style.json](../../metrics/integration/render-tests/text-keep-upright/line-placement-true-rotated/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-keep-upright/line-placement-true-text-anchor/style.json](../../metrics/integration/render-tests/text-keep-upright/line-placement-true-text-anchor/style.json) | line, symbol | geojson |
| [metrics/integration/render-tests/text-keep-upright/line-placement-true/style.json](../../metrics/integration/render-tests/text-keep-upright/line-placement-true/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-keep-upright/point-placement-align-map-false/style.json](../../metrics/integration/render-tests/text-keep-upright/point-placement-align-map-false/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-keep-upright/point-placement-align-map-true/style.json](../../metrics/integration/render-tests/text-keep-upright/point-placement-align-map-true/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-keep-upright/point-placement-align-viewport-false/style.json](../../metrics/integration/render-tests/text-keep-upright/point-placement-align-viewport-false/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-keep-upright/point-placement-align-viewport-true/style.json](../../metrics/integration/render-tests/text-keep-upright/point-placement-align-viewport-true/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-letter-spacing/function-close/style.json](../../metrics/integration/render-tests/text-letter-spacing/function-close/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-letter-spacing/function-far/style.json](../../metrics/integration/render-tests/text-letter-spacing/function-far/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-letter-spacing/literal/style.json](../../metrics/integration/render-tests/text-letter-spacing/literal/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-letter-spacing/property-function/style.json](../../metrics/integration/render-tests/text-letter-spacing/property-function/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-letter-spacing/zoom-and-property-function/style.json](../../metrics/integration/render-tests/text-letter-spacing/zoom-and-property-function/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-line-height/literal/style.json](../../metrics/integration/render-tests/text-line-height/literal/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-max-angle/line-center/style.json](../../metrics/integration/render-tests/text-max-angle/line-center/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-max-angle/literal/style.json](../../metrics/integration/render-tests/text-max-angle/literal/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-max-width/force-double-newline/style.json](../../metrics/integration/render-tests/text-max-width/force-double-newline/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-max-width/force-newline-line-center/style.json](../../metrics/integration/render-tests/text-max-width/force-newline-line-center/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-max-width/force-newline-line/style.json](../../metrics/integration/render-tests/text-max-width/force-newline-line/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-max-width/force-newline/style.json](../../metrics/integration/render-tests/text-max-width/force-newline/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-max-width/ideographic-breaking/style.json](../../metrics/integration/render-tests/text-max-width/ideographic-breaking/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-max-width/ideographic-punctuation-breaking/style.json](../../metrics/integration/render-tests/text-max-width/ideographic-punctuation-breaking/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-max-width/literal/style.json](../../metrics/integration/render-tests/text-max-width/literal/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-max-width/property-function/style.json](../../metrics/integration/render-tests/text-max-width/property-function/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-max-width/zero-width-line-center-placement/style.json](../../metrics/integration/render-tests/text-max-width/zero-width-line-center-placement/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-max-width/zero-width-line-placement/style.json](../../metrics/integration/render-tests/text-max-width/zero-width-line-placement/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-max-width/zero-width-point-placement/style.json](../../metrics/integration/render-tests/text-max-width/zero-width-point-placement/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-max-width/zoom-and-property-function/style.json](../../metrics/integration/render-tests/text-max-width/zoom-and-property-function/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-no-cross-source-collision/default/style.json](../../metrics/integration/render-tests/text-no-cross-source-collision/default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorcenter-justifycenter-offsetnegative/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorcenter-justifycenter-offsetnegative/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorcenter-justifycenter-offsetpositive/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorcenter-justifycenter-offsetpositive/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorcenter-justifyleft-offsetnegative/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorcenter-justifyleft-offsetnegative/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorcenter-justifyleft-offsetpositive/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorcenter-justifyleft-offsetpositive/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorcenter-justifyright-offsetnegative/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorcenter-justifyright-offsetnegative/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorcenter-justifyright-offsetpositive/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorcenter-justifyright-offsetpositive/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorleft-justifycenter-offsetnegative/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorleft-justifycenter-offsetnegative/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorleft-justifycenter-offsetpositive/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorleft-justifycenter-offsetpositive/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorleft-justifyleft-offsetnegative/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorleft-justifyleft-offsetnegative/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorleft-justifyleft-offsetpositive/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorleft-justifyleft-offsetpositive/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorleft-justifyright-offsetnegative/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorleft-justifyright-offsetnegative/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorleft-justifyright-offsetpositive/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorleft-justifyright-offsetpositive/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorright-justifycenter-offsetnegative/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorright-justifycenter-offsetnegative/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorright-justifycenter-offsetpositive/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorright-justifycenter-offsetpositive/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorright-justifyleft-offsetnegative/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorright-justifyleft-offsetnegative/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorright-justifyleft-offsetpositive/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorright-justifyleft-offsetpositive/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorright-justifyright-offsetnegative/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorright-justifyright-offsetnegative/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal-multiline-anchorright-justifyright-offsetpositive/style.json](../../metrics/integration/render-tests/text-offset/literal-multiline-anchorright-justifyright-offsetpositive/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/literal/style.json](../../metrics/integration/render-tests/text-offset/literal/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-offset/property-function/style.json](../../metrics/integration/render-tests/text-offset/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-offset/semiliteral/style.json](../../metrics/integration/render-tests/text-offset/semiliteral/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-opacity/default/style.json](../../metrics/integration/render-tests/text-opacity/default/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-opacity/function/style.json](../../metrics/integration/render-tests/text-opacity/function/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-opacity/literal/style.json](../../metrics/integration/render-tests/text-opacity/literal/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-opacity/property-function/style.json](../../metrics/integration/render-tests/text-opacity/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-pitch-alignment/auto-text-rotation-alignment-map/style.json](../../metrics/integration/render-tests/text-pitch-alignment/auto-text-rotation-alignment-map/style.json) | background, line, symbol | vector |
| [metrics/integration/render-tests/text-pitch-alignment/auto-text-rotation-alignment-viewport/style.json](../../metrics/integration/render-tests/text-pitch-alignment/auto-text-rotation-alignment-viewport/style.json) | background, line, symbol | vector |
| [metrics/integration/render-tests/text-pitch-alignment/map-text-depthtest/style.json](../../metrics/integration/render-tests/text-pitch-alignment/map-text-depthtest/style.json) | background, fill, symbol | geojson |
| [metrics/integration/render-tests/text-pitch-alignment/map-text-rotation-alignment-map/style.json](../../metrics/integration/render-tests/text-pitch-alignment/map-text-rotation-alignment-map/style.json) | background, line, symbol | vector |
| [metrics/integration/render-tests/text-pitch-alignment/map-text-rotation-alignment-viewport/style.json](../../metrics/integration/render-tests/text-pitch-alignment/map-text-rotation-alignment-viewport/style.json) | background, line, symbol | vector |
| [metrics/integration/render-tests/text-pitch-alignment/viewport-overzoomed-single-glyph/style.json](../../metrics/integration/render-tests/text-pitch-alignment/viewport-overzoomed-single-glyph/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-pitch-alignment/viewport-overzoomed/style.json](../../metrics/integration/render-tests/text-pitch-alignment/viewport-overzoomed/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-pitch-alignment/viewport-text-depthtest/style.json](../../metrics/integration/render-tests/text-pitch-alignment/viewport-text-depthtest/style.json) | background, fill, symbol | geojson |
| [metrics/integration/render-tests/text-pitch-alignment/viewport-text-rotation-alignment-map/style.json](../../metrics/integration/render-tests/text-pitch-alignment/viewport-text-rotation-alignment-map/style.json) | background, line, symbol | vector |
| [metrics/integration/render-tests/text-pitch-alignment/viewport-text-rotation-alignment-viewport/style.json](../../metrics/integration/render-tests/text-pitch-alignment/viewport-text-rotation-alignment-viewport/style.json) | background, line, symbol | vector |
| [metrics/integration/render-tests/text-pitch-scaling/line-half-roll/style.json](../../metrics/integration/render-tests/text-pitch-scaling/line-half-roll/style.json) | background, line, symbol | vector |
| [metrics/integration/render-tests/text-pitch-scaling/line-half/style.json](../../metrics/integration/render-tests/text-pitch-scaling/line-half/style.json) | background, line, symbol | vector |
| [metrics/integration/render-tests/text-radial-offset/basic/style.json](../../metrics/integration/render-tests/text-radial-offset/basic/style.json) | background, circle, symbol | geojson |
| [metrics/integration/render-tests/text-roll-alignment/auto-symbol-placement-line/style.json](../../metrics/integration/render-tests/text-roll-alignment/auto-symbol-placement-line/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-roll-alignment/auto-symbol-placement-point/style.json](../../metrics/integration/render-tests/text-roll-alignment/auto-symbol-placement-point/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-roll-alignment/map-symbol-placement-line/style.json](../../metrics/integration/render-tests/text-roll-alignment/map-symbol-placement-line/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-roll-alignment/map-symbol-placement-point/style.json](../../metrics/integration/render-tests/text-roll-alignment/map-symbol-placement-point/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-roll-alignment/viewport-symbol-placement-line/style.json](../../metrics/integration/render-tests/text-roll-alignment/viewport-symbol-placement-line/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-roll-alignment/viewport-symbol-placement-point/style.json](../../metrics/integration/render-tests/text-roll-alignment/viewport-symbol-placement-point/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-rotate/anchor-bottom/style.json](../../metrics/integration/render-tests/text-rotate/anchor-bottom/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-rotate/anchor-left/style.json](../../metrics/integration/render-tests/text-rotate/anchor-left/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-rotate/anchor-right/style.json](../../metrics/integration/render-tests/text-rotate/anchor-right/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-rotate/anchor-top/style.json](../../metrics/integration/render-tests/text-rotate/anchor-top/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-rotate/function/style.json](../../metrics/integration/render-tests/text-rotate/function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-rotate/literal/style.json](../../metrics/integration/render-tests/text-rotate/literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-rotate/property-function/style.json](../../metrics/integration/render-tests/text-rotate/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-rotate/with-offset/style.json](../../metrics/integration/render-tests/text-rotate/with-offset/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-rotation-alignment/auto-symbol-placement-line/style.json](../../metrics/integration/render-tests/text-rotation-alignment/auto-symbol-placement-line/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-rotation-alignment/auto-symbol-placement-point/style.json](../../metrics/integration/render-tests/text-rotation-alignment/auto-symbol-placement-point/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-rotation-alignment/map-symbol-placement-line/style.json](../../metrics/integration/render-tests/text-rotation-alignment/map-symbol-placement-line/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-rotation-alignment/map-symbol-placement-point/style.json](../../metrics/integration/render-tests/text-rotation-alignment/map-symbol-placement-point/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-rotation-alignment/viewport-symbol-placement-line/style.json](../../metrics/integration/render-tests/text-rotation-alignment/viewport-symbol-placement-line/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-rotation-alignment/viewport-symbol-placement-point/style.json](../../metrics/integration/render-tests/text-rotation-alignment/viewport-symbol-placement-point/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-size/camera-function-high-base/style.json](../../metrics/integration/render-tests/text-size/camera-function-high-base/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-size/camera-function-interval/style.json](../../metrics/integration/render-tests/text-size/camera-function-interval/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-size/composite-expression/style.json](../../metrics/integration/render-tests/text-size/composite-expression/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-size/composite-function-line-placement/style.json](../../metrics/integration/render-tests/text-size/composite-function-line-placement/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-size/composite-function/style.json](../../metrics/integration/render-tests/text-size/composite-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-size/default/style.json](../../metrics/integration/render-tests/text-size/default/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-size/function/style.json](../../metrics/integration/render-tests/text-size/function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-size/literal/style.json](../../metrics/integration/render-tests/text-size/literal/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-size/nan/style.json](../../metrics/integration/render-tests/text-size/nan/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-size/property-function/style.json](../../metrics/integration/render-tests/text-size/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-size/zero/style.json](../../metrics/integration/render-tests/text-size/zero/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-tile-edge-clipping/default/style.json](../../metrics/integration/render-tests/text-tile-edge-clipping/default/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-transform/lowercase/style.json](../../metrics/integration/render-tests/text-transform/lowercase/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-transform/property-function/style.json](../../metrics/integration/render-tests/text-transform/property-function/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-transform/uppercase/style.json](../../metrics/integration/render-tests/text-transform/uppercase/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-translate-anchor/map/style.json](../../metrics/integration/render-tests/text-translate-anchor/map/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-translate-anchor/viewport/style.json](../../metrics/integration/render-tests/text-translate-anchor/viewport/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-translate/default/style.json](../../metrics/integration/render-tests/text-translate/default/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-translate/function/style.json](../../metrics/integration/render-tests/text-translate/function/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-translate/literal/style.json](../../metrics/integration/render-tests/text-translate/literal/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor-offset/all-anchors-icon-text-fit/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/all-anchors-icon-text-fit/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor-offset/all-anchors-text-allow-overlap/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/all-anchors-text-allow-overlap/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor-offset/all-anchors/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/all-anchors/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor-offset/databind-coalesce/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/databind-coalesce/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor-offset/databind-interpolate/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/databind-interpolate/style.json) | background, symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor-offset/icon-image-all-anchors/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/icon-image-all-anchors/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor-offset/icon-image-offset/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/icon-image-offset/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor-offset/icon-image/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/icon-image/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor-offset/icon-text-fit-collision-box/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/icon-text-fit-collision-box/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor-offset/no-animate-zoom/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/no-animate-zoom/style.json) | background, circle, symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor-offset/pitched-offset/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/pitched-offset/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor-offset/pitched-with-map/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/pitched-with-map/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor-offset/pitched/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/pitched/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor-offset/rotated-offset/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/rotated-offset/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor-offset/rotated-with-map/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/rotated-with-map/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor-offset/rotated/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/rotated/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor-offset/single-justification/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/single-justification/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor-offset/single-line/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/single-line/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor-offset/text-allow-overlap/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/text-allow-overlap/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor-offset/top-bottom-left-right/style.json](../../metrics/integration/render-tests/text-variable-anchor-offset/top-bottom-left-right/style.json) | background, circle, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor/all-anchors-icon-text-fit/style.json](../../metrics/integration/render-tests/text-variable-anchor/all-anchors-icon-text-fit/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor/all-anchors-offset-zero/style.json](../../metrics/integration/render-tests/text-variable-anchor/all-anchors-offset-zero/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor/all-anchors-offset/style.json](../../metrics/integration/render-tests/text-variable-anchor/all-anchors-offset/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor/all-anchors-radial-offset-zero/style.json](../../metrics/integration/render-tests/text-variable-anchor/all-anchors-radial-offset-zero/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor/all-anchors-text-allow-overlap/style.json](../../metrics/integration/render-tests/text-variable-anchor/all-anchors-text-allow-overlap/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor/all-anchors-tile-map-mode/style.json](../../metrics/integration/render-tests/text-variable-anchor/all-anchors-tile-map-mode/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor/all-anchors-two-dimentional-offset-negative/style.json](../../metrics/integration/render-tests/text-variable-anchor/all-anchors-two-dimentional-offset-negative/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor/all-anchors-two-dimentional-offset-zero/style.json](../../metrics/integration/render-tests/text-variable-anchor/all-anchors-two-dimentional-offset-zero/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor/all-anchors-two-dimentional-offset/style.json](../../metrics/integration/render-tests/text-variable-anchor/all-anchors-two-dimentional-offset/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor/all-anchors/style.json](../../metrics/integration/render-tests/text-variable-anchor/all-anchors/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor/icon-image-all-anchors/style.json](../../metrics/integration/render-tests/text-variable-anchor/icon-image-all-anchors/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor/icon-image/style.json](../../metrics/integration/render-tests/text-variable-anchor/icon-image/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor/icon-text-fit-collision-box/style.json](../../metrics/integration/render-tests/text-variable-anchor/icon-text-fit-collision-box/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor/left-top-right-bottom-offset-tile-map-mode/style.json](../../metrics/integration/render-tests/text-variable-anchor/left-top-right-bottom-offset-tile-map-mode/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor/no-animate-zoom/style.json](../../metrics/integration/render-tests/text-variable-anchor/no-animate-zoom/style.json) | background, circle, symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor/pitched-offset/style.json](../../metrics/integration/render-tests/text-variable-anchor/pitched-offset/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor/pitched-rotated-debug/style.json](../../metrics/integration/render-tests/text-variable-anchor/pitched-rotated-debug/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor/pitched-with-map/style.json](../../metrics/integration/render-tests/text-variable-anchor/pitched-with-map/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor/pitched/style.json](../../metrics/integration/render-tests/text-variable-anchor/pitched/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor/remember-last-placement/style.json](../../metrics/integration/render-tests/text-variable-anchor/remember-last-placement/style.json) | background, circle, symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor/rotated-offset/style.json](../../metrics/integration/render-tests/text-variable-anchor/rotated-offset/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor/rotated-with-map/style.json](../../metrics/integration/render-tests/text-variable-anchor/rotated-with-map/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor/rotated/style.json](../../metrics/integration/render-tests/text-variable-anchor/rotated/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor/single-justification/style.json](../../metrics/integration/render-tests/text-variable-anchor/single-justification/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor/single-line/style.json](../../metrics/integration/render-tests/text-variable-anchor/single-line/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-variable-anchor/text-allow-overlap/style.json](../../metrics/integration/render-tests/text-variable-anchor/text-allow-overlap/style.json) | circle, symbol | geojson |
| [metrics/integration/render-tests/text-variable-anchor/top-bottom-left-right/style.json](../../metrics/integration/render-tests/text-variable-anchor/top-bottom-left-right/style.json) | background, circle, symbol | vector |
| [metrics/integration/render-tests/text-visibility/none/style.json](../../metrics/integration/render-tests/text-visibility/none/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-visibility/visible/style.json](../../metrics/integration/render-tests/text-visibility/visible/style.json) | background, symbol | vector |
| [metrics/integration/render-tests/text-writing-mode/line_label/chinese-punctuation/style.json](../../metrics/integration/render-tests/text-writing-mode/line_label/chinese-punctuation/style.json) | background, line, symbol | geojson |
| [metrics/integration/render-tests/text-writing-mode/line_label/chinese/style.json](../../metrics/integration/render-tests/text-writing-mode/line_label/chinese/style.json) | background, line, symbol | geojson |
| [metrics/integration/render-tests/text-writing-mode/line_label/latin/style.json](../../metrics/integration/render-tests/text-writing-mode/line_label/latin/style.json) | background, line, symbol | geojson |
| [metrics/integration/render-tests/text-writing-mode/line_label/mixed-upright-digits/style.json](../../metrics/integration/render-tests/text-writing-mode/line_label/mixed-upright-digits/style.json) | background, line, symbol | geojson |
| [metrics/integration/render-tests/text-writing-mode/line_label/mixed-upright-edge-cases/style.json](../../metrics/integration/render-tests/text-writing-mode/line_label/mixed-upright-edge-cases/style.json) | background, line, symbol | geojson |
| [metrics/integration/render-tests/text-writing-mode/line_label/mixed/style.json](../../metrics/integration/render-tests/text-writing-mode/line_label/mixed/style.json) | background, line, symbol | geojson |
| [metrics/integration/render-tests/text-writing-mode/point_label/cjk-arabic-vertical-mode/style.json](../../metrics/integration/render-tests/text-writing-mode/point_label/cjk-arabic-vertical-mode/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-writing-mode/point_label/cjk-horizontal-vertical-mode/style.json](../../metrics/integration/render-tests/text-writing-mode/point_label/cjk-horizontal-vertical-mode/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-writing-mode/point_label/cjk-multiline-vertical-horizontal-mode/style.json](../../metrics/integration/render-tests/text-writing-mode/point_label/cjk-multiline-vertical-horizontal-mode/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-writing-mode/point_label/cjk-punctuation-vertical-mode/style.json](../../metrics/integration/render-tests/text-writing-mode/point_label/cjk-punctuation-vertical-mode/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-writing-mode/point_label/cjk-variable-anchors-vertical-horizontal-mode-icon-text-fit/style.json](../../metrics/integration/render-tests/text-writing-mode/point_label/cjk-variable-anchors-vertical-horizontal-mode-icon-text-fit/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-writing-mode/point_label/cjk-variable-anchors-vertical-horizontal-mode/style.json](../../metrics/integration/render-tests/text-writing-mode/point_label/cjk-variable-anchors-vertical-horizontal-mode/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-writing-mode/point_label/cjk-variable-anchors-vertical-mode/style.json](../../metrics/integration/render-tests/text-writing-mode/point_label/cjk-variable-anchors-vertical-mode/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-writing-mode/point_label/cjk-vertical-horizontal-mode/style.json](../../metrics/integration/render-tests/text-writing-mode/point_label/cjk-vertical-horizontal-mode/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-writing-mode/point_label/cjk-vertical-mode/style.json](../../metrics/integration/render-tests/text-writing-mode/point_label/cjk-vertical-mode/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-writing-mode/point_label/latin-vertical-mode/style.json](../../metrics/integration/render-tests/text-writing-mode/point_label/latin-vertical-mode/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-writing-mode/point_label/mixed-multiline-vertical-horizontal-mode-icon-text-fit/style.json](../../metrics/integration/render-tests/text-writing-mode/point_label/mixed-multiline-vertical-horizontal-mode-icon-text-fit/style.json) | symbol | geojson |
| [metrics/integration/render-tests/text-writing-mode/point_label/mixed-multiline-vertical-horizontal-mode/style.json](../../metrics/integration/render-tests/text-writing-mode/point_label/mixed-multiline-vertical-horizontal-mode/style.json) | symbol | geojson |
| [metrics/integration/render-tests/tile-lod/default/style.json](../../metrics/integration/render-tests/tile-lod/default/style.json) | raster | raster |
| [metrics/integration/render-tests/tile-lod/distance-based-pitch-threshold/style.json](../../metrics/integration/render-tests/tile-lod/distance-based-pitch-threshold/style.json) | raster | raster |
| [metrics/integration/render-tests/tile-lod/distance-based-scale/style.json](../../metrics/integration/render-tests/tile-lod/distance-based-scale/style.json) | raster | raster |
| [metrics/integration/render-tests/tile-lod/distance-based/style.json](../../metrics/integration/render-tests/tile-lod/distance-based/style.json) | raster | raster |
| [metrics/integration/render-tests/tile-lod/min-radius/style.json](../../metrics/integration/render-tests/tile-lod/min-radius/style.json) | raster | raster |
| [metrics/integration/render-tests/tile-lod/pitch-threshold/style.json](../../metrics/integration/render-tests/tile-lod/pitch-threshold/style.json) | raster | raster |
| [metrics/integration/render-tests/tile-lod/scale/style.json](../../metrics/integration/render-tests/tile-lod/scale/style.json) | raster | raster |
| [metrics/integration/render-tests/tile-lod/zoom-shift/style.json](../../metrics/integration/render-tests/tile-lod/zoom-shift/style.json) | raster | raster |
| [metrics/integration/render-tests/tile-mode/streets-v11/style.json](../../metrics/integration/render-tests/tile-mode/streets-v11/style.json) | — | — |
| [metrics/integration/render-tests/tilejson-bounds/default/style.json](../../metrics/integration/render-tests/tilejson-bounds/default/style.json) | background, line | vector |
| [metrics/integration/render-tests/tilejson-bounds/overwrite-bounds/style.json](../../metrics/integration/render-tests/tilejson-bounds/overwrite-bounds/style.json) | background, line | vector |
| [metrics/integration/render-tests/tms/tms/style.json](../../metrics/integration/render-tests/tms/tms/style.json) | background, fill | vector |
| [metrics/integration/render-tests/video/default/style.json](../../metrics/integration/render-tests/video/default/style.json) | raster | video |
| [metrics/integration/render-tests/within/filter-with-inlined-geojson/style.json](../../metrics/integration/render-tests/within/filter-with-inlined-geojson/style.json) | circle, fill | geojson |
| [metrics/integration/render-tests/within/layout-text/style.json](../../metrics/integration/render-tests/within/layout-text/style.json) | fill, symbol | geojson |
| [metrics/integration/render-tests/within/paint-circle/style.json](../../metrics/integration/render-tests/within/paint-circle/style.json) | circle, fill | geojson |
| [metrics/integration/render-tests/within/paint-icon/style.json](../../metrics/integration/render-tests/within/paint-icon/style.json) | fill, symbol | geojson |
| [metrics/integration/render-tests/within/paint-line/style.json](../../metrics/integration/render-tests/within/paint-line/style.json) | circle, fill, line | geojson |
| [metrics/integration/render-tests/within/paint-text/style.json](../../metrics/integration/render-tests/within/paint-text/style.json) | fill, symbol | geojson |
| [metrics/integration/render-tests/zoom-history/in/style.json](../../metrics/integration/render-tests/zoom-history/in/style.json) | line | geojson |
| [metrics/integration/render-tests/zoom-history/out/style.json](../../metrics/integration/render-tests/zoom-history/out/style.json) | line | geojson |
| [metrics/integration/render-tests/zoom-visibility/above/style.json](../../metrics/integration/render-tests/zoom-visibility/above/style.json) | circle | geojson |
| [metrics/integration/render-tests/zoom-visibility/below/style.json](../../metrics/integration/render-tests/zoom-visibility/below/style.json) | circle | geojson |
| [metrics/integration/render-tests/zoom-visibility/in-range/style.json](../../metrics/integration/render-tests/zoom-visibility/in-range/style.json) | circle | geojson |
| [metrics/integration/render-tests/zoom-visibility/out-of-range/style.json](../../metrics/integration/render-tests/zoom-visibility/out-of-range/style.json) | circle | geojson |
| [metrics/integration/render-tests/zoom-visibility/was-above/style.json](../../metrics/integration/render-tests/zoom-visibility/was-above/style.json) | circle | geojson |
| [metrics/integration/render-tests/zoom-visibility/was-below/style.json](../../metrics/integration/render-tests/zoom-visibility/was-below/style.json) | circle | geojson |
| [metrics/integration/render-tests/zoomed-fill/default/style.json](../../metrics/integration/render-tests/zoomed-fill/default/style.json) | background, fill | vector |
| [metrics/integration/render-tests/zoomed-fill/negative-zoom/style.json](../../metrics/integration/render-tests/zoomed-fill/negative-zoom/style.json) | background, fill | vector |
| [metrics/integration/render-tests/zoomed-raster/fractional/style.json](../../metrics/integration/render-tests/zoomed-raster/fractional/style.json) | raster | raster |
| [metrics/integration/render-tests/zoomed-raster/overzoom/style.json](../../metrics/integration/render-tests/zoomed-raster/overzoom/style.json) | raster | raster |
| [metrics/integration/render-tests/zoomed-raster/underzoom/style.json](../../metrics/integration/render-tests/zoomed-raster/underzoom/style.json) | raster | raster |

### integration shared assets (6)

| Style file | Layer families | Source types |
| --- | --- | --- |
| [metrics/integration/styles/bangkok.json](../../metrics/integration/styles/bangkok.json) | background, fill, line, symbol | vector |
| [metrics/integration/styles/chicago.json](../../metrics/integration/styles/chicago.json) | background, fill, line, symbol | vector |
| [metrics/integration/styles/nepal.json](../../metrics/integration/styles/nepal.json) | background, fill, line, symbol | vector |
| [metrics/integration/styles/norway.json](../../metrics/integration/styles/norway.json) | background, fill, line, symbol | vector |
| [metrics/integration/styles/sanfrancisco.json](../../metrics/integration/styles/sanfrancisco.json) | background, fill, line, symbol | vector |
| [metrics/integration/styles/uruguay.json](../../metrics/integration/styles/uruguay.json) | background, fill, line, symbol | vector |

### metrics archived results/manifests (7)

| Style file | Layer families | Source types |
| --- | --- | --- |
| [metrics/binary-size/android-arm64-v8a/style.json](../../metrics/binary-size/android-arm64-v8a/style.json) | background | — |
| [metrics/binary-size/android-armeabi-v7a/style.json](../../metrics/binary-size/android-armeabi-v7a/style.json) | background | — |
| [metrics/binary-size/android-x86/style.json](../../metrics/binary-size/android-x86/style.json) | background | — |
| [metrics/binary-size/android-x86_64/style.json](../../metrics/binary-size/android-x86_64/style.json) | background | — |
| [metrics/binary-size/linux-clang8/style.json](../../metrics/binary-size/linux-clang8/style.json) | background | — |
| [metrics/binary-size/linux-gcc8/style.json](../../metrics/binary-size/linux-gcc8/style.json) | background | — |
| [metrics/binary-size/macos-xcode11/style.json](../../metrics/binary-size/macos-xcode11/style.json) | background | — |

### metrics test styles (42)

| Style file | Layer families | Source types |
| --- | --- | --- |
| [metrics/tests/location_indicator/change_image/style.json](../../metrics/tests/location_indicator/change_image/style.json) | background, circle, location-indicator | geojson |
| [metrics/tests/location_indicator/dateline/style.json](../../metrics/tests/location_indicator/dateline/style.json) | background, location-indicator | — |
| [metrics/tests/location_indicator/default/style.json](../../metrics/tests/location_indicator/default/style.json) | background, circle, location-indicator | — |
| [metrics/tests/location_indicator/image_pixel_ratio/style.json](../../metrics/tests/location_indicator/image_pixel_ratio/style.json) | background, circle, location-indicator | geojson |
| [metrics/tests/location_indicator/no_radius_border/style.json](../../metrics/tests/location_indicator/no_radius_border/style.json) | background, location-indicator | — |
| [metrics/tests/location_indicator/no_radius_fill/style.json](../../metrics/tests/location_indicator/no_radius_fill/style.json) | background, location-indicator | — |
| [metrics/tests/location_indicator/no_textures/style.json](../../metrics/tests/location_indicator/no_textures/style.json) | background, location-indicator | — |
| [metrics/tests/location_indicator/one_texture/style.json](../../metrics/tests/location_indicator/one_texture/style.json) | background, location-indicator | — |
| [metrics/tests/location_indicator/query_test/style.json](../../metrics/tests/location_indicator/query_test/style.json) | background, location-indicator | — |
| [metrics/tests/location_indicator/query_test_miss/style.json](../../metrics/tests/location_indicator/query_test_miss/style.json) | background, location-indicator | — |
| [metrics/tests/location_indicator/query_test_no_image/style.json](../../metrics/tests/location_indicator/query_test_no_image/style.json) | background, location-indicator | — |
| [metrics/tests/location_indicator/rotated/style.json](../../metrics/tests/location_indicator/rotated/style.json) | background, location-indicator | — |
| [metrics/tests/location_indicator/tilted/style.json](../../metrics/tests/location_indicator/tilted/style.json) | background, location-indicator | — |
| [metrics/tests/location_indicator/tilted_texture_shift/style.json](../../metrics/tests/location_indicator/tilted_texture_shift/style.json) | background, location-indicator | — |
| [metrics/tests/location_indicator/tilted_texture_shift_bottom_left/style.json](../../metrics/tests/location_indicator/tilted_texture_shift_bottom_left/style.json) | background, location-indicator | — |
| [metrics/tests/location_indicator/tilted_texture_shift_bottom_right/style.json](../../metrics/tests/location_indicator/tilted_texture_shift_bottom_right/style.json) | background, location-indicator | — |
| [metrics/tests/location_indicator/tilted_texture_shift_top_left/style.json](../../metrics/tests/location_indicator/tilted_texture_shift_top_left/style.json) | background, location-indicator | — |
| [metrics/tests/location_indicator/tilted_texture_shift_top_right/style.json](../../metrics/tests/location_indicator/tilted_texture_shift_top_right/style.json) | background, location-indicator | — |
| [metrics/tests/location_indicator/two_textures/style.json](../../metrics/tests/location_indicator/two_textures/style.json) | background, location-indicator | — |
| [metrics/tests/probes/file-size/fail-file-doesnt-match/style.json](../../metrics/tests/probes/file-size/fail-file-doesnt-match/style.json) | circle | geojson |
| [metrics/tests/probes/file-size/fail-size-is-over/style.json](../../metrics/tests/probes/file-size/fail-size-is-over/style.json) | circle | geojson |
| [metrics/tests/probes/file-size/fail-size-is-under/style.json](../../metrics/tests/probes/file-size/fail-size-is-under/style.json) | circle | geojson |
| [metrics/tests/probes/file-size/pass-size-is-in-tolerance-higher/style.json](../../metrics/tests/probes/file-size/pass-size-is-in-tolerance-higher/style.json) | circle | geojson |
| [metrics/tests/probes/file-size/pass-size-is-in-tolerance-lower/style.json](../../metrics/tests/probes/file-size/pass-size-is-in-tolerance-lower/style.json) | circle | geojson |
| [metrics/tests/probes/file-size/pass-size-is-same/style.json](../../metrics/tests/probes/file-size/pass-size-is-same/style.json) | circle | geojson |
| [metrics/tests/probes/gfx/fail-ib-mem-mismatch/style.json](../../metrics/tests/probes/gfx/fail-ib-mem-mismatch/style.json) | — | — |
| [metrics/tests/probes/gfx/fail-negative-framebuffer-count/style.json](../../metrics/tests/probes/gfx/fail-negative-framebuffer-count/style.json) | — | — |
| [metrics/tests/probes/gfx/fail-texture-mem-mismatch/style.json](../../metrics/tests/probes/gfx/fail-texture-mem-mismatch/style.json) | — | — |
| [metrics/tests/probes/gfx/fail-too-few-buffers/style.json](../../metrics/tests/probes/gfx/fail-too-few-buffers/style.json) | — | — |
| [metrics/tests/probes/gfx/fail-too-few-textures/style.json](../../metrics/tests/probes/gfx/fail-too-few-textures/style.json) | — | — |
| [metrics/tests/probes/gfx/fail-too-many-drawcalls/style.json](../../metrics/tests/probes/gfx/fail-too-many-drawcalls/style.json) | — | — |
| [metrics/tests/probes/gfx/fail-vb-mem-mismatch/style.json](../../metrics/tests/probes/gfx/fail-vb-mem-mismatch/style.json) | — | — |
| [metrics/tests/probes/gfx/pass-double-probe/style.json](../../metrics/tests/probes/gfx/pass-double-probe/style.json) | — | — |
| [metrics/tests/probes/gfx/pass-probe-reset/style.json](../../metrics/tests/probes/gfx/pass-probe-reset/style.json) | — | — |
| [metrics/tests/probes/gfx/pass/style.json](../../metrics/tests/probes/gfx/pass/style.json) | — | — |
| [metrics/tests/probes/memory/fail-memory-size-is-too-big/style.json](../../metrics/tests/probes/memory/fail-memory-size-is-too-big/style.json) | circle | geojson |
| [metrics/tests/probes/memory/fail-memory-size-is-too-small/style.json](../../metrics/tests/probes/memory/fail-memory-size-is-too-small/style.json) | circle | geojson |
| [metrics/tests/probes/memory/pass-memory-size-is-same/style.json](../../metrics/tests/probes/memory/pass-memory-size-is-same/style.json) | circle | geojson |
| [metrics/tests/probes/network/fail-requests-transferred/style.json](../../metrics/tests/probes/network/fail-requests-transferred/style.json) | background, line, symbol | vector |
| [metrics/tests/probes/network/fail-requests/style.json](../../metrics/tests/probes/network/fail-requests/style.json) | background, line, symbol | vector |
| [metrics/tests/probes/network/fail-transferred/style.json](../../metrics/tests/probes/network/fail-transferred/style.json) | background, line, symbol | vector |
| [metrics/tests/probes/network/pass/style.json](../../metrics/tests/probes/network/pass/style.json) | background, line, symbol | vector |

### native other fixtures (22)

| Style file | Layer families | Source types |
| --- | --- | --- |
| [test/fixtures/api/annotation.json](../../test/fixtures/api/annotation.json) | fill | vector |
| [test/fixtures/api/empty-zoomed.json](../../test/fixtures/api/empty-zoomed.json) | — | — |
| [test/fixtures/api/empty.json](../../test/fixtures/api/empty.json) | — | — |
| [test/fixtures/api/icon_style.json](../../test/fixtures/api/icon_style.json) | symbol | geojson |
| [test/fixtures/api/query_style.json](../../test/fixtures/api/query_style.json) | symbol | geojson, raster, vector |
| [test/fixtures/api/simple.json](../../test/fixtures/api/simple.json) | background, fill | vector |
| [test/fixtures/api/water.json](../../test/fixtures/api/water.json) | background, fill | vector |
| [test/fixtures/api/water_missing_tiles.json](../../test/fixtures/api/water_missing_tiles.json) | background, fill | vector |
| [test/fixtures/local_glyphs/mixed.json](../../test/fixtures/local_glyphs/mixed.json) | background, line, symbol | geojson |
| [test/fixtures/map/offline/style.json](../../test/fixtures/map/offline/style.json) | background, fill, symbol | vector |
| [test/fixtures/map/online/style.json](../../test/fixtures/map/online/style.json) | background, fill | vector |
| [test/fixtures/map/prefetch/empty.json](../../test/fixtures/map/prefetch/empty.json) | — | — |
| [test/fixtures/map/prefetch/style.json](../../test/fixtures/map/prefetch/style.json) | background, raster | raster |
| [test/fixtures/map/style_update_zoom_dependency/style.json](../../test/fixtures/map/style_update_zoom_dependency/style.json) | background | — |
| [test/fixtures/offline_download/empty.style.json](../../test/fixtures/offline_download/empty.style.json) | — | — |
| [test/fixtures/offline_download/geojson_source.style.json](../../test/fixtures/offline_download/geojson_source.style.json) | — | geojson |
| [test/fixtures/offline_download/inline_source.style.json](../../test/fixtures/offline_download/inline_source.style.json) | fill | vector |
| [test/fixtures/offline_download/mapbox_source.style.json](../../test/fixtures/offline_download/mapbox_source.style.json) | fill | vector |
| [test/fixtures/offline_download/style.json](../../test/fixtures/offline_download/style.json) | background, fill, symbol | image, vector |
| [test/fixtures/resources/style-unused-sources.json](../../test/fixtures/resources/style-unused-sources.json) | symbol | vector |
| [test/fixtures/resources/style_raster.json](../../test/fixtures/resources/style_raster.json) | line, raster, symbol | raster, vector |
| [test/fixtures/resources/style_vector.json](../../test/fixtures/resources/style_vector.json) | background, fill, fill-extrusion, line, symbol | vector |

### native parser fixtures (30)

| Style file | Layer families | Source types |
| --- | --- | --- |
| [test/fixtures/style_parser/center-not-latlong.style.json](../../test/fixtures/style_parser/center-not-latlong.style.json) | — | — |
| [test/fixtures/style_parser/circle-blur.style.json](../../test/fixtures/style_parser/circle-blur.style.json) | circle | vector |
| [test/fixtures/style_parser/circle-color.style.json](../../test/fixtures/style_parser/circle-color.style.json) | circle | vector |
| [test/fixtures/style_parser/circle-opacity.style.json](../../test/fixtures/style_parser/circle-opacity.style.json) | circle | vector |
| [test/fixtures/style_parser/circle-radius.style.json](../../test/fixtures/style_parser/circle-radius.style.json) | circle | vector |
| [test/fixtures/style_parser/colors.style.json](../../test/fixtures/style_parser/colors.style.json) | background, fill | vector |
| [test/fixtures/style_parser/expressions.style.json](../../test/fixtures/style_parser/expressions.style.json) | fill, fill-extrusion, line | vector |
| [test/fixtures/style_parser/font_stacks.json](../../test/fixtures/style_parser/font_stacks.json) | symbol | vector |
| [test/fixtures/style_parser/function-numeric.style.json](../../test/fixtures/style_parser/function-numeric.style.json) | line | vector |
| [test/fixtures/style_parser/function-string-bool-enum.style.json](../../test/fixtures/style_parser/function-string-bool-enum.style.json) | line, symbol | vector |
| [test/fixtures/style_parser/function-type.style.json](../../test/fixtures/style_parser/function-type.style.json) | line | vector |
| [test/fixtures/style_parser/geojson-data-inline.style.json](../../test/fixtures/style_parser/geojson-data-inline.style.json) | — | geojson |
| [test/fixtures/style_parser/geojson-data-url.style.json](../../test/fixtures/style_parser/geojson-data-url.style.json) | — | geojson |
| [test/fixtures/style_parser/geojson-invalid-data.style.json](../../test/fixtures/style_parser/geojson-invalid-data.style.json) | — | geojson |
| [test/fixtures/style_parser/geojson-missing-data.style.json](../../test/fixtures/style_parser/geojson-missing-data.style.json) | — | geojson |
| [test/fixtures/style_parser/geojson-missing-properties.style.json](../../test/fixtures/style_parser/geojson-missing-properties.style.json) | — | geojson |
| [test/fixtures/style_parser/image-coordinates.style.json](../../test/fixtures/style_parser/image-coordinates.style.json) | — | image |
| [test/fixtures/style_parser/image-url.style.json](../../test/fixtures/style_parser/image-url.style.json) | — | image |
| [test/fixtures/style_parser/line-opacity.style.json](../../test/fixtures/style_parser/line-opacity.style.json) | line | vector |
| [test/fixtures/style_parser/line-translate.style.json](../../test/fixtures/style_parser/line-translate.style.json) | line | vector |
| [test/fixtures/style_parser/line-width.style.json](../../test/fixtures/style_parser/line-width.style.json) | line | vector |
| [test/fixtures/style_parser/non-object.style.json](../../test/fixtures/style_parser/non-object.style.json) | — | — |
| [test/fixtures/style_parser/paint-nonobject.style.json](../../test/fixtures/style_parser/paint-nonobject.style.json) | background | — |
| [test/fixtures/style_parser/sprites-missing-fields.style.json](../../test/fixtures/style_parser/sprites-missing-fields.style.json) | — | — |
| [test/fixtures/style_parser/sprites-not-same-ids.style.json](../../test/fixtures/style_parser/sprites-not-same-ids.style.json) | — | — |
| [test/fixtures/style_parser/stop-zoom-value.style.json](../../test/fixtures/style_parser/stop-zoom-value.style.json) | fill | vector |
| [test/fixtures/style_parser/stops-array.style.json](../../test/fixtures/style_parser/stops-array.style.json) | line | vector |
| [test/fixtures/style_parser/text-font.style.json](../../test/fixtures/style_parser/text-font.style.json) | symbol | vector |
| [test/fixtures/style_parser/text-size.style.json](../../test/fixtures/style_parser/text-size.style.json) | symbol | vector |
| [test/fixtures/style_parser/version-not-number.style.json](../../test/fixtures/style_parser/version-not-number.style.json) | — | — |

### platform assets/tests/examples (24)

| Style file | Layer families | Source types |
| --- | --- | --- |
| [platform/android/MapLibreAndroidTestApp/src/androidTest/assets/streets.json](../../platform/android/MapLibreAndroidTestApp/src/androidTest/assets/streets.json) | background, fill, fill-extrusion, line, symbol | vector |
| [platform/android/MapLibreAndroidTestApp/src/main/assets/fill_color_style.json](../../platform/android/MapLibreAndroidTestApp/src/main/assets/fill_color_style.json) | background, fill, line, symbol | vector |
| [platform/android/MapLibreAndroidTestApp/src/main/assets/fill_filter_style.json](../../platform/android/MapLibreAndroidTestApp/src/main/assets/fill_filter_style.json) | background, fill, line, symbol | vector |
| [platform/android/MapLibreAndroidTestApp/src/main/assets/heavy_style.json](../../platform/android/MapLibreAndroidTestApp/src/main/assets/heavy_style.json) | background, fill, fill-extrusion, line, symbol | vector |
| [platform/android/MapLibreAndroidTestApp/src/main/assets/line_filter_style.json](../../platform/android/MapLibreAndroidTestApp/src/main/assets/line_filter_style.json) | background, fill, fill-extrusion, line, symbol | vector |
| [platform/android/MapLibreAndroidTestApp/src/main/assets/numeric_filter_style.json](../../platform/android/MapLibreAndroidTestApp/src/main/assets/numeric_filter_style.json) | background, fill, line, symbol | vector |
| [platform/android/MapLibreAndroidTestApp/src/main/assets/outdoor.json](../../platform/android/MapLibreAndroidTestApp/src/main/assets/outdoor.json) | background, fill, hillshade, line, symbol | raster-dem, vector |
| [platform/android/MapLibreAndroidTestApp/src/main/assets/pastel.json](../../platform/android/MapLibreAndroidTestApp/src/main/assets/pastel.json) | background, fill, line, raster, symbol | raster, vector |
| [platform/android/MapLibreAndroidTestApp/src/main/assets/satellite-hybrid.json](../../platform/android/MapLibreAndroidTestApp/src/main/assets/satellite-hybrid.json) | line, raster, symbol | raster, vector |
| [platform/android/MapLibreAndroidTestApp/src/main/assets/streets.json](../../platform/android/MapLibreAndroidTestApp/src/main/assets/streets.json) | background, fill, fill-extrusion, line, symbol | vector |
| [platform/android/MapLibreAndroidTestApp/src/main/res/raw/demotiles.json](../../platform/android/MapLibreAndroidTestApp/src/main/res/raw/demotiles.json) | background, fill, line, symbol | geojson, vector |
| [platform/android/MapLibreAndroidTestApp/src/main/res/raw/local_style.json](../../platform/android/MapLibreAndroidTestApp/src/main/res/raw/local_style.json) | background, fill, fill-extrusion, line, symbol | vector |
| [platform/android/MapLibreAndroidTestApp/src/main/res/raw/no_bg_style.json](../../platform/android/MapLibreAndroidTestApp/src/main/res/raw/no_bg_style.json) | fill, fill-extrusion, line, symbol | vector |
| [platform/android/MapLibreAndroidTestApp/src/main/res/raw/test_feature_state_style.json](../../platform/android/MapLibreAndroidTestApp/src/main/res/raw/test_feature_state_style.json) | background, circle | geojson |
| [platform/darwin/app/PluginLayerTestStyle.json](../../platform/darwin/app/PluginLayerTestStyle.json) | background, fill, line, plugin-layer-metal-rendering, symbol | geojson, vector |
| [platform/darwin/test/one-liner.json](../../platform/darwin/test/one-liner.json) | — | — |
| [platform/ios/app/fill_filter_style.json](../../platform/ios/app/fill_filter_style.json) | background, fill, line, symbol | vector |
| [platform/ios/app/line_filter_style.json](../../platform/ios/app/line_filter_style.json) | background, fill, fill-extrusion, line, symbol | vector |
| [platform/ios/app/missing_icon.json](../../platform/ios/app/missing_icon.json) | background, circle, symbol | geojson |
| [platform/ios/app/numeric_filter_style.json](../../platform/ios/app/numeric_filter_style.json) | background, fill, line, symbol | vector |
| [platform/ios/benchmark/assets/styles/streets.json](../../platform/ios/benchmark/assets/styles/streets.json) | background, fill, fill-extrusion, line, symbol | vector |
| [platform/macos/app/heatmap.json](../../platform/macos/app/heatmap.json) | background, fill, fill-extrusion, line, symbol | vector |
| [platform/macos/app/wms.json](../../platform/macos/app/wms.json) | raster | raster |
| [platform/node/test/fixtures/style.json](../../platform/node/test/fixtures/style.json) | background, fill | vector |

### plugin examples/other (1)

| Style file | Layer families | Source types |
| --- | --- | --- |
| [plugins/ngon-layer/examples/interpolation.json](../../plugins/ngon-layer/examples/interpolation.json) | background, ngon | geojson |

### plugin render tests (13)

| Style file | Layer families | Source types |
| --- | --- | --- |
| [plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json](../../plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json) | background, ngon | geojson |
| [plugins/ngon-layer/render-tests/ngon/blur-and-opacity/style.json](../../plugins/ngon-layer/render-tests/ngon/blur-and-opacity/style.json) | background, ngon | geojson |
| [plugins/ngon-layer/render-tests/ngon/composite-fractional-zoom/style.json](../../plugins/ngon-layer/render-tests/ngon/composite-fractional-zoom/style.json) | background, ngon | geojson |
| [plugins/ngon-layer/render-tests/ngon/corners-and-rotation/style.json](../../plugins/ngon-layer/render-tests/ngon/corners-and-rotation/style.json) | background, ngon | geojson |
| [plugins/ngon-layer/render-tests/ngon/feature-state/style.json](../../plugins/ngon-layer/render-tests/ngon/feature-state/style.json) | background, ngon | geojson |
| [plugins/ngon-layer/render-tests/ngon/paint-update/style.json](../../plugins/ngon-layer/render-tests/ngon/paint-update/style.json) | background, ngon | geojson |
| [plugins/ngon-layer/render-tests/ngon/pitch-map-map/style.json](../../plugins/ngon-layer/render-tests/ngon/pitch-map-map/style.json) | background, ngon | geojson |
| [plugins/ngon-layer/render-tests/ngon/pitch-map-viewport/style.json](../../plugins/ngon-layer/render-tests/ngon/pitch-map-viewport/style.json) | background, ngon | geojson |
| [plugins/ngon-layer/render-tests/ngon/pitch-viewport-map/style.json](../../plugins/ngon-layer/render-tests/ngon/pitch-viewport-map/style.json) | background, ngon | geojson |
| [plugins/ngon-layer/render-tests/ngon/pitch-viewport-viewport/style.json](../../plugins/ngon-layer/render-tests/ngon/pitch-viewport-viewport/style.json) | background, ngon | geojson |
| [plugins/ngon-layer/render-tests/ngon/tile-boundary-and-multipoint/style.json](../../plugins/ngon-layer/render-tests/ngon/tile-boundary-and-multipoint/style.json) | background, ngon | geojson |
| [plugins/ngon-layer/render-tests/ngon/translation-anchors/style.json](../../plugins/ngon-layer/render-tests/ngon/translation-anchors/style.json) | background, ngon | geojson |
| [plugins/ngon-layer/render-tests/ngon/zero-radius/style.json](../../plugins/ngon-layer/render-tests/ngon/zero-radius/style.json) | background, ngon | geojson |

## Supplemental programmatic style locations

The following 152 source files contain style loading/construction calls. This is a locator index, not proof that all possible dynamically constructed styles were expanded or reviewed individually. Remote URLs and computed strings remain unresolved.

| Source file | Matching line numbers |
| --- | --- |
| [platform/android/MapLibreAndroid/src/test/java/org/maplibre/android/location/LocationComponentPositionManagerTest.kt](../../platform/android/MapLibreAndroid/src/test/java/org/maplibre/android/location/LocationComponentPositionManagerTest.kt#L116) | 116 |
| [platform/android/MapLibreAndroid/src/test/java/org/maplibre/android/location/LocationLayerControllerTest.kt](../../platform/android/MapLibreAndroid/src/test/java/org/maplibre/android/location/LocationLayerControllerTest.kt#L184) | 184, 883 |
| [platform/android/MapLibreAndroid/src/test/java/org/maplibre/android/maps/MapLibreMapTest.kt](../../platform/android/MapLibreAndroid/src/test/java/org/maplibre/android/maps/MapLibreMapTest.kt#L62) | 62, 216, 229, 231, 244, 245 |
| [platform/android/MapLibreAndroid/src/test/java/org/maplibre/android/maps/StyleBuilderTest.kt](../../platform/android/MapLibreAndroid/src/test/java/org/maplibre/android/maps/StyleBuilderTest.kt#L34) | 34 |
| [platform/android/MapLibreAndroid/src/test/java/org/maplibre/android/maps/StyleTest.kt](../../platform/android/MapLibreAndroid/src/test/java/org/maplibre/android/maps/StyleTest.kt#L61) | 61, 67, 68, 69, 75, 76, 84, 99, 109, 119, 129, 138, 149, 160, 177, 189, 201, 212, 223, 236, 250, 263, 264, 265, 279, 296, 309, 310, 311, 313, 320, 323, 324, 332, 346, 361, 385, 388, 391, 409, 438, 467 |
| [platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/location/LocationComponentTest.kt](../../platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/location/LocationComponentTest.kt#L195) | 195, 680, 801, 826, 913 |
| [platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/location/LocationLayerControllerTest.kt](../../platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/location/LocationLayerControllerTest.kt#L507) | 507 |
| [platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/location/utils/StyleChangeIdlingResource.kt](../../platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/location/utils/StyleChangeIdlingResource.kt#L36) | 36 |
| [platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/maps/BaseLayerTest.kt](../../platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/maps/BaseLayerTest.kt#L25) | 25 |
| [platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/maps/NativeMapViewTest.kt](../../platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/maps/NativeMapViewTest.kt#L78) | 78, 79 |
| [platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/activity/EspressoTest.kt](../../platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/activity/EspressoTest.kt#L23) | 23 |
| [platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/geometry/GeoJsonConversionTest.java](../../platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/geometry/GeoJsonConversionTest.java#L47) | 47, 59, 71, 83, 95, 107, 119, 143 |
| [platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/maps/ImageMissingTest.kt](../../platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/maps/ImageMissingTest.kt#L37) | 37, 51, 64, 69, 113 |
| [platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/maps/RemoveUnusedImagesTest.kt](../../platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/maps/RemoveUnusedImagesTest.kt#L41) | 41, 127 |
| [platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/maps/StyleLoadTest.kt](../../platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/maps/StyleLoadTest.kt#L24) | 24, 26 |
| [platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/style/CustomGeometrySourceTest.kt](../../platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/style/CustomGeometrySourceTest.kt#L68) | 68 |
| [platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/style/ExpressionTest.java](../../platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/style/ExpressionTest.java#L264) | 264, 300, 324, 348, 379, 409, 444, 485, 517, 547, 569, 596, 622, 648, 674, 707, 769, 794, 838, 861, 880 |
| [platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/style/GeoJsonSourceTests.java](../../platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/style/GeoJsonSourceTests.java#L53) | 53, 55, 65, 77, 87, 107, 126, 127, 156, 157, 253, 255, 303, 305 |
| [platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/style/LightTest.java](../../platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/style/LightTest.java#L163) | 163 |
| [platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/style/RuntimeStyleTests.java](../../platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/style/RuntimeStyleTests.java#L270) | 270, 323, 326, 387, 400, 407 |
| [platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/style/StyleLoaderTest.kt](../../platform/android/MapLibreAndroidTestApp/src/androidTest/java/org/maplibre/android/testapp/style/StyleLoaderTest.kt#L32) | 32, 53 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/annotation/BulkMarkerActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/annotation/BulkMarkerActivity.kt#L47) | 47 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/annotation/DynamicMarkerChangeActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/annotation/DynamicMarkerChangeActivity.kt#L33) | 33 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/annotation/JsonApiActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/annotation/JsonApiActivity.kt#L55) | 55, 86, 117 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/annotation/PolygonActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/annotation/PolygonActivity.kt#L59) | 59 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/annotation/PolylineActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/annotation/PolylineActivity.kt#L48) | 48 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/annotation/PressForMarkerActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/annotation/PressForMarkerActivity.kt#L47) | 47 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/camera/CameraAnimationTypeActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/camera/CameraAnimationTypeActivity.kt#L71) | 71 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/camera/CameraAnimatorActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/camera/CameraAnimatorActivity.kt#L44) | 44 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/camera/CameraPositionActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/camera/CameraPositionActivity.kt#L51) | 51 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/camera/GestureDetectorActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/camera/GestureDetectorActivity.kt#L57) | 57 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/camera/LatLngBoundsActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/camera/LatLngBoundsActivity.kt#L9) | 9, 57, 74 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/camera/ManualZoomActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/camera/ManualZoomActivity.kt#L31) | 31 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/camera/MaxMinZoomActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/camera/MaxMinZoomActivity.kt#L30) | 30, 39 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/camera/ScrollByActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/camera/ScrollByActivity.kt#L56) | 56 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/customlayer/CustomLayerActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/customlayer/CustomLayerActivity.kt#L50) | 50 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/espresso/DeviceIndependentTestActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/espresso/DeviceIndependentTestActivity.kt#L24) | 24 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/espresso/PixelTestActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/espresso/PixelTestActivity.kt#L30) | 30 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/events/ObserverActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/events/ObserverActivity.kt#L87) | 87 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/feature/FeatureStateActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/feature/FeatureStateActivity.kt#L62) | 62, 81, 100 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/feature/FeatureStateVectorActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/feature/FeatureStateVectorActivity.kt#L69) | 69, 88, 105 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/feature/QueryRenderedFeaturesBoxCountActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/feature/QueryRenderedFeaturesBoxCountActivity.kt#L45) | 45 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/feature/QueryRenderedFeaturesBoxHighlightActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/feature/QueryRenderedFeaturesBoxHighlightActivity.kt#L72) | 72 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/feature/QueryRenderedFeaturesBoxSymbolCountActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/feature/QueryRenderedFeaturesBoxSymbolCountActivity.kt#L47) | 47 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/feature/QueryRenderedFeaturesPropertiesActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/feature/QueryRenderedFeaturesPropertiesActivity.kt#L65) | 65 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/feature/QuerySourceFeaturesActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/feature/QuerySourceFeaturesActivity.kt#L41) | 41, 65 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/fragment/FragmentBackStackActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/fragment/FragmentBackStackActivity.kt#L63) | 63 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/fragment/MapFragmentActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/fragment/MapFragmentActivity.kt#L71) | 71 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/fragment/MultiMapActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/fragment/MultiMapActivity.kt#L32) | 32 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/fragment/NestedViewPagerActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/fragment/NestedViewPagerActivity.kt#L118) | 118, 127, 136 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/fragment/SupportMapFragmentActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/fragment/SupportMapFragmentActivity.kt#L71) | 71 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/fragment/ViewPagerActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/fragment/ViewPagerActivity.kt#L98) | 98 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/imagegenerator/PrintActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/imagegenerator/PrintActivity.kt#L35) | 35 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/imagegenerator/SnapshotActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/imagegenerator/SnapshotActivity.kt#L47) | 47 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/infowindow/DynamicInfoWindowAdapterActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/infowindow/DynamicInfoWindowAdapterActivity.kt#L61) | 61 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/infowindow/InfoWindowActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/infowindow/InfoWindowActivity.kt#L70) | 70 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/infowindow/InfoWindowAdapterActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/infowindow/InfoWindowAdapterActivity.kt#L35) | 35 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/location/BasicLocationPulsingCircleActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/location/BasicLocationPulsingCircleActivity.kt#L52) | 52, 136 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/location/CustomizedLocationPulsingCircleActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/location/CustomizedLocationPulsingCircleActivity.kt#L88) | 88, 204 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/location/LocationComponentActivationActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/location/LocationComponentActivationActivity.kt#L65) | 65 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/location/LocationFragmentActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/location/LocationFragmentActivity.kt#L111) | 111 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/location/LocationMapChangeActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/location/LocationMapChangeActivity.kt#L30) | 30, 69 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/location/LocationModesActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/location/LocationModesActivity.kt#L123) | 123, 260 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/location/ManualLocationUpdatesActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/location/ManualLocationUpdatesActivity.kt#L106) | 106 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/BottomSheetActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/BottomSheetActivity.kt#L128) | 128, 224 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/DebugModeActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/DebugModeActivity.kt#L85) | 85, 160 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/DoubleMapActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/DoubleMapActivity.kt#L67) | 67, 87 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/LatLngBoundsForCameraActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/LatLngBoundsForCameraActivity.kt#L31) | 31 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/LocalGlyphActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/LocalGlyphActivity.kt#L25) | 25 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/MapChangeActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/MapChangeActivity.kt#L92) | 92 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/MapInDialogActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/MapInDialogActivity.kt#L44) | 44 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/MapPaddingActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/MapPaddingActivity.kt#L29) | 29 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/OverlayMapActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/OverlayMapActivity.kt#L27) | 27 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/SimpleMapActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/SimpleMapActivity.kt#L35) | 35, 42 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/SurfaceRecyclerViewActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/SurfaceRecyclerViewActivity.kt#L147) | 147 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/VisibilityChangeActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/maplayout/VisibilityChangeActivity.kt#L32) | 32 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/offline/MergeOfflineRegionsActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/offline/MergeOfflineRegionsActivity.kt#L34) | 34, 88 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/offline/OfflineActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/offline/OfflineActivity.kt#L73) | 73 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/options/MapOptionsRuntimeActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/options/MapOptionsRuntimeActivity.kt#L62) | 62 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/options/MapOptionsXmlActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/options/MapOptionsXmlActivity.kt#L30) | 30 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/render/RenderTestActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/render/RenderTestActivity.kt#L81) | 81, 83, 87, 320, 321, 334, 345, 350 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/render/RenderTestDefinition.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/render/RenderTestDefinition.kt#L9) | 9, 55, 65 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/snapshot/MapSnapshotterLocalStyleActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/snapshot/MapSnapshotterLocalStyleActivity.kt#L41) | 41, 51 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/sources/PMTilesActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/sources/PMTilesActivity.kt#L27) | 27, 52, 67, 80 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/sources/VectorTileActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/sources/VectorTileActivity.kt#L34) | 34, 51 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/stability/NavigationMap.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/stability/NavigationMap.kt#L316) | 316 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/storage/CustomHttpRequestImpl.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/storage/CustomHttpRequestImpl.kt#L39) | 39 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/storage/UrlTransformActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/storage/UrlTransformActivity.kt#L47) | 47 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/AnimatedImageSourceActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/AnimatedImageSourceActivity.kt#L52) | 52 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/AnimatedSymbolLayerActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/AnimatedSymbolLayerActivity.kt#L51) | 51, 294 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/BuildingFillExtrusionActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/BuildingFillExtrusionActivity.kt#L44) | 44, 69 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/CircleLayerActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/CircleLayerActivity.kt#L52) | 52, 86, 148, 201, 215, 237 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/CollectionUpdateOnStyleChange.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/CollectionUpdateOnStyleChange.kt#L51) | 51, 68, 79 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/CustomSpriteActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/CustomSpriteActivity.kt#L40) | 40 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/CustomVectorSourceActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/CustomVectorSourceActivity.kt#L53) | 53, 76 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/DataDrivenStyleActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/DataDrivenStyleActivity.kt#L45) | 45, 557 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/DistanceExpressionActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/DistanceExpressionActivity.kt#L57) | 57 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/DraggableMarkerActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/DraggableMarkerActivity.kt#L73) | 73 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/FillExtrusionActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/FillExtrusionActivity.kt#L33) | 33, 46 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/FillExtrusionStyleTestActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/FillExtrusionStyleTestActivity.kt#L28) | 28 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/GeoJsonClusteringActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/GeoJsonClusteringActivity.kt#L74) | 74 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/GradientLineActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/GradientLineActivity.kt#L38) | 38 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/GridSourceActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/GridSourceActivity.kt#L104) | 104 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/HeatmapLayerActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/HeatmapLayerActivity.kt#L35) | 35 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/HillshadeLayerActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/HillshadeLayerActivity.kt#L29) | 29 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/ImageInLabelActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/ImageInLabelActivity.kt#L37) | 37, 83 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/NoStyleActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/NoStyleActivity.kt#L35) | 35 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/RealTimeGeoJsonActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/RealTimeGeoJsonActivity.kt#L65) | 65, 82 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/RuntimeStyleActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/RuntimeStyleActivity.kt#L91) | 91, 389, 418, 513, 556, 560, 570, 606, 649, 679 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/RuntimeStyleTimingTestActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/RuntimeStyleTimingTestActivity.kt#L41) | 41 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/StretchableImageActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/StretchableImageActivity.kt#L41) | 41, 81, 100, 101 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/StyleFileActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/StyleFileActivity.kt#L26) | 26, 38, 46, 65 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/StyleUrlActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/StyleUrlActivity.kt#L52) | 52, 58 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/SymbolGeneratorActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/SymbolGeneratorActivity.kt#L54) | 54, 180, 266 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/SymbolLayerActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/SymbolLayerActivity.kt#L154) | 154 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/ZoomFunctionSymbolLayerActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/style/ZoomFunctionSymbolLayerActivity.kt#L56) | 56, 58, 97, 116 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/surfaceview/SurfaceViewResizeActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/surfaceview/SurfaceViewResizeActivity.kt#L42) | 42 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/telemetry/PerformanceMeasurementActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/telemetry/PerformanceMeasurementActivity.kt#L39) | 39 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/textureview/TextureViewAnimationActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/textureview/TextureViewAnimationActivity.kt#L45) | 45 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/textureview/TextureViewResizeActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/textureview/TextureViewResizeActivity.kt#L43) | 43 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/textureview/TextureViewTransparentBackgroundActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/textureview/TextureViewTransparentBackgroundActivity.kt#L53) | 53, 54 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/turf/MapSnapshotterWithinExpression.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/turf/MapSnapshotterWithinExpression.kt#L110) | 110, 192, 193, 253 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/turf/PhysicalUnitCircleActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/turf/PhysicalUnitCircleActivity.kt#L63) | 63 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/turf/WithinExpressionActivity.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/activity/turf/WithinExpressionActivity.kt#L100) | 100, 211 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/utils/CoroutineUtils.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/utils/CoroutineUtils.kt#L35) | 35 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/utils/GeoParseUtil.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/utils/GeoParseUtil.kt#L32) | 32 |
| [platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/utils/OfflineUtils.kt](../../platform/android/MapLibreAndroidTestApp/src/main/java/org/maplibre/android/testapp/utils/OfflineUtils.kt#L13) | 13 |
| [platform/darwin/test/MLNDocumentationExampleTests.swift](../../platform/darwin/test/MLNDocumentationExampleTests.swift#L264) | 264, 292, 311, 327, 352, 377, 397, 417, 438 |
| [platform/darwin/test/MLNDocumentationGuideTests.swift](../../platform/darwin/test/MLNDocumentationGuideTests.swift#L172) | 172, 325 |
| [platform/darwin/test/MLNMapViewTests.m](../../platform/darwin/test/MLNMapViewTests.m#L188) | 188, 189, 192, 197, 207, 208, 211, 213, 226, 229, 241, 247, 248, 259, 262 |
| [platform/darwin/test/MLNStyleTests.mm](../../platform/darwin/test/MLNStyleTests.mm#L563) | 563, 564, 569 |
| [platform/ios/test/MLNFeatureStateTests.swift](../../platform/ios/test/MLNFeatureStateTests.swift#L69) | 69 |
| [render-test/parser.cpp](../../render-test/parser.cpp#L782) | 782, 910 |
| [render-test/runner.cpp](../../render-test/runner.cpp#L667) | 667 |
| [test/api/annotations.test.cpp](../../test/api/annotations.test.cpp#L54) | 54, 75, 90, 105, 118, 139, 164, 173, 186, 200, 214, 232, 249, 266, 283, 296, 314, 327, 335, 341, 348, 361, 397, 435, 486, 573, 589, 607 |
| [test/api/custom_geometry_source.test.cpp](../../test/api/custom_geometry_source.test.cpp#L27) | 27, 65, 69 |
| [test/api/custom_layer.test.cpp](../../test/api/custom_layer.test.cpp#L105) | 105, 107, 112 |
| [test/api/query.test.cpp](../../test/api/query.test.cpp#L28) | 28, 65 |
| [test/api/recycle_map.cpp](../../test/api/recycle_map.cpp#L40) | 40, 42 |
| [test/gl/context.test.cpp](../../test/gl/context.test.cpp#L99) | 99 |
| [test/map/map.test.cpp](../../test/map/map.test.cpp#L113) | 113, 164, 402, 409, 498, 533, 534, 573, 644, 661, 672, 684, 689, 733, 737, 745, 749, 778, 820, 893, 911, 947, 964, 990, 1007, 1024, 1048, 1178, 1181, 1190, 1266, 1314, 1378, 1576, 1606, 1625, 1697, 1710, 1739, 1819, 1823, 1952, 1979, 1985, 2020, 2030, 2034, 2052 |
| [test/map/map_snapshotter.test.cpp](../../test/map/map_snapshotter.test.cpp#L33) | 33, 36, 62, 133 |
| [test/map/prefetch.test.cpp](../../test/map/prefetch.test.cpp#L58) | 58, 59 |
| [test/plugin/plugin.test.cpp](../../test/plugin/plugin.test.cpp#L216) | 216 |
| [test/plugin/rendering.test.cpp](../../test/plugin/rendering.test.cpp#L158) | 158, 186, 200 |
| [test/renderer/shader_registry.test.cpp](../../test/renderer/shader_registry.test.cpp#L231) | 231, 234, 265, 268, 323, 326 |
| [test/storage/sync_file_source.test.cpp](../../test/storage/sync_file_source.test.cpp#L59) | 59 |
| [test/style/style.test.cpp](../../test/style/style.test.cpp#L30) | 30, 33, 37, 41, 46, 50, 54, 58, 62, 66, 80, 100, 102, 137, 146 |
| [test/style/style_layer.test.cpp](../../test/style/style_layer.test.cpp#L334) | 334, 337, 341, 355, 359 |
| [test/text/local_glyph_rasterizer.test.cpp](../../test/text/local_glyph_rasterizer.test.cpp#L66) | 66, 79, 87, 98, 107, 117, 133 |
| [test/tile/tile_lod.test.cpp](../../test/tile/tile_lod.test.cpp#L41) | 41, 68 |
| [test/util/action_journal.test.cpp](../../test/util/action_journal.test.cpp#L125) | 125, 216, 441 |

## Initialized dependency revisions

All 63 visited dependency checkouts are listed, including those with zero relevant test JSON. The count is the tracked test/fixture/benchmark subset, not every dependency JSON file.

| Dependency | Commit | Test JSON/GeoJSON files |
| --- | --- | ---: |
| [docs/doxygen/doxygen-awesome-css](../../docs/doxygen/doxygen-awesome-css) | `a3c119b4797be2039761ec1fa0731f038e3026f6` | 0 |
| [platform/windows/vendor/vcpkg](../../platform/windows/vendor/vcpkg) | `ce613c41372b23b1f51333815feb3edd87ef8a8b` | 82 |
| [vendor/PMTiles](../../vendor/PMTiles) | `e232df55745642b39f0cc1edfc85df2633bce29c` | 2 |
| [vendor/Vulkan-Headers](../../vendor/Vulkan-Headers) | `015e25c3c91b70eb1a754d36fb14c4ba6ad9b0b9` | 0 |
| [vendor/VulkanMemoryAllocator](../../vendor/VulkanMemoryAllocator) | `7942b798289f752dc23b0a79516fd8545febd718` | 0 |
| [vendor/args](../../vendor/args) | `016aa8a054dd88b8e1d2a020466ce38f310444cc` | 0 |
| [vendor/benchmark](../../vendor/benchmark) | `344117638c8ff7e239044fd0fa7085839fc03021` | 8 |
| [vendor/boost](../../vendor/boost) | `c6eb4cbc932c74c1d341d6eece8eda2da018d1e0` | 0 |
| [vendor/cpp-httplib](../../vendor/cpp-httplib) | `30b7732565c4630263b43c34267a605a1511f794` | 0 |
| [vendor/earcut.hpp](../../vendor/earcut.hpp) | `0d0897a9dc462edf6396aedb335ddeb4aa302b78` | 0 |
| [vendor/earcut.hpp/glfw](../../vendor/earcut.hpp/glfw) | `90e22947c60b0c1fa47cf1496790ce1942e4a5d8` | 0 |
| [vendor/eternal](../../vendor/eternal) | `dd2f5b9ff38fcd36b59efd9d289127fa73efc6cb` | 0 |
| [vendor/eternal/vendor/benchmark](../../vendor/eternal/vendor/benchmark) | `bb15a4e3bf4c5941ee7124d284ed9ef96e9a1c68` | 5 |
| [vendor/expected-lite](../../vendor/expected-lite) | `3583e95bad46f5af7d8a826632bc68a6f7281551` | 0 |
| [vendor/filesystem](../../vendor/filesystem) | `9fda7b0afbd0640f482f4aea8720a8c0afd18740` | 0 |
| [vendor/freetype](../../vendor/freetype) | `42608f77f20749dd6ddc9e0536788eaad70ea4b5` | 0 |
| [vendor/freetype/subprojects/dlg](../../vendor/freetype/subprojects/dlg) | `72dfcc858c040c54a6a0b88fcb7e70ee186d3167` | 0 |
| [vendor/glslang](../../vendor/glslang) | `a92c61f8456fa9731c0b000a2c6fc52a740c2be7` | 0 |
| [vendor/googletest](../../vendor/googletest) | `b514bdc898e2951020cbdca1304b75f5950d1f59` | 0 |
| [vendor/harfbuzz](../../vendor/harfbuzz) | `c1eb66d4159fec311334aee5c0a59384491d3989` | 0 |
| [vendor/kdbush.hpp](../../vendor/kdbush.hpp) | `e1e847bfe97c8cdc09edbe87a4e74babc512d18d` | 0 |
| [vendor/kdbush.hpp/.mason](../../vendor/kdbush.hpp/.mason) | `1f9a7bb04855ad9ce673e0e27084eddd690b2752` | 0 |
| [vendor/maplibre-native-base/deps/cheap-ruler-cpp](../../vendor/maplibre-native-base/deps/cheap-ruler-cpp) | `2778eb89cb3b078d31ce225c7592360d6d9bb0a0` | 0 |
| [vendor/maplibre-native-base/deps/geojson-vt-cpp](../../vendor/maplibre-native-base/deps/geojson-vt-cpp) | `b0f25fec9eb068cdf6fb9556b3cd11fd3694abf0` | 21 |
| [vendor/maplibre-native-base/deps/geojson-vt-cpp/.mason](../../vendor/maplibre-native-base/deps/geojson-vt-cpp/.mason) | `9e7f1d8d54ac6c60d09b9c4744d20dbfbc7bc860` | 0 |
| [vendor/maplibre-native-base/deps/geojson.hpp](../../vendor/maplibre-native-base/deps/geojson.hpp) | `ca4638c545183d9c138464d93f2c20975c8c4808` | 22 |
| [vendor/maplibre-native-base/deps/geojson.hpp/.mason](../../vendor/maplibre-native-base/deps/geojson.hpp/.mason) | `b1582f56531830b7e7015d291f8f0158e88c2246` | 0 |
| [vendor/maplibre-native-base/deps/geometry.hpp](../../vendor/maplibre-native-base/deps/geometry.hpp) | `a5571a3ace5853e0d1e8d5fbdc87163c824ebeb7` | 0 |
| [vendor/maplibre-native-base/deps/jni.hpp](../../vendor/maplibre-native-base/deps/jni.hpp) | `57ca9ed4bbeb22ed8d20a55063dcaa217ba47f42` | 0 |
| [vendor/maplibre-native-base/deps/pixelmatch-cpp](../../vendor/maplibre-native-base/deps/pixelmatch-cpp) | `61f433cb485d6b08dc7fe97ae5f8717007c7bda1` | 0 |
| [vendor/maplibre-native-base/deps/shelf-pack-cpp](../../vendor/maplibre-native-base/deps/shelf-pack-cpp) | `450f25f710c346f9c1583eab2749e011081ed20e` | 0 |
| [vendor/maplibre-native-base/deps/variant](../../vendor/maplibre-native-base/deps/variant) | `a2a4858345423a760eca300ec42acad1ad123aa3` | 0 |
| [vendor/maplibre-native-base/deps/variant/.mason](../../vendor/maplibre-native-base/deps/variant/.mason) | `6adb140160cb549400f73ea35c1d9eb5782210e0` | 0 |
| [vendor/maplibre-tile-spec](../../vendor/maplibre-tile-spec) | `751ebb1837e4fd03c9074b91fce24727013c1868` | 498 |
| [vendor/maplibre-tile-spec/cpp/vendor/earcut](../../vendor/maplibre-tile-spec/cpp/vendor/earcut) | `f36ced7e50254738c4e5af1a239f5fb7b1094007` | 0 |
| [vendor/maplibre-tile-spec/cpp/vendor/earcut/vendor/benchmark](../../vendor/maplibre-tile-spec/cpp/vendor/earcut/vendor/benchmark) | `2948b6a2e61ccabecc952c24794c6960d86c9ed6` | 10 |
| [vendor/maplibre-tile-spec/cpp/vendor/earcut/vendor/glfw](../../vendor/maplibre-tile-spec/cpp/vendor/earcut/vendor/glfw) | `7d5a16ce714f0b5f4efa3262de22e4d948851525` | 0 |
| [vendor/maplibre-tile-spec/cpp/vendor/earcut/vendor/googletest](../../vendor/maplibre-tile-spec/cpp/vendor/earcut/vendor/googletest) | `0934b7e112354d609133d2c5f973c402c9efc9b9` | 0 |
| [vendor/maplibre-tile-spec/cpp/vendor/fsst](../../vendor/maplibre-tile-spec/cpp/vendor/fsst) | `75b2e7535392d5ca5eaec937259388d89e38ba4d` | 0 |
| [vendor/maplibre-tile-spec/cpp/vendor/googletest](../../vendor/maplibre-tile-spec/cpp/vendor/googletest) | `c00fd25b71a17e645e4567fcb465c3fa532827d2` | 0 |
| [vendor/maplibre-tile-spec/cpp/vendor/json](../../vendor/maplibre-tile-spec/cpp/vendor/json) | `9cca280a4d0ccf0c08f47a99aa71d1b0e52f8d03` | 0 |
| [vendor/maplibre-tile-spec/test/mvt-fixtures](../../vendor/maplibre-tile-spec/test/mvt-fixtures) | `4b329c12f6ceb8bb442f09726cb1957b0add0c3a` | 148 |
| [vendor/maplibre-tile-spec/test/mvt-fixtures/vector-tile-spec](../../vendor/maplibre-tile-spec/test/mvt-fixtures/vector-tile-spec) | `452ef14841f3e1288de8b355f722682b63c1b13f` | 0 |
| [vendor/metal-cpp](../../vendor/metal-cpp) | `27c4382b7151d55a51692cdcb27aaa98752240de` | 0 |
| [vendor/polylabel](../../vendor/polylabel) | `6ed5f1aef3510a6c43d7d9348078948a7e074868` | 2 |
| [vendor/polylabel/.mason](../../vendor/polylabel/.mason) | `1f9a7bb04855ad9ce673e0e27084eddd690b2752` | 0 |
| [vendor/protozero](../../vendor/protozero) | `f379578a3f7c8162aac0ac31c2696de09a5b5f93` | 0 |
| [vendor/rapidjson](../../vendor/rapidjson) | `27c3a8dc0e2c9218fe94986d249a12b5ed838f1d` | 58 |
| [vendor/rapidjson/thirdparty/gtest](../../vendor/rapidjson/thirdparty/gtest) | `ba96d0b1161f540656efdaed035b3c062b60e006` | 0 |
| [vendor/supercluster](../../vendor/supercluster) | `e5ba492754f865b475577fd85eecc3be70e050ce` | 1 |
| [vendor/supercluster/.mason](../../vendor/supercluster/.mason) | `e78d6d7c729f05e53a77f6156056974d13b8c32f` | 0 |
| [vendor/unique_resource](../../vendor/unique_resource) | `cba309e92ec79a95be2aa5a324a688a06af8d40a` | 0 |
| [vendor/unordered_dense](../../vendor/unordered_dense) | `729896c7ba8bbd9da5573679270133086d05b5dd` | 0 |
| [vendor/vector-tile](../../vendor/vector-tile) | `8ad3bf0b78bf9cb0dba0abaccec0c180e1c27ab3` | 0 |
| [vendor/vector-tile/bench/mvt-bench-fixtures](../../vendor/vector-tile/bench/mvt-bench-fixtures) | `77758e86720801c5fb7db7a2c578bfe137eb8872` | 0 |
| [vendor/vector-tile/test/mvt-fixtures](../../vendor/vector-tile/test/mvt-fixtures) | `2bf95cf80be810e1b20b92f178b26a739151a929` | 0 |
| [vendor/wagyu](../../vendor/wagyu) | `9c87e553a51170e4ce85fd2ef3160bc3434eb751` | 150 |
| [vendor/wagyu/tests/geometry-test-data](../../vendor/wagyu/tests/geometry-test-data) | `a623c19a91947a9d29f9ec5625ce620ab42325dc` | 13 |
| [vendor/webgpu-cpp](../../vendor/webgpu-cpp) | `b9507d9753960e6ca0adabd7e91c97d8c8f6e2de` | 0 |
| [vendor/wgpu-native](../../vendor/wgpu-native) | `d2e3330ade4ae1bb238d76b485926f067e7ee64c` | 0 |
| [vendor/wgpu-native/examples/vendor/glfw](../../vendor/wgpu-native/examples/vendor/glfw) | `b35641f4a3c62aa86a0b3c983d163bc0fe36026d` | 0 |
| [vendor/wgpu-native/ffi/webgpu-headers](../../vendor/wgpu-native/ffi/webgpu-headers) | `7d3186c3dd2c708703524027b46b8703534ab3cc` | 1 |
| [vendor/zip-archive](../../vendor/zip-archive) | `b145c6f3619619fb48715c479643e2f2e0e42d2d` | 0 |
