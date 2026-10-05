# Native plugin API v1 review

**Review revision:** R2  
**Reviewed commit:** `b60be9a7ef3bf8e27911c1d5cd9926edb66b8cd5`  
**Review date:** 2026-09-14  
**Previous revision:** R1, 2026-09-13, `af7905cfd2ae0cb6446c18cfbcd52887ae773c75`  
**Scope:** the new C ABI in `include/mln/plugin/plugin_api.h`, registration, style properties, worker layout, paint binders, drawables/shaders, queries, example plugin, build/platform integration, and the repository's tracked style corpus.

The API is a useful foundation for procedural marks built from source geometry. It already supports host-evaluated camera, feature, composite and feature-state paint expressions. R2 confirms source fixes for shader identity (F01), registration after renderer initialization (F02), and normalized byte attributes (F03), with new regression tests. Remaining defects include missing context for geometric paint expressions, rendered-feature queries that disagree with the drawing, and Darwin layer enumeration. The fixed items have not been runtime-validated during this refresh.

It is also substantially narrower than an extension mechanism for the entire builtin layer catalog. Plugin-defined layout properties, feature attributes in worker callbacks, textures, glyphs, placement, multiple render passes, source-free drawing and persistent instances are absent. Those are API evolution decisions, distinct from defects in promised functionality. **Existing builtin styles still use their builtin implementations.** A finding that a plugin cannot reproduce a symbol or hillshade style does not establish a regression in ordinary symbol or hillshade rendering.

The companion [complete style inventory](plugin-api-style-inventory.md) enumerates every identified style file, every observed layer/property/expression/operation family, all 175 integration render folders and 30 query folders, the 13 n-gon fixtures, parser negatives, exclusions, supplemental programmatic style locations and dependency revisions.

## Revision history and status policy

| Review | Date | Source revision | Change in review |
| --- | --- | --- | --- |
| R1 | 2026-09-13 | `af7905cfd2ae0cb6446c18cfbcd52887ae773c75` | Initial API review and 1,572-file style inventory. |
| **R2 (current)** | **2026-09-14** | **`b60be9a7ef3bf8e27911c1d5cd9926edb66b8cd5`** | Rechecked the revision delta and all issue statuses; F01–F03 resolved in source, C02 partially addressed; refreshed inventory and `semiliteral` coverage. |

All individual statuses below are **as of R2 / the reviewed commit**, not inferred from commit titles alone. IDs and the original failure descriptions are retained for continuity.

- **Resolved in source:** the original failure mechanism is corrected and a focused regression test is present; execution has not been verified in this refresh.
- **Open:** the documented defect or capability gap remains; controlling implementation paths were compared with R1 and rechecked where changed.
- **Partially addressed:** some original coverage is now present, but the listed remainder is still open.
- **Unverified:** a source-traced hypothesis still requires a runtime reproduction; it is neither closed nor promoted to a confirmed defect.

For F01–F21: **3 resolved in source, 16 open implementation defects, 1 open ABI-policy gap (F18), and 1 unverified timing hypothesis (F20)**. G01–G12, B01–B03 and E01 remain open. C01/C03/C04/C05 remain open; C02 is partially addressed. The new tests raise the core C-ABI test count from 13 to 17; they do not establish a passing backend run in this review.

The relevant fix commits are `afa51ff05383` (F01), `18efc91d6b65` (F02), and `348de031fe1e` (F03), followed by formatting and the merge from `main`. Property evaluation, binders, layout, registration validation, the public C ABI, and Darwin layer-peer/enumeration code have no changes that resolve the remaining findings. The render-layer delta changes shader lookup/registration and normalized attribute mapping, while its transition, paint and query logic remains unchanged.

## Evidence and completeness

R1 used three independent read-only investigations followed by synthesis. R2 compared the complete commit delta, inspected each fix and its assertions, rechecked unchanged controlling paths for the other issues, refreshed source-line links, and reran the full fixture inventory. “Verified implementation defect” means a deterministic mismatch established by tracing the current source and contract; **it does not mean a runtime reproduction was executed**. Confidence is high unless stated otherwise. Severity measures consequence when the concrete use case is exercised, not how often users exercise it. Medium/low severity ABI gaps may still block an entire application that depends on that facility. F20 is explicitly a timing hypothesis requiring first-frame execution.

Both revisions used strict-JSON inventory/report scripts and inspected the native tests and render harness. R2 also inspected all four new `PluginRendering` tests and the merged `semiliteral` implementation, conversion/dependency tests and fixtures. No builds, C++ unit tests, shader compilations, renders, expression evaluator probes, native parser tests, query tests or platform tests were run for this review. Document checks cover links, coverage accounting, issue-status completeness and source locations. Proposed JSON and regression recipes below are **unexecuted test cases**, not recorded outputs. Existing expected images demonstrate authored test coverage, not a current passing run.

At R2, the reviewed checkout matched the stated commit apart from the user's existing `platform/glfw/CMakeLists.txt` modification. That modification, untracked `.clangd`, and `bloaty/` were excluded and left untouched. No production code was changed. All 63 visited submodules were initialized; no content was fetched. Network style/source URLs were not resolved.

| Inventory measure | Count and interpretation |
| --- | --- |
| Candidate JSON/GeoJSON | 10,450: 9,384 root tracked JSON, 45 root tracked GeoJSON, 1,021 initialized dependency test files |
| Identified styles | 1,573 files: 1,543 ordinary style-shaped documents and 30 parser fixture documents; 1,517 distinct normalized JSON hashes |
| Integration styles | 1,299 render styles in 175 family folders; 126 query styles in 30 family folders, with 126 expected query JSON files |
| Other style scopes | 42 metrics test styles, 24 platform assets/examples, 22 other native fixtures, 13 plugin render styles, 7 archived-scope style documents, 6 shared styles, 3 benchmarks, 1 plugin example, plus 30 parser styles |
| Style contents | 4,999 layer instances, 1,633 source instances, 13 layer types; includes inline `addLayer`, `addSource`, and `setStyle` objects |
| Properties | 163 family/section/property keys in objects, 93 mutation occurrences; 164 keys including one mutation whose externally loaded layer family is unresolved |
| Expressions | 50 operators in styles; 481 raw expression fixtures covering 84 operators; 2,121 legacy function occurrences in 226 styles |
| Non-style accounting | 8,395 categorized non-style exclusions, plus 481 raw expression fixtures and one strict-JSON parse error |
| Supplemental construction | 152 source-code files located by style loading/construction calls; computed programmatic styles were not expanded |

The sole strict-JSON failure is the intentional dependency input [geojson.hpp `invalid.json`](../../vendor/maplibre-native-base/deps/geojson.hpp/test/fixtures/invalid.json#L1), at line 1, column 10. It is not a failed style parse. All 30 native parser style fixtures remain included: 25 have expected diagnostic entries, four have explicit empty diagnostic logs, and `font_stacks.json` has no companion diagnostic file. The inventory retains intentionally invalid styles rather than treating valid JSON as proof of valid style syntax. Historical metrics, source geometry, expected outputs, sprite metadata and configuration are categorized separately.

Completeness is finite: every tracked candidate and every identified JSON style family is accounted for. There is no claim to have manually executed all fixtures or to prove an unbounded set of every possible plugin use case. Source-generated styles, external content and conceptual families are treated separately. The companion's property line anchors identify the first occurrence of a key in a file, not every repeated occurrence.

### R2 corpus and expression delta

The merge from `main` adds **one render style, 13 raw expression integration fixtures, and two expression-equality inputs**. The spec reference is also updated. There are now 1,573 style files, 481 raw expression fixtures, 50 operators observed in styles and 84 in raw fixtures. The new style introduces `semiliteral` and the previously raw-fixture-only `length` into the style operator set; this does not mean `length` is a new native operator. The inventory walker now traverses each expression inside a `semiliteral` array, while continuing to skip `literal` payloads. The complete refreshed ledgers are in the companion inventory. The 13 n-gon image fixtures are unchanged; the newly added host rendering cases construct styles in C++, so they are counted as tests and a supplemental source locator, not as additional JSON style files.

[`Semiliteral`](../../src/mln/style/expression/semiliteral.cpp#L17) evaluates an array of child expressions and combines their dependencies. It is available through the host property evaluator: a declared feature-driven FLOAT2 paint can use a typed result such as:

```json
{"ngon-translate": ["semiliteral", [["number", ["get", "dx"]], ["number", ["get", "dy"]]]]}
```

This is a source-supported, unexecuted plugin example. The child `number` assertions matter: the new [native tests](../../test/style/expression/semiliteral.test.cpp#L31) reject untyped `get` children for a statically typed two-number property, wrong lengths, mixed types and a direct nested `zoom`. Native zoom placement rules and plugin dependency capabilities still apply. The new [text-offset style](../../metrics/integration/render-tests/text-offset/semiliteral/style.json) uses builtin symbol layout; it does not supply plugin layout properties, glyph resources or additional final property types. G01, G02 and G06 therefore remain open. No plugin-specific `semiliteral` render/binding test was added.

## What is already implemented

Registration copies descriptors and nested metadata, validates ordinary conflicts, and publishes the prepared layer-factory batch under locks. Layout output is copied into host-owned buffers before the temporary layout handle is destroyed. Failure paths correctly destroy a nonnull handle, even when creation reports failure. The [registration publication](../../src/mln/plugin/plugin_registry.cpp#L589), [factory batch](../../src/mln/layermanager/layer_manager.cpp#L36), and [layout ownership tests](../../test/plugin/layout.test.cpp#L207) support these positive conclusions.

Paint uses native typed conversion and expression evaluation. With the relevant declared capabilities and a shader binding, `get`, `has`, `id`, `geometry-type`, `properties`, nested lookups, arithmetic, `case`, `match`, lexical `let`/`var`, legacy functions, source expressions, camera expressions and composite expressions can influence rendering. The real source feature reaches initial host evaluation; mutation snapshots retain ID, properties and geometry type. The absence of these values from the plugin's C layout callback does **not** mean host paint expressions lack them. See [conversion and capability gating](../../src/mln/style/plugin_property.cpp#L333), [evaluation](../../src/mln/style/plugin_property.cpp#L93), and [binder population](../../src/mln/renderer/buckets/plugin_bucket.cpp#L160).

Null/unset paint resets to descriptor defaults, explicit values/expressions/transitions serialize, and mutation notifies the renderer. Retained paint binders synchronize expressions and rebuild drawables when uniform/attribute mode changes. Per-layer binder maps preserve different paint on shared geometry. Existing source-state machinery supports state updates/removal while a binder is data-driven; GeoJSON data changes reparse tiles and replace buckets. No permanent stale-source-data defect was established. [Property mutation](../../src/mln/style/layers/plugin_style_layer.cpp#L88), [binder synchronization](../../src/mln/renderer/buckets/plugin_bucket.cpp#L414), [source-state replay](../../src/mln/renderer/source_state.cpp#L70), and [GeoJSON tile updates](../../src/mln/tile/geojson_tile.cpp#L22) are the relevant paths.

Native filters run before plugin layout with actual feature geometry and canonical tile context. Common source/source-layer, visibility, filter and zoom limits remain host functions. Multiple shaders, drawables, vertex streams and segmented buckets are allowed; the whole bucket is not limited to 65,535 vertices. Wrapped tile matrices and pitched query context already exist. These supported pieces have the specific correctness limits below.

## Findings in the current contract

The identifiers are stable across revisions. F01–F03 are resolved in source; their historical impact is retained below. F04–F17, F19 and F21 remain open implementation defects with high confidence; F17 is latent behind F05. F18 remains an open ABI capability/contract-policy gap. F20 remains unverified: a medium-confidence timing hypothesis requiring execution. Severity describes the original failure, not a claim that a resolved item still fails.

| ID | R2 status | Severity | Original finding / remaining failure |
| --- | --- | --- | --- |
| F01 | **Resolved in source** | High | Layer-local shader IDs collide in a renderer-wide namespace |
| F02 | **Resolved in source** | High | A plugin registered before a later style load is invisible to an already initialized renderer |
| F03 | **Resolved in source** | High | `UINT8_X4_NORMALIZED` becomes unnormalized integer bytes on GL, Metal and Vulkan |
| F04 | **Open** | High | Darwin layer enumeration adds a nil peer for a parsed plugin layer |
| F05 | **Open** | High | `within`/`distance` paint lacks canonical tile context |
| F06 | **Open** | High | Precise camera-property queries use tile zoom rather than rendered zoom |
| F07 | **Open** | High | Query statistics use only the first drawable's bounds for each property |
| F08 | **Open** | Medium | Exact queries always receive `feature_index == 0` |
| F09 | **Open** | Medium | `supports_transitions = false` still permits global transitions |
| F10 | **Open** | Medium | Source-expression-to-constant paint loses native transition delay/retention |
| F11 | **Open** | Medium | Legacy ref layers inherit paint and transitions incorrectly |
| F12 | **Open** | Medium | Invalid dynamic enum values differ between GPU and query consumers |
| F13 | **Open** | Medium | Distinct accepted property names generate identical shader macros |
| F14 | **Open** | Medium | Paint names collide with synthetic transitions and common layer controls |
| F15 | **Open** | Medium | An embedded NUL in a layer type truncates factory publication |
| F16 | **Open** | Medium | Accepted image-availability branches evaluate with no image set |
| F17 | **Open** | Medium | Mutation refill discards geometry needed by geometric expressions |
| F18 | **Open — ABI policy** | Medium | Numeric bound policy is incomplete and differs by evaluation path |
| F19 | **Open** | Medium | Nonfinite numeric values can cross the property boundary |
| F20 | **Unverified** | Medium, pending | First frame after constant-to-state binding can use stale/default state |
| F21 | **Open** | Low | Query-radius `explicitly_set` metadata changes for unset defaults |

### F01 — Shader identity must include the layer type

**R2 status: Resolved in source.** Fix: `afa51ff05383`. Focused regressions added; not executed during this refresh.

**Original failure (R1):** two layer types within one plugin could both declare shader ID `main`, but renderer keys included only plugin ID and shader ID. Unescaped separators could also make distinct IDs collide. Registration was accepted, then the first group silently supplied the program for both types.

**Current evidence:** [shaderGroupName](../../src/mln/plugin/plugin_shader.cpp#L194) now encodes `(plugin ID, layer type, shader ID)` with each component's byte length. [Group publication](../../src/mln/plugin/plugin_shader.cpp#L208), variant caching in `PluginShaderGroup`, and [render lookup](../../src/mln/renderer/layers/render_plugin_style_layer.cpp#L213) all use the same complete identity. Including a separator in an ID no longer makes component boundaries ambiguous.

[`LayerLocalShaderIDsProduceDifferentPrograms`](../../test/plugin/rendering.test.cpp#L171) registers two layer types sharing `main` and checks red/green pixels on opposite viewport halves. [`ShaderIdentityHasUnambiguousComponents`](../../test/plugin/rendering.test.cpp#L177) checks separator and layer-type distinctions directly. Keep both regressions; no part of the original namespace defect remains identified in the source. Backend execution evidence is still pending.

### F02 — Registration timing disagrees with the public contract

**R2 status: Resolved in source.** Fix: `18efc91d6b65`. Focused regression added; not executed during this refresh.

**Original failure (R1):** rendering an ordinary style, registering a plugin, then loading its style on the same map met the public contract but missed shader groups installed only during initial renderer setup.

**Current evidence:** [RenderPluginStyleLayer::update](../../src/mln/renderer/layers/render_plugin_style_layer.cpp#L168) now resolves that layer's immutable shader definitions on the render thread before drawing. The [per-layer registration overload](../../src/mln/plugin/plugin_shader.cpp#L208) adds missing groups and leaves existing ones in place. The early return when no render tiles exist does not recreate the original failure: registration is attempted once the layer has tiles to draw. The contract still permits registration before the dependent style, without requiring process-startup registration.

[`RegistrationAfterRendererInitialization`](../../test/plugin/rendering.test.cpp#L183) first renders an empty style, registers a plugin, then checks its pixels on that existing map and a newly constructed map. This exercises both original orderings. No registry refresh defect remains identified in that path; failure diagnostics and active-backend capability checks remain separate open items C05/B01/B02.

### F03 — The normalized-byte attribute enum is not implemented

**R2 status: Resolved in source.** Fix: `348de031fe1e`. Focused regression added; advertised-backend execution has not been verified during this refresh.

**Original failure (R1):** `MLN_PLUGIN_VERTEX_UINT8_X4_NORMALIZED` mapped to unnormalized byte attributes, so packed `(128, 64, 0, 255)` could reach shaders as integer-scale values rather than approximately `(0.502, 0.251, 0, 1)`.

**Current evidence:** a distinct [`UByte4Normalized`](../../include/mln/gfx/gfx_types.hpp#L64) now flows through both [plugin shader declarations](../../src/mln/plugin/plugin_shader.cpp#L53) and [drawable bindings](../../src/mln/renderer/layers/render_plugin_style_layer.cpp#L53), with four-byte stride. GL sets [normalization true](../../src/mln/gl/value.cpp#L623), Metal uses [`UChar4Normalized`](../../src/mln/mtl/drawable.cpp#L468), and Vulkan uses [`R8G8B8A8Unorm`](../../src/mln/vulkan/pipeline.cpp#L27). The generic WebGPU format map also adds [`Unorm8x4`](../../src/mln/webgpu/drawable.cpp#L132); this does **not** add WebGPU support to the plugin ABI (B02 remains open).

[`NormalizedByteColorAttributes`](../../test/plugin/rendering.test.cpp#L197) renders the packed color on both viewport halves and checks RGB approximately `(128, 64, 0)` and alpha 255. The helper contains GL, Metal and Vulkan shader sources, but a test run exercises the backend selected by its build, not all three simultaneously. Retain cross-backend execution in C02; distinguish that validation gap from the now-correct format mappings.

### F04 — Darwin's common layer API can throw on plugin layers

**R2 status — F04: Open; controlling code unchanged.**

A host that enables plugin core support and registers/loads JSON successfully can still fail when accessing `MLNStyle.layers`. Runtime registration publishes only a core factory; [Darwin peer lookup](../../platform/darwin/src/MLNStyleLayerManager.mm#L109) has no generic plugin peer. [Layer enumeration](../../platform/darwin/src/MLNStyle.mm#L285) unconditionally adds the returned nil to `NSMutableArray`, which raises an exception. Named lookup returns nil despite the core layer being present. `setLayers` also traverses this path.

Provide a generic platform layer peer or, at minimum, handle unsupported peers explicitly in common operations. Test enumeration, lookup and layer replacement with a builtin and a plugin layer together. [Android peer lookup](../../platform/android/MapLibreAndroid/src/cpp/style/layers/layer_manager.cpp#L90) also lacks a plugin peer and can yield null entries through [getLayers](../../platform/android/MapLibreAndroid/src/cpp/native_map_view.cpp#L1088); that is separate from the Darwin exception. This finding is conditional on a host enabling the new API, and does not allege a crash in the default plugin-disabled SDK.

### F05 and F17 — Geometry-dependent paint loses two required inputs

**R2 status — F05: Open; controlling code unchanged; F17: Open; controlling code unchanged.**

For a point at longitude/latitude `[0, 0]`, this n-gon paint is a concrete inside/outside test:

```json
{
  "ngon-radius": [
    "case",
    ["within", {"type": "Polygon", "coordinates": [
      [[-1, -1], [1, -1], [1, 1], [-1, 1], [-1, -1]]
    ]}],
    40,
    4
  ]
}
```

[Plugin evaluation](../../src/mln/style/plugin_property.cpp#L94) constructs context with zoom, feature and state but no canonical tile ID. [`within`](../../src/mln/style/expression/within.cpp#L222) returns false without it, so the radius is 4; [`distance`](../../src/mln/style/expression/distance.cpp#L878) returns an evaluation error, which the [typed property wrapper](../../include/mln/style/property_expression.hpp#L84) converts to fallback. A numeric paint expression such as `["distance", {"type":"Point","coordinates":[0,0]}]` therefore cannot provide its intended result. Filters have [canonical context](../../src/mln/layout/plugin_layout.cpp#L96), so using `within` in a filter is a different, supported path.

F17 is an additional, currently masked defect: on a retained bucket, [synchronize/refill](../../src/mln/renderer/buckets/plugin_bucket.cpp#L232) uses [PluginFeatureSnapshot](../../src/mln/renderer/buckets/plugin_bucket.cpp#L18), which stores ID/type/properties but no geometry. Its inherited `getGeometries` returns [an empty collection](../../src/mln/tile/geometry_tile_data.cpp#L299). Fixing canonical context alone leaves later paint mutation broken. Preserve immutable geometry and tile identity, or supply the current tile layer during refill. Also pass the complete context to exact queries and state updates.

Adapt [within raw fixtures](../../test/fixtures/expression_equality/within.a.json), [distance raw fixtures](../../test/fixtures/expression_equality/distance.a.json), and the [feature-driven n-gon style](../../plugins/ngon-layer/render-tests/ngon/all-properties-feature-driven/style.json) into initial-render, mutation and query tests. The geometric mechanism is source-confirmed; no images were rendered here.

### F06 — Camera paint queries use the wrong zoom

**R2 status — F06: Open; controlling code unchanged.**

```json
{
  "ngon-radius": ["interpolate", ["linear"], ["zoom"], 10, 10, 11, 30]
}
```

At map zoom 10.5 with a canonical z10 tile, drawing evaluates radius 20. [FeatureIndex](../../src/mln/geometry/feature_index.cpp#L289) passes canonical `tileID.z` to the precise-query path, which [evaluates all plugin paint at that value](../../src/mln/renderer/layers/render_plugin_style_layer.cpp#L308); the callback receives radius 10. A point query 15 pixels from the center can miss visible geometry. Low-maxzoom sources make overzoom discrepancies larger. Pure camera expressions remain expressions [outside active transitions](../../src/mln/style/plugin_property.cpp#L300), while shader uniforms use [current transform zoom](../../src/mln/renderer/layers/plugin_layer_tweaker.cpp#L82), so transition completion can also change query behavior.

Use current transform zoom for query paint or retain correctly pre-evaluated camera values. Add host `queryRenderedFeatures` assertions at fractional zoom and overzoom; the [existing fractional-zoom image](../../plugins/ngon-layer/render-tests/ngon/composite-fractional-zoom/style.json) and [direct callback test](../../plugins/ngon-layer/tests/ngon.test.cpp#L85) do not exercise this handoff. Native circle camera properties are already [pre-evaluated](../../src/mln/renderer/data_driven_property_evaluator.hpp#L38). Native composite queries have their own canonical-zoom limitation; only the pure-camera discrepancy is attributed here as plugin-specific.

### F07 and F08 — Broad-phase bounds and precise identity are incomplete

**R2 status — F07: Open; controlling code unchanged; F08: Open; controlling code unchanged.**

F07: return drawable key 1 containing a radius-5 feature and key 2 containing a radius-100 feature, each with its own feature-bound stream. [updateQueryRadius](../../src/mln/renderer/buckets/plugin_bucket.cpp#L441) visits both, but [appendStatistics](../../src/mln/renderer/buckets/plugin_bucket.cpp#L380) skips any property already present. The callback receives the first radius bounds rather than the union. [Tile query padding](../../src/mln/tile/geometry_tile.cpp#L501) can exclude the second feature before exact hit testing. Reduce minima/maxima over every drawable, including FLOAT2 translations, and test both drawable orders. The [existing statistics test](../../test/plugin/property_binding.test.cpp#L62) has only one binder.

F08: layout correctly supplies the source feature index at [layout dispatch](../../src/mln/layout/plugin_layout.cpp#L114). Exact query constructs the same ABI feature structure but [never sets its index](../../src/mln/renderer/layers/render_plugin_style_layer.cpp#L288), so every callback sees zero. Geometry generated deterministically from index 1 or greater cannot be reconstructed using that field. Carry the actual index through `FeatureIndex`'s render-layer dispatch or provide an explicit feature handle. A two-feature callback-index test is sufficient; the [current n-gon query](../../plugins/ngon-layer/shared/cpp/ngon_layer.cpp#L338) uses geometry/paint and masks the problem. This index is distinct from the feature's stable source ID.

### F09 and F10 — Transition semantics diverge from the descriptor/native contract

**R2 status — F09: Open; controlling code unchanged; F10: Open; controlling code unchanged.**

F09: a FLOAT property registered with `supports_transitions = 0` rejects its per-property transition setter, but [RenderPluginStyleLayer::transition](../../src/mln/renderer/layers/render_plugin_style_layer.cpp#L96) applies global options to every definition without checking the flag. With root `"transition":{"duration":1000}`, changing its constant from 5 to 40 still interpolates for a second. STRING properties can be delayed until the end, although they cannot opt into transitions at registration. Honor the [opt-in flag](../../include/mln/plugin/plugin_api.h#L124) when choosing options. Compare opted-in and opted-out float/string values under the same global duration/delay. The [registry test descriptors](../../test/plugin/registry.test.cpp#L14) leave the flag false but never render a transition.

F10: start with feature-driven radius and then set a constant:

```json
{
  "ngon-radius": ["get", "size"],
  "ngon-radius-transition": {"duration": 1000, "delay": 500}
}
```

Changing `ngon-radius` to `20` drops the prior immediately because [PluginTransitioningPropertyValue](../../src/mln/style/plugin_property.cpp#L292) rejects a prior when either operand is data-driven. Native [Transitioning](../../src/mln/style/properties.hpp#L50) snaps when the **new** value is data-driven, but preserves the previous source expression through delay and the transition interval when changing to a constant. Native [nonconstant interpolation](../../src/mln/renderer/possibly_evaluated_property_value.hpp#L136) does not smoothly interpolate arbitrary source expressions; it retains the old expression until the end. Match that behavior or document and validate a narrower model. Extend the [source-to-constant binder test](../../test/plugin/property_binding.test.cpp#L82) and [paint-update style](../../plugins/ngon-layer/render-tests/ngon/paint-update/style.json) with timed intermediate assertions.

### F11 — Ref layers retain the parent's paint

**R2 status — F11: Open; controlling code unchanged.**

```json
[
  {
    "id": "base", "type": "ngon", "source": "points",
    "paint": {"ngon-radius": 40, "ngon-opacity": 0}
  },
  {"id": "child", "ref": "base", "paint": {"ngon-radius": 10}}
]
```

The child should inherit layout/source semantics and use its own default opacity. [Plugin cloneRef](../../src/mln/style/layers/plugin_style_layer.cpp#L182) copies the entire implementation, including paint and transitions. The [style parser](../../src/mln/style/parser.cpp#L381) then overrides only child paint that was supplied, leaving opacity zero. Native [circle cloneRef](../../src/mln/style/layers/circle_layer.cpp#L57) resets paint. Clear both plugin paint maps when cloning a ref. Replace the [existing inherited-opacity assertion](../../test/plugin/registry.test.cpp#L125), which currently encodes the wrong expectation, and add full-parser tests for omitted/overridden/null child paint. This finding concerns legacy style `ref`, not the semantics of an ordinary full clone.

### F12 — Dynamic enum fallback differs by consumer

**R2 status — F12: Open; controlling code unchanged.**

For a bound enum with allowed values `map`/`viewport` and default `map`, use `"anchor":["get","anchor"]` and feature property `"anchor":"invalid"`. [Constant validation](../../src/mln/style/plugin_property.cpp#L122) checks allowed values, but evaluated expressions return arbitrary strings. The [GPU encoder](../../src/mln/renderer/buckets/plugin_bucket.cpp#L69) converts an unknown string to the default ordinal; [exact queries](../../src/mln/renderer/layers/render_plugin_style_layer.cpp#L308) and [camera values passed to radius callbacks](../../src/mln/renderer/buckets/plugin_bucket.cpp#L468) receive the original string. A callback using a different invalid-value rule can hit-test a different anchor from the shader.

Normalize enum values once before all consumers, including statistics and query callbacks. Test invalid strings, missing/null values and camera-generated invalid outputs. The [enum binder test](../../test/plugin/property_binding.test.cpp#L66) uses valid outputs only. The n-gon helper happens to apply a default for invalid anchors; that does not repair the general ABI inconsistency for other callbacks.

### F13–F15 — Accepted names can be unusable

**R2 status — F13: Open; controlling code unchanged; F14: Open; controlling code unchanged; F15: Open; controlling code unchanged.**

F13: `foo-bar` and `foo_bar`, or `x` and `X`, are distinct accepted property names. [propertyMacro](../../src/mln/plugin/plugin_shader.cpp#L83) uppercases alphanumerics and replaces punctuation with underscores, generating the same `MLN_PLUGIN_PROPERTY_*_IS_UNIFORM` name. Mixed constant/feature bindings can redefine a macro inconsistently with attributes, causing compilation failure or wrong shader branches. Validate macro uniqueness per shader or use an injective encoding. Add colliding names to the [descriptor validation tests](../../test/plugin/api.test.cpp#L179); the example's controlled names avoid the issue.

F14: register transition-enabled `p` and a separate `p-transition` FLOAT. [Registration](../../src/mln/plugin/plugin_registry.cpp#L408) checks exact duplicate names only; [setter dispatch](../../src/mln/style/layers/plugin_style_layer.cpp#L169) routes `p-transition` to transition conversion first, making the FLOAT property inaccessible. Names such as `visibility`, `filter`, `minzoom` and `source` can also shadow [generic layer controls](../../src/mln/style/layer.cpp#L170). Reserve generated/native names or separate namespaces end to end. Test descriptor rejection plus common control setters, not merely registration success.

F15: the sized string `name\0suffix` is copied in full into registry keys, but [LayerTypeIdentity](../../src/mln/plugin/plugin_registry.hpp#L94) exposes `name.c_str()` and [factory publication](../../src/mln/layermanager/layer_manager.cpp#L46) reconstructs a string without the original length. Registration can publish the truncated type `name` with mismatched property lookup identity. Reject embedded NUL early, with no factory published. Extend the [string/pointer validation test](../../test/plugin/api.test.cpp#L133) to cover sized identifiers with NUL. These are ordinary descriptor-validation failures, not claims of hostile-plugin containment.

### F16 — Numeric expressions can observe incorrect image availability

**R2 status — F16: Open; controlling code unchanged.**

A direct image-valued property is unnecessary to trigger this defect. With style image `present` available and `missing` unavailable, use:

```json
{
  "ngon-radius": [
    "case",
    ["==", ["to-string", ["coalesce", ["image", "missing"], ["image", "present"]]], "present"],
    10,
    1
  ]
}
```

The result should select 10. [Plugin capability checks](../../src/mln/style/plugin_property.cpp#L353) do not reject image dependencies, but [evaluation context](../../src/mln/style/plugin_property.cpp#L94) has no available-images set. [Image evaluation](../../src/mln/style/expression/image_expression.cpp#L63) marks both unavailable, and [coalesce](../../src/mln/style/expression/coalesce.cpp#L18) returns the first requested image when none is available; the branch selects 1. Image changes also have no plugin binder dependency invalidation path.

Either implement image availability and add/remove invalidation or reject this dependency during conversion. Adapt the [image expression fixtures](../../test/fixtures/expression_equality/image.a.json) to scalar paint. Texture/pattern rendering remains a separate ABI gap even if this numeric-expression defect is fixed.

### F18 and F19 — Numeric policy and validity need explicit boundaries

**R2 status — F18: Open; ABI policy unresolved; F19: Open; controlling code unchanged.**

F18 is classified as an **ABI capability gap**, with high confidence in the observed behavior but no claim that native properties universally clamp expression outputs. A FLOAT descriptor with min 0/max 1 rejects literal `2`, while `["get","opacity"]` with feature value `2` or a camera expression output of `2` bypasses [constant-only validation](../../src/mln/style/plugin_property.cpp#L122) and reaches GPU/query consumers. FLOAT2 may declare bounds, but [default registration](../../src/mln/plugin/plugin_registry.cpp#L446) and constant validation check numeric bounds only for FLOAT. The API does not state a complete evaluation-time bound policy.

Choose fallback, rejection of statically invalid outputs, documented clamping, or explicitly constant-only bounds; reject bounds on types that do not support them. Use the same final-value policy across uniform, attributes and queries. Current [property tests](../../test/plugin/property_binding.test.cpp#L28) do not define numeric bounds, and the [style conversion check](../../test/plugin/registry.test.cpp#L130) covers wrong type rather than this matrix.

F19 is the concrete boundary defect: [valueMatches](../../src/mln/plugin/plugin_registry.cpp#L671) validates type without finiteness. Unbounded C defaults containing NaN/Infinity, FLOAT2/COLOR nonfinite components, or double-to-float overflow can pass [default validation](../../src/mln/plugin/plugin_registry.cpp#L438) or [numeric narrowing](../../src/mln/style/expression/value.cpp#L200) and enter [GPU paint data](../../src/mln/renderer/buckets/plugin_bucket.cpp#L65). The eventual query-radius finite check does not sanitize paint or statistics. Shader consequences are backend/content-dependent; no crash is asserted.

Require finite defaults, meaningful finite bounds, and finite values after narrowing/evaluation. Define the color representation before imposing channel bounds. Extend [descriptor boundary tests](../../test/plugin/api.test.cpp#L96) with C NaN/Infinity and finite-but-too-large JSON numbers, and add matching constant/source/camera/composite consumer checks.

### F20 — First-frame state after a binding-mode change needs execution

**R2 status — F20: Unverified; runtime reproduction pending.**

This is a **coverage gap and medium-confidence likely defect**, not a demonstrated runtime result. Set state `{ "size": 42 }` for feature ID 1 while radius remains constant, then install:

```json
{"ngon-radius": ["coalesce", ["feature-state", "size"], 12]}
```

[Binder update](../../src/mln/renderer/buckets/plugin_bucket.cpp#L238) returns before caching state when the binder is constant. The orchestrator [prepares sources](../../src/mln/renderer/render_orchestrator.cpp#L439) before [layer update](../../src/mln/renderer/render_orchestrator.cpp#L983); synchronization later refills the newly data-driven binding from cached state. Source tracing therefore predicts fallback 12 in the first frame, or older state if that binder was data-driven before a constant interval. The [next source preparation](../../src/mln/renderer/source_state.cpp#L100) resends the complete current state and repairs it. Whether another frame is scheduled immediately was not executed or proven; **permanent state loss is not claimed**.

Cache full state independently of current binding mode, or synchronize mode before applying state. Assert the first rendered frame after install, including state changed/removed while constant. The [unit test](../../test/plugin/property_binding.test.cpp#L53) explicitly applies state after a data-driven change, and the [feature-state image](../../plugins/ngon-layer/render-tests/ngon/feature-state/style.json) installs expressions before setting state; neither tests this ordering.

### F21 — Explicit-set metadata is derived from the wrong map

**R2 status — F21: Open; controlling code unchanged.**

The renderer [populates evaluated paint with every default](../../src/mln/renderer/layers/render_plugin_style_layer.cpp#L83). [Query-radius update](../../src/mln/renderer/buckets/plugin_bucket.cpp#L468) infers `explicitly_set` from presence in that complete map. An unset property is false during initial layout, true later in radius callbacks, and correctly false in [precise query metadata](../../src/mln/renderer/layers/render_plugin_style_layer.cpp#L309). A callback cannot reliably use this advertised field to distinguish explicit settings. Preserve the raw layer flag independently from evaluated values. Add callback assertions to the [defaults test](../../test/plugin/registry.test.cpp#L114), covering unset, explicitly default-valued, and null-reset paint.

## Capability matrix and unsupported use models

All G/B/E statuses were rechecked for R2 and remain **Open**; the C ABI is unchanged. The following are **ABI capability gaps** with high source confidence unless a row says packaging/backend or example limitation. Priority is conditional on expanding beyond procedural geometry marks. The companion inventory maps every observed style property/operator/operation to these families; “partial” means a useful analogue can be implemented, not builtin equivalence.

| ID / category | R2 status | Supported portion | Concrete use case that cannot be represented faithfully | Boundary, fixture and direction |
| --- | --- | --- | --- | --- |
| G01 — Property types, medium | **Open** | FLOAT, FLOAT2, COLOR, STRING; boolean/array/object intermediates may produce a supported final result | Typed booleans/integers, float3/float4 property results, variable arrays, font stacks, dash arrays, formatted text, resolved images and padding/anchor objects | [Value types](../../include/mln/plugin/plugin_api.h#L63) and [encodings](../../include/mln/plugin/plugin_api.h#L199); [line-dasharray fixture](../../metrics/integration/render-tests/line-dasharray/zero-length-gap/style.json). Add types only with concrete storage/evaluation consumers; float/enum workarounds lose native type contracts. |
| G02 — Layout inputs, high | **Open** | Geometry, extent, overscaled zoom and tile-local index; host can bind paint onto fixed geometry | Per-feature cap/join/topology/sort/label content or tessellation resolution chosen by source properties or style layout values | [Feature/layout structs](../../include/mln/plugin/plugin_api.h#L267), [paint-only properties](../../src/mln/style/layers/plugin_style_layer.cpp#L88); [line-sort-key](../../metrics/integration/render-tests/line-sort-key/literal/style.json), [symbol line placement](../../metrics/integration/render-tests/symbol-placement/line/style.json). Add evaluated layout properties, borrowed feature access and relayout invalidation. |
| G03 — Unbound values, medium | **Open** | Unbound camera/constant values parse, serialize and query; finite string enums can bind to GPU ordinals | Free-string label/custom uniform parameters or registered unbound paint that should alter rendering | [Binding requirement](../../src/mln/plugin/plugin_registry.cpp#L569), [uniform callback inputs](../../include/mln/plugin/plugin_api.h#L242); [unbound test-opacity](../../test/plugin/registry.test.cpp#L26) only tests style access. Neither layout nor uniforms receives evaluated paint. Reject/document nonrendering values or add an evaluated-value consumer. |
| G04 — Special evaluation, high | **Open** | Native feature/camera/state expressions and lexical variables | Integer-zoom policy, image/pattern crossfades, line-progress/heatmap ramps, elevation samples, formatted sections or a renderer-wide mutable global variable | [Continuous property contract](../../include/mln/plugin/plugin_api.h#L117), [no crossfade](../../src/mln/renderer/layers/render_plugin_style_layer.hpp#L38), [context fields](../../include/mln/style/expression/expression.hpp#L88); [line-gradient](../../metrics/integration/render-tests/line-gradient/gradient-tile-boundaries/style.json), [heatmap-color](../../metrics/integration/render-tests/heatmap-color/expression/style.json). Add the relevant domain pipeline or reject unsupported dependencies; ordinary per-feature binding is insufficient. |
| G05 — Sources, high | **Open** | Host vector/GeoJSON GeometryTileLayer, source-layer selection, host-generated clustered/custom geometry | Source-free backgrounds/sky/grids, raster/image/video/DEM layers, plugin source decoders, a layout joining two sources or source-layers | [Required geometry source](../../src/mln/plugin/plugin_registry.hpp#L94), [factory](../../src/mln/plugin/plugin_style_layer_factory.cpp#L14); [background](../../metrics/integration/render-tests/background-color/literal/style.json), [raster](../../metrics/integration/render-tests/raster-opacity/literal/style.json). Define separate source/provider/source-free extensions. Geometry generated in finish-layout still depends on a source tile. |
| G06 — Images/fonts/GPU resources, high | **Open** | Analytic shaders, vertex attributes, fixed-size uniform bytes | Sprite/pattern sampling, glyph/SDF text, font fallback, animated raster weather, texture arrays, storage buffers or compute-generated geometry | [Shader descriptor](../../include/mln/plugin/plugin_api.h#L228), [empty texture list](../../src/mln/plugin/plugin_shader.cpp#L130), [drawables](../../src/mln/renderer/layers/render_plugin_style_layer.cpp#L240); [fill-pattern](../../metrics/integration/render-tests/fill-pattern/literal/style.json). Introduce host-owned resource handles, dependencies, samplers and lifetime rules. |
| G07 — Placement, high | **Open** | Independent per-tile procedural geometry | Labels/icons avoiding builtin and plugin labels, cross-tile identity, line placement, variable anchors and collision-aware fading | [Disabled cross-tile/fade flags](../../src/mln/plugin/plugin_registry.hpp#L100), [output-only layout](../../include/mln/plugin/plugin_api.h#L353); [line-center placement](../../metrics/integration/render-tests/symbol-placement/line-center-buffer/style.json). Integrate host placement rather than attempting global worker caches. |
| G08 — Render passes/state, high | **Open** | Multiple shaders/drawables, all translucent alpha-blended 2D triangles | Heatmap accumulation/composite, blur/glow/offscreen passes, depth-writing 3D meshes/extrusions, shadows, additive particles, opaque/culling/custom-stencil modes, instancing | [Fixed draw state](../../src/mln/renderer/layers/render_plugin_style_layer.cpp#L240), [public drawable limits](../../include/mln/plugin/plugin_api.h#L313); [extrusion opacity](../../metrics/integration/render-tests/fill-extrusion-pattern/opacity/style.json). Version render-state/pass/resource-transition descriptors. FLOAT3 positions alone do not grant host 3D semantics. |
| G09 — Tile context/ownership, medium | **Open** | Wrapped host matrices; plugins can clip geometry or apply half-open point ownership | Global phase/latitude-dependent meshes, complete buffered polygon/line seam behavior, neighbor/parent coordination and native tile-stencil semantics | [Layout context](../../include/mln/plugin/plugin_api.h#L279), [stencil disabled](../../src/mln/renderer/layers/render_plugin_style_layer.cpp#L248); [point boundary fixture](../../plugins/ngon-layer/render-tests/ngon/tile-boundary-and-multipoint/style.json), [wrapped patterns](../../metrics/integration/render-tests/fill-pattern/wrapping-with-interpolation/style.json). Expose canonical/wrap/overscale context and optional stencil policy. Buffered lines/polygons are not categorically impossible; plugins must implement edge handling themselves. |
| G10 — Geometry/mapping, medium | **Open** | Point/line/polygon paths, custom float3 output, multiple streams and uint16 segments exceeding 65,535 total vertices | Original geographic/elevation coordinates, hardware instancing, shared vertices with independent per-feature paint, synthetic logical query features | [2D int16 input](../../include/mln/plugin/plugin_api.h#L261), [range validation](../../src/mln/layout/plugin_layout.cpp#L210); [large example segmentation](../../plugins/ngon-layer/tests/ngon.test.cpp#L68). Document segment-local indices and per-drawable full feature coverage; add richer indices/instances only when demanded. Segment feature_index is not the paint mapping; feature-vertex ranges are. |
| G11 — Queries, medium | **Open** | Optional analytic hit callback, source feature results, paint values, matrix/bearing/viewport/pitch context and scalar conservative radius | Picking emitted triangles or 3D depth, correlating retained mesh state, matching custom draw order, joining outputs from sources, arbitrary far-displaced marks | [Query ABI](../../include/mln/plugin/plugin_api.h#L370), [source-based dispatch](../../src/mln/renderer/layers/render_plugin_style_layer.cpp#L275), [padding cap](../../src/mln/geometry/feature_index.cpp#L164); [world-wrapping query](../../metrics/integration/query-tests/world-wrapping/box/style.json). Add identity/retained query geometry and transform-aware bounds. A missing query callback returns false; it has no automatic triangle fallback. |
| G12 — Lifecycle/animation, high | **Open** | Temporary worker layout create/destroy; process-lifetime registration; normal paint transitions schedule changes | Independent per-map/per-layer state, asynchronous resources, simulations requesting frames, hot reload/unload, context recreation or cache teardown | [Lifetime contract](../../include/mln/plugin/plugin_api.h#L35), [callbacks](../../include/mln/plugin/plugin_api.h#L408), [transition-only scheduling](../../src/mln/renderer/layers/render_plugin_style_layer.cpp#L142); [layout destruction tests](../../test/plugin/layout.test.cpp#L207). Introduce explicit instance handles and invalidation/lifecycle hooks if required. No persistent instance ID/userdata reaches uniform callbacks. |
| B01 — Shader/backend capacity, medium, packaging/backend gap | **Open** | GL, Metal and Vulkan sources, property variants, uniform binding macros | Portably selecting device resource limits, arbitrary uniform-block layouts or active-backend fallback | [Registration caps](../../src/mln/plugin/plugin_registry.cpp#L218), [backend shader creation](../../src/mln/plugin/plugin_shader.cpp#L109); [16-attribute example](../../plugins/ngon-layer/tests/ngon.test.cpp#L37). Expose capabilities and validate against the active backend/device. See detailed limits below. |
| B02 — WebGPU, high, packaging/backend gap | **Open** | Builtin WebGPU remains a separate supported renderer path | Running this C-ABI plugin on a WebGPU host | [Backend enum](../../include/mln/plugin/plugin_api.h#L57), [known-mask validation](../../src/mln/plugin/plugin_registry.cpp#L113), [WebGPU initialization](../../src/mln/webgpu/renderer_backend.cpp#L89). No WGSL/backend branch or plugin shader-group initialization exists. Reject active-backend incompatibility clearly or implement it. |
| B03 — Distribution/SDKs, high, packaging/backend gap | **Open** | Opt-in CMake/Bazel integration and example registration in custom native hosts | Turnkey binary plugin loading and ordinary Android/iOS SDK registration/dynamic layer manipulation | [Plugin CMake](../../plugins/CMakeLists.txt#L1), [Android exports](../../platform/android/version-script#L2), [platform peer factories](../../platform/darwin/src/MLNStyleLayerManager.mm#L109). Add API-only packages, platform bridges/export policy and generic layer peers; F04 is the immediate crash within this broader gap. |
| E01 — N-gon sample, medium, example-plugin limitation | **Open** | Procedural point/MultiPoint marks with 13 paints, blur/stroke, rotation, pitch/translation modes and analytic queries | Line/polygon layer demonstrations, more than one layer/shader/drawable, resource use and host query validation | [Example registration](../../plugins/ngon-layer/shared/cpp/ngon_layer.cpp#L427), [sample ownership](../../plugins/ngon-layer/shared/cpp/ngon_layer.cpp#L85), [fixture audit](plugin-api-style-inventory.md#the-13-native-ngon-image-fixtures). Use additional small plugins to test API dimensions; the example's point-only choice does not limit the ABI's geometry mask. |

G04 requires careful attribution. `heatmap-density` and `line-progress` need a per-sample ramp domain; `accumulated` belongs to source cluster reduction; elevation needs DEM/context plumbing. A standalone specialized expression may already fail native conversion's runtime-constant assumptions rather than parse and silently fall back. Feature-dependent wrappers can parse but receive missing context. Host source clustering can still supply resulting geometry/properties to a plugin; the cluster reducer itself is not a plugin paint callback. Lexical `let`/`var` is supported and must not be confused with an external mutable global/config API, which is absent from this native evaluation context.

B01's practical constraints are concrete: attribute IDs/locations must be below 16, with locations contiguous from zero; every advertised backend needs source for every shader. GL/Metal allow at most one vertex-only, one fragment-only and one both-stage uniform block. Vulkan uses sequential blocks up to the host's builtin-derived UBO maximum. Byte sizes must be multiples of 16, without device-size negotiation. Paint bindings reserve endpoint attribute slots even when a particular layer uses uniforms; packed scalar/enum and FLOAT2 endpoints consume less space than two FLOAT4 color endpoints. Uniform callbacks receive no shader/layer/drawable identity, which complicates several shaders with different custom uniform protocols. These are distinct from F03's incorrect implementation of an explicitly advertised format.

G11 is also narrower than “no pitched query support.” Query context has a double tile matrix, camera distance, bearing and logical viewport; tile preparation accounts for wrapping, overscale and pitch when computing padding. But screen geometry is first unprojected to ground and narrowed to int16 tile coordinates, extra broad-phase padding is capped at `util::EXTENT`, and the radius callback has no transform, tile identity or retained generated geometry. Arbitrarily displaced/3D marks cannot rely on this path. The application's final source-feature result can still contain real ID/properties/state even though the plugin hit callback lacks them.

B03 is not merely missing convenience wrappers. The feature defaults off; enabling core plugins also enables the n-gon example and its Node shader generator. The CMake example PUBLIC-links `mbgl-core`, unlike the [Bazel header-only ABI dependency](../../plugins/ngon-layer/BUILD.bazel#L33). There is no installed standalone loader/package or Android JNI registration bridge. Android Release's export map exposes only `JNI_OnLoad` and hides other symbols, so a visibility annotation on `mln_plugin_register_v1` alone is insufficient for an external `dlsym` integration. A custom host can pass the registration callback directly while linking internally. That route should be documented separately from a future binary plugin SDK.

Conceptual uses not represented by a current JSON fixture include source-free sky/world grids, multi-source joins, per-map animated simulations, asynchronous plugin resources, terrain-aware meshes, shadow maps, compute/instanced particles and host-wide mutable variables. Their blockers are G02/G04–G12, not an inference from their absence in the corpus. No terrain, sky or projection root object occurs in the identified JSON corpus; that observation alone says nothing about builtin implementation support.

## Existing tests: actual assertions and blind spots

There are **17 core C-ABI GoogleTest cases**, one standalone n-gon test executable, and 13 n-gon image fixtures in the reviewed source. The three tests in [test/plugin/plugin.test.cpp](../../test/plugin/plugin.test.cpp#L104) exercise the older C++ plugin mechanism and are not evidence for this C ABI. None of these tests was executed during this review.

| Test group | Count | What the test code asserts | What it cannot establish |
| --- | ---: | --- | --- |
| [API descriptors](../../test/plugin/api.test.cpp#L96) | 5 | Truncated struct handling, pointer/array/string presence, vertex limits, short bindings, metadata copying and atomic conflict rejection | Shader namespace semantics, shader compilation, normalized bytes, numeric finiteness, active backend or renderer registration timing |
| [Registry/style](../../test/plugin/registry.test.cpp#L75) | 2 | Descriptor validation/copying, parsing/defaults/expressions, setter errors and clone behavior | Native ref correctness (one assertion enshrines F11), full parse-serialize-parse, transition execution, unset callback metadata or nonrendering unbound values |
| [Layout ownership](../../test/plugin/layout.test.cpp#L207) | 4 | Failed-create nonnull handle destruction, null handling, exactly-once early-return cleanup, output copied before destruction | Line/polygon semantics, CPU feature attributes, multi-drawable bounds, backend uploads, generated-geometry picking or concurrent map isolation |
| [Paint binders](../../test/plugin/property_binding.test.cpp#L39) | 2 | Packed composite endpoints, state update, expression synchronization, valid enum ordinals and owned statistics strings | Geometry context, first frame after constant/state mode switch, multiple drawable reduction, camera query zoom, bounds/finite values or invalid enum parity |
| [Host rendering and shader identity](../../test/plugin/rendering.test.cpp#L171) | 4 | Distinct red/green programs for layer-local shader IDs, separator-safe names, registration after a rendered frame plus a new map, and normalized packed-color pixels | No recorded execution in this review; each build selects one backend; no plugin rendered-query or mobile-peer assertions |
| [Standalone n-gon callbacks](../../plugins/ngon-layer/tests/ngon.test.cpp#L33) | 1 executable | One layer/one shader, 13 properties, 16 attributes; half-open point ownership; 17,000 points split into two uint16 segments; triangle point hit/miss before/after rotation and zero radius | Host registration, shader compilation, GPU rendering, feature-index handoff, broad-phase queries, pitched/box query correctness or returned application feature results |
| [N-gon images](plugin-api-style-inventory.md#the-13-native-ngon-image-fixtures) | 13 | Expected-image comparison after configured operations | Query expectations, per-frame transition/state-mode correctness, backend parity or comprehensive expression/property semantics |

The images cover all 13 n-gon properties driven by `get`, blur/opacity, corners/rotation, translation anchors, all four pitch alignment/scale combinations, one composite case at zoom 10.5, point/MultiPoint tile ownership, and zero radius. One fixture sets feature state with expressions already installed; another performs four constant paint mutations and captures only the final image. Each has an `expected.png`; none has `queryGeometry`, `queryOptions` or expected query JSON. The [manifest](../../plugins/ngon-layer/render-tests/manifest.json#L1) and [image comparison/operation harness](../../render-test/runner.cpp#L903) establish assertion type, not a current passing backend run.

**C01 — R2 status: Open. Coverage gap, high priority/high confidence:** no C-ABI host rendered-query fixture covers the handoffs implicated in F05–F08/F12/F20. Direct callback tests do not traverse broad phase or host property evaluation. Add host-level hit/miss and returned-ID checks, using the existing circle query families as comparators.

**C02 — R2 status: Partially addressed. Coverage gap, high priority/high confidence:** R2 adds four [host rendering/identity regressions](../../test/plugin/rendering.test.cpp#L171) covering the original F01–F03 triggers, with shader code for GL, Metal and Vulkan. It is no longer correct to say there are no late-registration, colliding-ID or normalized-byte tests. Remaining gaps are observed execution across all advertised backends, Darwin/Android peer behavior (F04/B03), active-device resource limits and backend mismatch (B01/B02), and broader failure/recovery lifecycle coverage. No passing run is recorded by this refresh.

**C03 — R2 status: Open. Coverage gap, medium priority/high confidence:** the style lifecycle lacks a full parser/serialization/default/error/transition matrix, including legacy functions, expression fallback, null resets, explicit-default metadata, dynamic invalid enums, state removal and source refresh. Adapt [native function conversion tests](../../test/style/conversion/function.test.cpp), [remove-feature-state queries](../../metrics/integration/query-tests/remove-feature-state/default/style.json), and [parser fixtures](../../test/fixtures/style_parser/function-string-bool-enum.style.json) into a plugin-specific suite.

**C04 — R2 status: Open. Coverage gap, medium priority/high confidence:** supported geometry/mutation breadth is inferred more often than tested. Test vector source-layer selection, line/polygon geometry masks, filter changes, add/remove/reorder layers, two paint-distinct layers sharing a bucket, GeoJSON property/ID/feature replacement, overzoom/wrap and translucent buffered seams. Existing [fill](../../metrics/integration/render-tests/fill-pattern/wrapping-with-interpolation/style.json), [line-gradient boundaries](../../metrics/integration/render-tests/line-gradient/gradient-tile-boundaries/style.json) and [point boundary](../../plugins/ngon-layer/render-tests/ngon/tile-boundary-and-multipoint/style.json) cases provide concrete geometry scenarios without implying their entire resource pipeline is available to plugins.

## Contract details and attribution that should be preserved

The ABI is a same-process, same-target C ABI using native pointers, `size_t`, enum storage and platform alignment. `struct_size` protects entry-point reads, while arrays intentionally retain fixed v1 stride. Document compiler/packing/calling-convention assumptions; this review found no measured cross-compiler ABI break. Descriptor memory is copied, while callbacks must remain callable for process lifetime. No unload/unregistration is promised, so lack of unload is G12's deliberate scope restriction rather than a lifetime bug.

The [registration boundary](../../src/mln/plugin/plugin_registry.cpp#L699) catches exceptions. Other plugin callbacks are required to return normally; the host does not wrap every callback in an exception handler. Callback thread/instance ownership needs clearer documentation for multiple maps and concurrent layout/radius work, but no data race was reproduced here. The temporary layout RAII paths are a positive finding and should be retained as the ABI grows.

**C05 — R2 status: Open. Coverage/diagnostic gap, medium priority/high confidence:** metadata acceptance is not execution validation. Registration neither compiles shaders nor checks the active renderer/backend. Layout errors produce generic messages; callbacks cannot return detailed plugin diagnostics. [Uniform update failure](../../src/mln/renderer/layers/plugin_layer_tweaker.cpp#L67) logs and continues, leaving an old block or no initial block; shader absence skips drawing. Define an observable layer-error path and test shader/layout/uniform failures and recovery. Do not infer a backend crash merely from missing initial uniforms.

Several apparent limitations are shared with native code and should not be reported as newly introduced plugin defects:

- Composite paint binders use two integer endpoint samples and native interpolation factors. Fractional stop crossings, multiple stops in one integer interval and step behavior require parity tests; the approximation is not unique to plugins. F06 isolates pure-camera query behavior, because native camera values are pre-evaluated.
- Native transitions do not smoothly interpolate arbitrary feature expressions. F10 concerns retaining the prior expression and respecting delay when the new value is constant.
- Native numeric expressions are not universally clamped to style-spec bounds. F18 asks for a coherent descriptor policy and rejects silently meaningless type/bound combinations.
- `interpolate-hcl`/`interpolate-lab` appear in raw historical fixtures but are absent from the current native operator registry. Their presence is not proof of current plugin or builtin support. Native query-option filters also have context omissions independent of plugin paint.
- Generic plugin getters return descriptor defaults when unset, whereas some generated native getters expose Undefined. Serialization emits only explicit settings. This is an observable API difference to document/test, not by itself a rendering defect.
- Legacy functions use native conversion, and lexical `let`/`var` works. Generic plugin string conversion does not enable legacy `{token}` interpolation. Neither fact justifies claiming all legacy functions or all variables are unsupported.

## Prioritized regression matrix

R2 keeps completed source fixes in the matrix as validation tasks. The first open stages repair functionality already exposed by the marker API. Broader rendering families should follow explicit scope decisions, not be prerequisites for fixing current contract defects.

| Order | Regression/action | Required observable result | Findings |
| --- | --- | --- | --- |
| 1 | Geometric paint on initial layout, retained-bucket mutation, state update and query | Known inside/outside and distance results; nonempty geometry and canonical context on every path | F05, F17 |
| 1 | Host query at zoom 10.5 and with low-maxzoom overzoom; multiple drawable radii/translations in both orders; two source indices | Visible marks hit, outside marks miss, correct feature IDs and matching layout/query indices; bounds independent of drawable order | F06–F08, C01 |
| Validate existing fixes | Run the new shader identity and late-registration regressions on supported backends | Distinct programs and both map orderings pass; tests are already present | F01/F02 resolved in source; C02 partial |
| Validate existing fix | Run the new normalized-byte color regression on GL/Metal/Vulkan | Correct 0–1 values and matching input type on each backend; test is already present | F03 resolved in source; B01 open; C02 partial |
| 1 for enabled Darwin hosts | Parse plugin plus builtin layer; enumerate/get/replace layers | No exception; common layer API exposes usable peers | F04, B03 |
| 2 | Transition opt-out/in under global duration/delay; source-to-constant update; parser ref semantics | Correct flag behavior and retained prior; child default paint independent of parent | F09–F11 |
| 2 | Invalid enum outputs and complete numeric/default/bound/type matrix across consumers | Uniforms, attributes, statistics and queries agree; malformed descriptors/results fail or fall back by documented policy | F12, F18, F19 |
| 2 | Image availability add/remove, and constant-to-state switch with state set/changed/removed while constant | Correct image branch or explicit rejection; correct first rendered frame using current state | F16, F20 |
| 2 | Macro/native/synthetic/NUL namespace validation; unset/default metadata | Deterministic rejection without publication or inaccessible paint; stable `explicitly_set` | F13–F15, F21 |
| 3 | Parse-serialize-parse, defaults/null/legacy functions, two shared layers, source refresh, filter/layer mutation | Stable values and isolation; removed/reordered source features do not retain old geometry/paint | C03, C04 |
| 3 | External plugin sample and callback/shader failures across supported build/platform routes | API-only distribution, clear backend/errors, safe observable failure and ownership cleanup | B01–B03, C02, C05 |
| After scope decision | A line/polygon plugin with layout inputs; resource/placement/pass/lifecycle prototypes | A representative family succeeds under its explicitly specified ABI contract, with backend and ownership tests | G01–G12, E01 |

Use exact-query assertions and first-frame captures where timing matters. A final image after `wait` can conceal F20, and a direct plugin callback bypasses F06–F08. Keep the existing n-gon images as broad appearance coverage while adding small fixtures that isolate each host boundary. The current review provides test directions only; all runtime results remain to be established.

## API evolution decisions and recommended sequence

1. **Stabilize v1's stated geometry-mark contract.** Keep F01–F03 closed at the source-review level and run their new regressions across advertised backends. Repair the remaining property/query context, bounds reduction, feature identity, transitions, ref handling and validation defects. Clarify supported property result/dependency types and reject accepted inputs that have no coherent rendering semantics. Address enabled-platform crashes alongside these fixes.
2. **Publish a usable host integration contract.** Separate an API-only CMake target/package from the sample/core dependency, define Android/Darwin registration and peers, provide active-backend capability checks and observable errors, and document process lifetime/thread ownership. An external sample that consumes the C header and host registration callback is a more meaningful distribution check than linking another renderer copy.
3. **Choose whether the next scope is richer geometry or builtin-like styling.** For custom lines/polygons/meshes, first add worker feature access, stable feature/tile identity, typed layout properties and invalidation. For symbol/pattern families, image/font resources and placement are necessary. For heatmaps/3D/postprocessing, explicit passes, resources and depth/blend semantics are necessary. Multiple drawables alone do not supply those facilities.
4. **Version persistent ownership before adding dynamic services.** Per-map/per-layer handles, device/resource lifecycle, asynchronous completion and frame invalidation should precede simulations, external resource loading or hot replacement. Preserve process-lifetime plugins as a simple supported mode. Select each extension using a representative corpus fixture and one conceptual use case, then require cross-backend regression evidence.

This sequence leaves builtin layer implementations available while making the plugin path's actual guarantees concrete and testable. The immediate release decision should be based on the current marker contract and the unresolved execution evidence, with the companion inventory serving as the checklist for any later claim of broader style-family coverage.
