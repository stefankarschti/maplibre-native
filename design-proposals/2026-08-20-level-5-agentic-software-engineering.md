# Roadmap to Level 5 Agentic Software Engineering

| Field | Value |
| --- | --- |
| Status | Proposal; no roadmap implementation is included |
| Assessment date | 2026-08-20 |
| Repository baseline | `3a919e7be1a96b9bca049f7b7458a55c4df68189` |
| Initial conclusion | L2 overall, with stronger automated test and release execution in some capabilities |
| Proposed end state | Evidence-gated L5 for explicitly bounded, reversible task classes; not universal autonomy |

## Scope, method, and decision standard

This proposal asks what evidence would justify removing a human from each routine engineering decision. It does not treat generated code, a green unit-test run, agent self-review, or increased agent count as proof of correctness.

The assessment inspected tracked repository content at the baseline above. Local untracked content and modified submodule state were excluded. CI configuration demonstrates that a capability is configured, not that it is currently green or reliable. GitHub branch-protection settings, environment protections, private issue and review history, secret values, registry controls, and downstream application telemetry are not stored in this repository and therefore were not credited.

The maturity levels used here are:

- **L0 — Manual:** humans perform the work.
- **L1 — Task delegation:** an agent can perform an isolated task under human control.
- **L2 — Pair engineering:** a human works interactively with an agent and inspects essentially all output.
- **L3 — Agent implementation:** an agent can implement a substantial change, but a human reviews generated code before merge.
- **L4 — Spec-driven engineering:** humans define requirements, architecture, and acceptance criteria; agents implement and verify unattended; humans validate outcomes.
- **L5 — Software factory:** humans define intent, policy, constraints, and acceptance criteria but normally neither write nor inspect implementation code. The system implements, certifies, integrates, observes, and repairs within a defined autonomy envelope.

L5 remains governed by humans. For this public native SDK, release publication, security policy, licensing, public API strategy, irreversible baseline acceptance, and acceptable downstream risk remain human responsibilities unless separate evidence later justifies changing that boundary.

---

## A. Executive assessment

### Recommendation

MapLibre Native should not attempt repository-wide L5. It should first make its strong existing test and CI assets reproducible, machine-readable, policy-addressable, and independently certifiable. After those foundations are measured, constrained L5 is realistic for narrow task classes such as non-procedural documentation, deterministic generated-file refreshes, test-only changes, and selected local refactors or bug fixes with complete behavioral oracles.

Broad autonomous public SDK release is not currently justified. Published Maven, CocoaPods, npm, GitHub, and Swift Package artifacts cannot be canaried or rolled back in the way an owned service can. Downstream applications run on hardware and configurations the repository does not observe. A confidently wrong release normally requires a follow-forward patch and downstream adoption.

The highest constraints are:

1. Requirements are not expressed as executable acceptance contracts.
2. There is no committed agent operating map or autonomous workflow; the existing harness proposal is still a proposal.
3. Local and CI validation are capable but fragmented across CMake, Bazel, Gradle, Make, npm, Rust, platform docs, and workflow YAML.
4. Evidence is incomplete or inconvenient for independent certification: several reports are HTML or console-oriented, some jobs are non-blocking, and there is no signed evidence packet.
5. Architecture is documented but not ratcheted mechanically.
6. Public release workflows have powerful credentials and automatically publish after human-reviewed version changes, while the repository has no downstream production SLO or automatic package rollback.
7. Current contribution policy requires humans to verify AI-generated content before review. L4/L5 would require an explicit community governance change, not a technical workaround.

### Highest-risk assumptions

- Render, expression, unit, simulator, and device tests are sufficiently representative of downstream behavior. This must be measured against escaped defects.
- Independent certification can replace the defect classes human reviewers currently catch. Review history is not present locally, so this remains unproven.
- A stable affected-test map can be conservative without forcing every platform/backend job for every change.
- GPU and device-farm variance can be controlled well enough to make gates reliable.
- Maintainers and the wider community would accept agents merging changes without routine implementation inspection.
- Registry publication can remain outside the first L5 envelope without making the term “specification in → production software out” misleading. In this proposal, “production” initially means merge to the protected integration branch and verified non-public artifacts; public distribution is a separate irreversible gate.

### Current reality

Every major finding is stated as **current reality → evidence → consequence**.

| Area | Current reality | Repository evidence | Consequence |
| --- | --- | --- | --- |
| Architecture | A large C++ core is wrapped by Android, Apple, Node, Qt, Linux, Windows, and GLFW surfaces. Rendering is abstracted through `gfx` with concrete OpenGL, Metal, Vulkan, and WebGPU implementations. | [`src/mbgl`](../src/mbgl), [`include/mbgl`](../include/mbgl), [`platform`](../platform), [CMake backend validation](../cmake/validate-backend-options.cmake), [platform overview](../docs/mdbook/src/platforms/README.md) | A change in shared C++ can affect many wrappers, toolchains, GPU backends, ABIs, and devices. A one-host green build is weak evidence. |
| Build and bootstrap | CMake exposes 30 configure presets; Bazel is important for Apple; Android uses Gradle and Make; Node and Rust have additional interfaces. Linux has a Docker environment. There is no single repository-wide doctor or check command. | [`CMakePresets.json`](../CMakePresets.json), [`BUILD.bazel`](../BUILD.bazel), [Android Makefile](../platform/android/Makefile), [`package.json`](../package.json), [`rustutils`](../rustutils), [Docker guide](../docs/mdbook/src/platforms/linux/using-docker.md) | An agent must reconstruct commands and prerequisites. Local success can omit a platform, renderer, generation step, or CI-only condition. |
| Test depth | The tracked tree contains 128 C++ `*.test.cpp` files, about 1,293 render styles with about 1,290 expected images, 350 expression fixtures, 126 query fixtures, device-test workflows, and 12 C++ benchmark files. | [`test`](../test), [render fixtures](../metrics/integration/render-tests), [expression fixtures](../metrics/integration/expression-tests), [query fixtures](../metrics/integration/query-tests), [`benchmark`](../benchmark), [render-test guide](../docs/mdbook/src/render-tests.md) | The repository has an unusually strong base for objective certification, especially for rendering. Coverage breadth is not yet a demonstrated autonomy success rate. |
| Static analysis | ClangFormat, Buildifier, Actionlint, SwiftFormat, Rust formatting, and repository hygiene hooks exist. ClangTidy treats enabled warnings as errors. Linux and Windows run CodeQL. Codecov allows a 1% project threshold delta. | [pre-commit configuration](../.pre-commit-config.yaml), [ClangTidy configuration](../.clang-tidy), [Linux CI](../.github/workflows/linux-ci.yml), [Windows CI](../.github/workflows/windows-ci.yml), [`codecov.yml`](../codecov.yml) | Mechanical errors and some security defects can be detected, but there is no secret scan, SBOM/provenance gate, dependency vulnerability gate for all ecosystems, or architecture dependency gate. |
| CI | There are 31 GitHub Actions workflows spanning platforms, renderers, docs, device tests, CodeQL, releases, performance, and size. Path-based pre-jobs skip work. At least ten `continue-on-error` uses weaken or qualify checks. | [workflow directory](../.github/workflows), [changed-path rules](../.github/changed-files.yml), [Linux CI Vulkan exception](../.github/workflows/linux-ci.yml), [Node CI](../.github/workflows/node-ci.yml), [Windows CI](../.github/workflows/windows-ci.yml), [iOS CI](../.github/workflows/ios-ci.yml) | CI is broad but its effective policy is difficult to infer. A certifier cannot treat “workflow green” as proof until skipped, advisory, retried, and quarantined results are explicit. |
| Visual evidence | Render tests run offline by default, accept filters and seeds, produce expected/actual/diff artifacts and HTML, and support expectation updates. The guide still requires manual inspection of new expected images. | [render-test runner](../render-test), [render-test guide](../docs/mdbook/src/render-tests.md), [metrics manifests](../metrics), [platform ignore files](../metrics/ignores) | Visual behavior is testable, but baseline creation or modification is an oracle decision. Autonomous rebaselining must be forbidden until an independent visual oracle is proven. |
| Specifications and planning | Issue templates ask for symptoms and reproduction. Feature requests ask for a desired solution. Design proposals have a community process, but the template does not require acceptance contracts, risk classification, evidence, rollback, or plan state. No PR template or execution-plan convention is committed. | [issue templates](../.github/ISSUE_TEMPLATE), [design proposal template](2022-09-02-design-proposal-template.md), [contribution process](../CONTRIBUTING.md) | Agents receive prose intent of uneven quality. Unresolved ambiguity is likely to become implementation choice rather than an explicit escalation. |
| Repository knowledge | A root architecture guide, mdBook, platform guides, security policy, and proposals exist. Some procedural and architectural content is stale. There is no root `AGENTS.md`. | [`ARCHITECTURE.md`](../ARCHITECTURE.md), [mdBook summary](../docs/mdbook/src/SUMMARY.md), [contribution guide](../CONTRIBUTING.md), [harness proposal](../docs/harness-engineering-proposal.md) | Agents can discover much of the system only through broad search and may follow obsolete Make/gyp/Mason or filename instructions. The harness proposal must not be mistaken for implemented capability. |
| Existing agent workflow | The contribution guide links to an external AI policy requiring contributors to verify generated content and disclose AI use. No tracked agent command, policy, benchmark, certifier, or autonomous merge loop exists. | [AI usage note](../CONTRIBUTING.md), absence of `AGENTS.md` and agent configuration in the tracked tree, [harness proposal](../docs/harness-engineering-proposal.md) | Current justified implementation maturity is L2, not L3–L5. A policy decision is required before humans can stop inspecting generated implementation. |
| Debugging | The codebase has logging hooks, action-journal support, sanitizers, render diffs/metrics, test HTTP servers, debug-symbol uploads, optional Tracy instrumentation, simulators, and device-farm runs. | [action journal implementation](../src/mbgl/util/action_journal_impl.cpp), [Android action-journal guide](../platform/android/docs/observability/action-journal.md), [profiling guide](../docs/mdbook/src/profiling/tracy-profiling.md), [CMake sanitizer options](../CMakeLists.txt), [Android device workflow](../.github/workflows/android-device-test.yml), [iOS device workflow](../.github/workflows/ios-device-test.yml) | Many failures are diagnosable, but no one command captures a sanitized, deterministic reproduction bundle with toolchain, renderer, device, logs, and artifacts. |
| Runtime observability | Test and benchmark environments expose rendering metrics and logs. MapLibre Native is embedded in downstream apps and owns no common production metrics, traces, SLOs, or business invariants. | [render-test guide](../docs/mdbook/src/render-tests.md), [rendering stats API](../include/mbgl/gfx/rendering_stats.hpp), [benchmark device scripts](../scripts/aws-device-farm), absence of a repository-operated production service | The factory can validate pre-release artifacts and opt-in test apps, but it cannot currently observe general downstream production or auto-diagnose regressions after distribution. |
| External services | CI uses GitHub Actions, AWS S3/Device Farm, Codecov, Sentry uploads, npm, Maven Central, CocoaPods, and an external compiler cache. Runtime storage can access remote styles, tiles, glyphs, and sprites; render tests are normally offline. | [Android CI](../.github/workflows/android-ci.yml), [device workflows](../.github/workflows/android-device-test.yml), [coverage upload](../.github/workflows/upload-coverage.yml), [release workflows](../.github/workflows), [storage interfaces](../include/mbgl/storage), [render-test guide](../docs/mdbook/src/render-tests.md) | Network and credential capabilities must be separated by phase. External content is untrusted input and cannot share a trust domain with release credentials and state-changing tools. |
| Secrets and production access | Release and device jobs use GitHub tokens, app private keys, OIDC AWS roles, Maven credentials/signing keys, CocoaPods tokens, App Store credentials, Codecov, and Sentry secrets. No GitHub `environment:` gate is declared in release workflow YAML. | [Android release](../.github/workflows/android-release.yml), [iOS release](../.github/workflows/ios-release.yml), [CocoaPods release](../.github/workflows/ios-release-cocoapods.yml), [Node release](../.github/workflows/node-release.yml), [test-app releases](../.github/workflows) | Builder and certifier runs must have no release secrets. Publication must run from a trusted, attested commit in an isolated identity with temporary least privilege. Repository YAML alone cannot prove GitHub-side protection. |
| Deployment | Android and iOS CI trigger release workflows when version files change on `main`. Node release runs on every push to `main` and publishes when the package version is absent from npm. Other releases are dispatch-driven. | [Android CI release trigger](../.github/workflows/android-ci.yml), [iOS CI release trigger](../.github/workflows/ios-ci.yml), [Node release](../.github/workflows/node-release.yml), [release policy](../docs/mdbook/src/release-policy.md) | Release execution is L3-equivalent automation after a human-controlled merge. A version-file change is a high-impact capability and must be outside initial autonomous write scope. |
| Rollback and recovery | Git changes can be reverted and workspaces recreated. Security response is human-run. The repository does not define package canaries, automated yanking, a universal runtime feature-flag system, or downstream rollback verification. | [security workflow](../SECURITY.md), [Android release guide](../docs/mdbook/src/platforms/android/release.md), [iOS release guide](../docs/mdbook/src/platforms/ios/release.md), [Node release guide](../platform/node/RELEASE.md) | Merge recovery can be cheap; public-release recovery is not. Release certification and follow-forward repair must be designed before release autonomy is considered. |
| Architecture enforcement | Public-symbol and backend-option checks exist, but no checked dependency graph or domain ownership rules prevent new cycles, backend leakage, private/public coupling, or inconsistent source registration across build systems. | [public-symbol script](../platform/scripts/check-public-symbols.js), [backend option validator](../cmake/validate-backend-options.cmake), [`ARCHITECTURE.md`](../ARCHITECTURE.md), [CMake sources](../CMakeLists.txt), [Bazel sources](../BUILD.bazel) | High agent throughput could accelerate architectural entropy while all behavioral tests remain green. |
| Dependency management | The repository pins 39 git submodules and also uses Bazel modules, Gradle, npm, and Rust dependencies. Dependabot covers GitHub Actions and Gradle; Renovate covers selected Bazel/Bazelisk managers. The security policy says external libraries are not automatically monitored for vulnerabilities. | [`.gitmodules`](../.gitmodules), [Dependabot](../.github/dependabot.yml), [Renovate](../.github/renovate.json), [`MODULE.bazel`](../MODULE.bazel), [security policy](../SECURITY.md) | Dependency updates have partial automation but incomplete cross-ecosystem vulnerability, license, provenance, and compatibility evidence. |
| Technical debt | The tracked non-vendor tree contains roughly 252 TODO/FIXME occurrences, stale instructions, disabled fuzz-like tests, commented sanitizer/UI jobs, and explicit non-blocking jobs. No central expiry/owner ledger exists. | [contribution guide](../CONTRIBUTING.md), [iOS CI](../.github/workflows/ios-ci.yml), [disabled tile-cover tests](../test/util/tile_cover.test.cpp), [workflow directory](../.github/workflows), [harness proposal](../docs/harness-engineering-proposal.md) | The factory cannot distinguish accepted debt from accidental gate erosion, and repeated failures are not systematically converted into rules. |

### Feasibility conclusion

- **Feasible after evidence:** L5 for T0 and a subset of T1 tasks defined below.
- **Plausible but unproven:** L4 for routine core changes with deterministic test oracles; constrained L5 for selected bug fixes and refactors.
- **Not currently justified:** autonomous render-baseline acceptance, public API/ABI evolution, storage migration, security-sensitive code, workflow privilege changes, release version changes, signing, or public package publication.
- **Structurally out of reach without downstream cooperation:** closed-loop production repair for arbitrary SDK consumers. Opt-in prerelease cohorts, test apps, and partner telemetry could narrow this gap, but the repository does not provide them today.

---

## B. Autonomy maturity matrix

The levels below describe demonstrated repository capability, not what a capable agent might do once.

### Engineering capability

| Capability | Current justified level | Why it stops there |
| --- | ---: | --- |
| Specification | L1 | Prose issues and design proposals exist, but acceptance, risk, and ambiguity are not machine-gated. |
| Planning | L1 | No canonical executable-plan format, durable task state, or plan verifier is committed. |
| Implementation | L2 | AI-assisted work is allowed only with human verification; no autonomous implementation harness is committed. |
| Testing | L3 | Broad CI executes unattended, but selection, flakes, evidence formats, and oracle ownership still require humans. |
| Debugging | L2 | Good diagnostics exist, but reproduction and repair are not an unattended closed loop. |
| Review | L2 | Static checks assist; humans still review implementation and accept visual/API judgments. |
| Architecture enforcement | L1 | Descriptive architecture plus narrow checks, but no dependency/ownership ratchet. |
| Security | L2 | ClangTidy, CodeQL, pinned actions, and private advisory flow exist; supply-chain and autonomy containment are incomplete. |
| Deployment/release execution | L3 | Scripted workflows publish unattended after version/main triggers, but have no agentic certification, canary, or rollback loop. |
| Production validation | L1 | Pre-release/device evidence exists; generalized downstream runtime evidence does not. |
| Maintenance/refactoring | L2 | Renovate/Dependabot can propose updates, but humans review and merge; no certified cleanup loop exists. |
| Incident recovery | L1 | Documented human security and patch-release process; no autonomous diagnosis, halt, rollback, or follow-forward certification. |

### Representative task classes

| Task class | Current level | Initial target ceiling | Reason |
| --- | ---: | ---: | --- |
| Non-procedural documentation | L2 | L5/T0 | Reversible and buildable once links, references, policy, and content checks are objective. |
| Procedural/build documentation | L2 | L4/T1 | Incorrect commands can waste contributor time or weaken validation; command claims need executable proof. |
| Deterministic generated refresh | L2 | L5/T0–T1 | Eligible only when input→generator→output provenance and clean-tree checks are complete. |
| Test-only change | L2 | L5/T1 | Must not weaken assertions, delete coverage, expand ignores, or encode a wrong oracle. |
| Local refactor with unchanged behavior | L2 | L5/T1 | Needs architecture ratchet, full affected tests, mutation/property evidence where useful, and diff scope limits. |
| Deterministic bug fix | L2 | L5/T1 after proof | Requires a pre-fix reproducer and independent hidden regression check. |
| Small feature | L2 | L4/T2 initially | Product semantics and cross-platform acceptance usually require human outcome validation. |
| Cross-module/backend feature | L2 | L4/T2 | Impact selection and backend equivalence are not yet mechanically proven. |
| Public API/ABI or style-schema change | L1–L2 | L3/T3 | Compatibility, governance, and downstream adoption are high-risk human decisions. |
| Dependency upgrade | L2 | L4/T2 | Bots propose changes, but license, vulnerability, provenance, ABI, renderer, and platform evidence is incomplete. |
| Performance or binary-size change | L1–L2 | L4/T2 | Benchmarks exist, but noise budgets and blocking thresholds are not calibrated. |
| Offline database/cache migration | L1 | L3/T3 | Data loss/corruption is difficult to reverse inside consumer applications. |
| Authentication/billing/payment | N/A | N/A | No first-party auth, billing, or payment subsystem was demonstrated. N/A is not L5 credit. |
| Security-sensitive parser/network/storage change | L1 | L3/T3 | Untrusted styles/resources and private advisory handling require specialist human oversight. |
| CI/infrastructure permission change | L1–L2 | L3/T3 | A small YAML change can combine untrusted artifacts, credentials, and state-changing actions. |
| Render-baseline update | L1 | L3/T3 | The current documented oracle is manual visual inspection. |
| Version bump, signing, or public release | L2–L3 execution | L2/T4 initially | Publication is externally visible and not reliably reversible or canaried. |

---

## C. Target L5 architecture

### Repository-specific operating model

The first legitimate “specification in → production software out” flow is:

```text
human intent and policy
  ↓
clarification and repository discovery
  ↓
versioned specification
  ↓
executable acceptance contract + hidden checks
  ↓
risk classification and capability grant
  ↓
bounded plan
  ↓
fresh isolated worktree and pinned toolchain
  ↓
builder implementation
  ↓
deterministic affected verification
  ↓
independent certifier + adversarial/security checks
  ↓
attested evidence packet
  ↓
protected integration / merge
  ↓
verified prerelease artifacts or test-app deployment
  ↓
device/simulator/downstream-cohort validation
  ↓
observe → repair/revert/halt
  ↓
separate human-authorized public release
```

Public package publication remains a distinct gate until the project has a viable prerelease cohort, measurable downstream signals, and registry-specific recovery playbooks.

### Objective gates

| Transition | Gate question | Required evidence |
| --- | --- | --- |
| Intent → discovery | Is the requested outcome owned, lawful, and unambiguous enough to investigate? | Named human sponsor; problem statement; in/out of scope; forbidden outcomes; linked issue/proposal; no unresolved product question. |
| Discovery → acceptance contract | Can current behavior and impact be reproduced from a clean checkout? | Pinned commit/submodules/toolchains; `doctor` success; baseline result; affected domains/platforms/backends from a versioned impact map; external dependencies declared. |
| Acceptance contract → risk | Is success observable without trusting the builder? | Every acceptance criterion maps to a command, scenario, invariant, image/metric oracle, or explicit human decision; hidden checks are stored outside builder context; zero “agent judges quality” criteria. |
| Risk → plan | Is the task inside a permitted autonomy envelope? | Policy-engine result with risk tier, paths, network, secrets, data, destructive operations, deployment target, cost/time/token limits, and required evaluators. Unknown classification fails closed. |
| Plan → workspace | Is the plan bounded and recoverable? | Expected files/targets; dependency and public API impact; verification matrix; checkpoint/retry strategy; revert or follow-forward plan; no T3/T4 action hidden inside a lower tier. |
| Workspace → implementation | Is execution isolated from valuable state? | Fresh worktree/container; immutable base SHA; isolated build directories/ports; no home-directory or release-secret access; read-only dependency cache; per-run identity and audit ID. |
| Implementation → deterministic checks | Did only authorized state change? | Diff path/size budget; generated-file provenance; no version, baseline, ignore, workflow permission, submodule, or public API change unless explicitly granted; clean-tree check. |
| Deterministic checks → certification | Does the change satisfy the complete repository-specific verification matrix? | Compile/typecheck/lint; unit/expression/render/contract/scenario results; architecture and build-registration rules; security/license/dependency checks; performance/size/coverage budgets; all results machine-readable. |
| Certification → integration | Did an evaluator independent of the builder fail to disprove the change? | Certifier receives spec, diff, artifacts, and hidden checks but not builder reasoning; zero unresolved severity-1/2 findings; adversarial tests pass; evidence completeness and traceability are 100%. |
| Integration → merge | Is evidence still valid on the current target branch? | Rebase or merge simulation on protected HEAD; affected gates rerun; evidence packet content hash matches the candidate commit; required checks and signatures verified. |
| Merge → prerelease deployment | Can the artifact be exercised without irreversible public impact? | Reproducible build inputs, provenance, checksums, install/link smoke tests, simulator/emulator/device matrix, no production credentials in build jobs. |
| Prerelease → public release | Has a human approved intent and irreversible distribution risk? | Human approval of release intent/evidence, changelog/API compatibility, signed artifacts, registry plan, recovery procedure, and all T4 controls. Routine code inspection is not the approval being requested. |
| Deployment → continued autonomy | Did observed behavior remain inside policy? | Post-merge CI, prerelease cohort/device results, error and performance budgets, incident/rollback status, and no missing or contradictory evidence. Deterioration automatically lowers autonomy. |

### Minimal factory components

1. **Specification/acceptance schema:** validates intent, risks, scenarios, and human decisions.
2. **Repository map:** a short root `AGENTS.md` routing tasks to canonical docs and commands.
3. **Thin command dispatcher:** invokes existing tools and always emits a versioned result envelope.
4. **Impact and policy engine:** maps files/domains/task declarations to required checks and allowed capabilities.
5. **Isolated executor:** fresh workspace, pinned environment, limited filesystem/network, no default secrets.
6. **Builder:** one implementation process, not a committee of agents.
7. **Evidence collector:** indexes native outputs rather than replacing CMake, Bazel, Gradle, GoogleTest, render-test, or benchmark tools.
8. **Independent certifier:** separate run/context; judges behavior and acceptance traceability and can add adversarial tests.
9. **Merge/release controller:** verifies packet signatures and policy; release authority is physically separate.
10. **Durable ledger:** records attempts, failures, interventions, costs, certification decisions, escapes, and autonomy changes.

The planner may initially be deterministic code plus the builder. A specialized planning agent, cross-model reviewer, or multi-agent debate should be added only if controlled experiments improve success per cost.

---

## D. Gap analysis

| Problem | Repository evidence | Required capability | Expected benefit |
| --- | --- | --- | --- |
| No agent navigation map | No root `AGENTS.md`; knowledge is distributed across root docs, mdBook, platform docs, and workflow YAML. | 80–120 line root map with task→docs→checks→escalations. | Lower discovery variance and fewer incomplete validation choices. |
| Canonical documentation is ambiguous/stale | [`ARCHITECTURE.md`](../ARCHITECTURE.md) describes Make/gyp/Mason while current builds center on CMake/Bazel; [`CONTRIBUTING.md`](../CONTRIBUTING.md) names the wrong pre-commit filename. | Canonical mdBook engineering pages, status metadata, checked paths/commands/links, generated inventories. | Prevent confidently following obsolete instructions. |
| No acceptance contract | [issue templates](../.github/ISSUE_TEMPLATE) and [proposal template](2022-09-02-design-proposal-template.md) lack executable acceptance and risk fields. | Versioned spec schema with scenarios, invariants, impact, exclusions, and unresolved-question gate. | Move ambiguity detection before implementation and make certification objective. |
| Fragmented execution | Commands are spread across [CMake presets](../CMakePresets.json), Bazel, Android Make/Gradle, npm, Rust, docs, and workflows. | Thin `doctor/list/check/repro` dispatcher used locally and in CI. | Local/CI parity and safe unattended execution. |
| Unstructured evidence | Render failure aggregate is primarily HTML; main GoogleTest calls do not standardize retained JSON/JUnit; benchmark/size results become comments/text. | Common `summary.json` envelope with native artifacts and schemas. | Certifier can decide from durable data rather than log scraping. |
| Incomplete gate semantics | Multiple `continue-on-error` uses and platform ignores have no central owner/expiry ledger. | Flake/quarantine/advisory register with reason, issue, owner domain, expiry, retry policy, and measured noise. | A green packet has stable meaning. |
| Architecture drift is not blocked | Architecture prose and narrow checks do not constrain domain/backend edges. | Baseline-aware include/dependency graph, public/private/backend/build-registration rules, explicit exceptions. | Prevent throughput from creating hidden coupling. |
| Affected-test selection is unproven | [changed-path rules](../.github/changed-files.yml) skip platform workflows, but no traceable impact model or miss-rate benchmark is present. | Versioned impact map, conservative fallbacks, shadow comparison against full matrices. | Faster feedback without silently skipping affected systems. |
| Security supply-chain proof is incomplete | CodeQL exists; repository search shows no dedicated secret scan, SBOM, signing/attestation, or all-ecosystem vulnerability gate. | Secret scan, SBOM/license/vulnerability policy, build provenance, artifact checksum/signature verification. | Makes autonomous output and release inputs auditable. |
| Privilege boundaries were designed for CI, not agents | Device/release workflows use OIDC, app tokens, signing and registry credentials, sometimes after `workflow_run` artifacts. | Separate builder/certifier/release identities and runners; trusted rebuild; capability tokens; explicit egress. | Prevent malicious input or builder compromise from becoming a release compromise. |
| Recovery is weak after package publication | Release docs describe publishing but not canary/automatic rollback; downstream runtimes are not owned. | Prerelease cohort, registry playbooks, forward-fix generator, release halt controls, compatibility fixtures. | Limits blast radius and makes confidence failures survivable. |
| Human review replacement is unmeasured | PR review comments are not in the repository and no detection baseline exists. | Review-finding taxonomy, historical replay, seeded-defect trials, human-vs-certifier non-inferiority test. | Proves whether review can be removed instead of assuming it. |
| No factory benchmark/ledger | Runtime benchmarks exist, but no repeated agent-task suite, variance report, or intervention/cost ledger. | 10–20 representative task suite, five-run trials, hidden checks, immutable run records. | Enables scientific advancement and regression to lower autonomy. |
| No downstream production loop | SDK runs inside consumer apps; no common telemetry or SLO. | Opt-in test-app/partner prerelease cohort with privacy-reviewed crash, performance, renderer/GPU, and install signals. | Supplies runtime evidence for broader autonomy without covert SDK telemetry. |

### Agent-legible repository layout

Use existing documentation systems rather than create a competing wiki:

```text
AGENTS.md                              # short routing and authority map
ARCHITECTURE.md                        # current overview, boundaries, canonical links
design-proposals/                      # governance decisions
docs/mdbook/src/engineering/
  architecture.md
  build-and-checks.md
  generated-code.md
  testing-and-evidence.md
  reliability.md
  security.md
  release-and-recovery.md
docs/specs/{active,completed}/         # versioned intent and acceptance contracts
docs/exec-plans/{active,completed}/    # resumable implementation plans
docs/generated/                        # dependency graph and generated inventories
factory/
  policy.yaml
  impact-map.yaml
  evidence.schema.json
  benchmarks/
```

The [existing harness-engineering proposal](../docs/harness-engineering-proposal.md) already describes much of the navigation, command, evidence, architecture-ratchet, and quality-ledger foundation. Its accepted parts should become P0/P1 implementation work rather than be duplicated.

---

## E. Evidence and certification model

### Evidence packet

An autonomous change is certified only when one immutable packet contains:

1. **Identity:** run ID, task/spec ID, candidate/base commit, diff hash, builder/certifier identities and model/harness versions.
2. **Environment:** OS/container image digest, compiler/SDK versions, CMake/Bazel/Gradle/npm/Rust versions, submodule SHAs, device/GPU/renderer facts, dirty-state flag.
3. **Policy:** risk tier, granted paths/capabilities, network allowlist, secrets available, deployment target, time/token/cost limits, and policy-engine version.
4. **Acceptance trace:** every criterion linked to at least one test, scenario, invariant, artifact, or recorded human decision.
5. **Static results:** builds, types, formats, ClangTidy, CodeQL, architecture rules, source/build registration, API/ABI, secret/dependency/license checks.
6. **Behavioral results:** unit, expression, query, render, integration, wrapper, install/link, property/fuzz/mutation results required by the impact map.
7. **Visual/runtime results:** expected, actual, and diff images; metrics; logs; action journal; screenshots/video; sanitizer traces; device results.
8. **Change budgets:** coverage delta, benchmark distributions, binary-size delta, public symbol delta, architecture exception delta, dependency delta.
9. **Independent evaluation:** hidden-test results, adversarial findings, severity, resolution status, evaluator confidence/calibration data, and explicit pass/fail reason codes.
10. **Recovery:** revert SHA/patch recipe for merge-only tasks; forward migration/repair plan for data or public artifacts; tested rollback or halt result where applicable.
11. **Attestation:** content hashes and signatures binding the evidence to the exact candidate commit and, for release, exact artifact checksums.

The builder's natural-language statement that a change works is metadata, never evidence.

### Proof required by change type

| Change | Required proof beyond the common packet |
| --- | --- |
| Docs | Link/path/command-reference validation; mdBook/Doxygen build as applicable; factual claims traced to repository sources; procedural commands smoke-tested. |
| Generated files | Declared input and generator changed or an approved toolchain update exists; regeneration is deterministic; second generation leaves a clean tree. |
| Core logic | Pre-fix regression or executable scenario; affected C++ tests; expression/query/render suites selected by impact; sanitizer profile for memory/thread-sensitive paths. |
| Rendering/backend | Zero unexplained render diffs across required backends; expected/actual/diff bundle; deterministic seed; device/GPU coverage when the defect is hardware-specific; baseline writes forbidden. |
| Platform wrapper | Core plus platform compile/tests; public symbol/API surface check; minimal sample-app or install/link scenario; lifecycle/threading scenario if relevant. |
| Public API/ABI | API review decision; symbol/header diff; source and binary compatibility fixtures; wrapper parity; changelog/migration guide; no autonomous merge before T3 policy changes. |
| Offline storage/schema | Old→new, interrupted, repeated, corrupt-input, low-disk, and forward-compatibility fixtures; idempotence; data preservation; human-approved migration/recovery plan. |
| Dependency | Provenance and checksum; vulnerability/license/SBOM diff; build/test matrix; API/ABI and size impact; removal/rollback feasibility. |
| Performance/size | Pinned hardware/environment; warmup and repeated samples; distribution and variance; no statistically significant regression beyond an approved subsystem budget; correctness tests remain green. |
| Security | Threat model; hostile input scenarios; CodeQL/static results; targeted fuzz/property tests; secret and dependency checks; independent security evaluator; human security owner approval. |
| CI/release | Least-privilege diff; action pinning; untrusted-input analysis; dry run in non-publishing environment; trusted rebuild; artifact provenance; no autonomous credential or permission expansion. |

### Builder/certifier separation

- The builder receives the public specification, repository map, allowed tools, and visible acceptance checks.
- The certifier receives the specification, exact diff, evidence packet, hidden checks, and policy. It does not receive the builder's chain of reasoning or self-review conclusion.
- The certifier must execute or validate observable behavior; a prose-only review cannot certify a behavioral change.
- The certifier may read implementation to identify risk, but certification is based on outcomes, invariants, and adversarial probes.
- Certification fails on missing evidence, contradictions, unexpected files, weakened tests/ignores, newly skipped checks, or unresolved severity-1/2 findings.
- Initially use one independent evaluator run. Compare same-family and cross-model-family certification only through seeded-fault experiments; do not assume diversity is beneficial.
- The release controller trusts only signed policy and evidence, not a builder or certifier chat message.

### Experiment for removing human code review

Human review is removed only after its controls are measured and replaced:

1. Export a stratified sample of at least 50 recent merged PRs and all substantive review comments from GitHub.
2. Classify each finding: requirement, correctness, platform/backend, API/ABI, security, architecture, testing, performance/size, readability/maintainability, release/governance, or false positive.
3. Replay the pre-review diffs against the proposed certifier without exposing the historical comments.
4. Add seeded variants for rare high-severity classes, including wrong render baselines, weakened assertions, private-header leakage, missing build registration, malicious workflow privilege, corrupt-input handling, and ABI breakage.
5. Compare detection recall, time, false positives, and escaped defects against human review.
6. For each class with at least ten observations, require certifier recall no more than 5 percentage points below the human baseline using a one-sided 95% non-inferiority interval. Require 100% detection in the trial set for credential exposure, data loss, critical security, and unintended public ABI breakage.
7. Keep human review for any class that fails. Convert repeated findings into specification fields, tests, rules, or specialized evaluators, then rerun the experiment.

---

## F. Security and autonomy envelope

### Default capability envelope

| Dimension | Default autonomous rule |
| --- | --- |
| Repository/filesystem | Write only inside an assigned worktree and run-specific build/temp directories. Root, home, other worktrees, developer caches, and signing stores are read-denied. |
| Network | Off by default. Read-only allowlists for declared dependency sources may be granted. Render tests stay offline unless the spec names and records an online fixture. |
| Secrets | Builder and certifier receive none. Test credentials are short-lived, scoped, synthetic, and separately granted. Release secrets are available only to the release controller. |
| Data | Public repository fixtures and synthetic data only. User-provided logs/styles are untrusted and sanitized; private advisory data runs in a separate enclave. |
| External actions | No comments, issues, PR writes, registry writes, S3 writes, device-farm runs, or releases by default. Each requires a named capability. |
| Destructive operations | No history rewriting, baseline rebaselining, ignore expansion, deletion outside declared paths, tag/version change, package overwrite/yank, or credential/permission change without higher-tier policy. |
| Deployment | Ephemeral local/test and non-public prerelease environments only for T0/T1. Public registries are T4. |
| Financial/resource impact | No paid external operation by default. Device-farm or cloud use is capped by the run manifest. Wall-clock/token/cost hard-stop is at most 1.5× the measured p95 successful budget for T0 and 2× for T1; before baselines exist, all paid operations require approval. |
| Rollback | Merge must be cleanly revertible. Data/public-artifact work requires a tested forward recovery plan because package deletion or downgrade cannot be assumed. |

### Risk tiers

| Tier | Examples | Maximum autonomy | Required verification/evaluation | Autonomous deployment | Human approval |
| --- | --- | ---: | --- | --- | --- |
| **T0 — mechanical/reversible** | Non-procedural docs, formatting, deterministic generated refresh with no semantic delta | L5 after gates | Common packet; deterministic checks; one independent certifier; zero unexpected files | Merge and docs preview/deploy may be autonomous | Intent/policy established for the task class, not per implementation |
| **T1 — bounded behavior** | Test additions, local refactor, deterministic single-domain bug fix with pre-fix reproducer; no API/baseline/dependency/version change | Constrained L5 after shadow proof | Full affected matrix, hidden regression, architecture/security checks, independent certifier, tested revert | Merge to protected branch and non-public artifacts; no public package publication | Human defines/approves intent and class policy; no routine code inspection |
| **T2 — broad/reversible with judgment** | Small feature, cross-domain change, dependency update, performance/size work, platform wrapper behavior | L4 initially | T1 plus cross-platform/backend matrix, API/compatibility and performance/size evidence, two independent evaluation modes when experiments justify them | Prerelease/test app only | Human validates outcome, risk, and any architecture exception |
| **T3 — high consequence** | Public API/ABI, render baseline, offline schema, security, licensing, new backend, CI permission/secret boundary | L3 | Specialist checks, threat/compatibility/migration review, adversarial evaluation, full relevant matrix | No autonomous public deployment | Human implementation review and explicit approval |
| **T4 — irreversible/privileged** | Version/tag change, signing, Maven/CocoaPods/npm/App Store/GitHub publication, credential/ownership policy, destructive registry action | L2 | Trusted rebuild, provenance, install smoke tests, full release checklist and recovery rehearsal | No, until a separate release-autonomy proposal is proven | Explicit authorized human action/approval |
| **Forbidden** | Hidden telemetry, unlicensed code import, production secret disclosure, bypassing branch/governance policy, destructive action without recovery | None | Not applicable | Never | Governance change cannot legitimize illegal or unsafe behavior |

### Trust-boundary analysis

Contexts combining **A: untrusted input**, **B: sensitive resources**, and **C: external/state-changing actions** need another boundary:

- `workflow_run` device and comparison workflows consume artifacts or metadata from pull-request runs and obtain AWS/GitHub App capabilities. They must validate provenance, use trusted scripts from the protected branch, isolate artifact execution, and narrowly scope roles. Evidence: [Android device tests](../.github/workflows/android-device-test.yml), [iOS device tests](../.github/workflows/ios-device-test.yml), [PR Linux tests](../.github/workflows/pr-linux-tests.yml), [iOS Bloaty](../.github/workflows/pr-bloaty-ios.yml).
- `pull_request_target` cache cleanup receives untrusted PR metadata and can mutate GitHub caches with a token. It must never execute PR-controlled code or shell-expanded untrusted values. Evidence: [cache cleanup](../.github/workflows/cache-cleanup.yml).
- Issues, comments, styles, tiles, logs, compiler errors, web content, and dependency documentation are untrusted instructions to an agent. A process that reads them must not simultaneously hold registry/signing credentials.
- Release workflows must rebuild from the certified protected commit in a trusted environment. They must not publish a builder-supplied binary merely because its filename matches.
- The Linux compiler-cache path is configured over HTTP and may use an auth key. Autonomous use needs a threat model and scoped read-only mode or a protected transport. Evidence: [Linux CI](../.github/workflows/linux-ci.yml).

Every run must record identity, capabilities used, external requests, files changed, process exits, evidence hashes, certification decision, and human intervention. Audit logs must redact secrets and user data by construction.

---

## G. Benchmark suite

### Representative tasks

Build the suite from historical changes while withholding the original patch from the builder. Each task uses the parent commit as the starting point, a normalized issue/spec, visible acceptance criteria, and hidden checks derived from the accepted patch and regressions.

| ID | Task and class | Historical/repository anchor | Hidden or independent acceptance |
| --- | --- | --- | --- |
| B01 | Correct stale build/generated-code documentation; docs | [`CONTRIBUTING.md`](../CONTRIBUTING.md), [generators](../scripts) | All referenced paths/commands exist; mdBook builds; no unrelated prose rewrite. |
| B02 | Remove duplicate GLFW swap-interval behavior; local refactor | Commit `743ed537942c`, [GLFW view](../platform/glfw/glfw_view.cpp) | Relevant build/test passes; call count and behavior oracle unchanged except duplication. |
| B03 | Fix null access during Vulkan custom-layer pre-render; bug | Commit `65a340537061`, [custom-layer parameters](../src/mbgl/style/layers/vulkan/custom_layer_render_parameters.cpp) | Pre-fix reproducer fails, patched test passes under sanitizer, no OpenGL/Metal regression. |
| B04 | Correct Vulkan surface transform; backend bug | Commit `2e13de25f46d`, [renderable resource](../src/mbgl/vulkan/renderable_resource.cpp) | Transform property/scenario tests and targeted render output match oracle. |
| B05 | Add split/join expressions; small feature | Commit `9a9f83178226`, [expression implementation](../src/mbgl/style/expression/compound_expression.cpp), [expression fixtures](../metrics/integration/expression-tests) | Hidden type/error/unicode cases; style-spec behavior; no unrelated expression changes. |
| B06 | Accept alpha in HSL colors; parser bug | Commit `83ce54ed3e40`, [style conversion tests](../test/style/conversion) | Boundary, invalid, and round-trip property tests; no parser acceptance broadening outside spec. |
| B07 | Convert PMTiles decompression failure to an error response; storage/security | Commit `07f8f33e52e6`, [PMTiles file source](../platform/default/src/mbgl/storage/pmtiles_file_source.cpp) | Corrupt/truncated hostile inputs do not crash; precise error contract; fuzz seeds pass. |
| B08 | Add exception handling to the action journal; reliability | Commit `f1905c521577`, [action journal](../src/mbgl/util/action_journal_impl.cpp), [tests](../test/util/action_journal.test.cpp) | Injected I/O failures; journal remains recoverable; logs contain no sensitive payload. |
| B09 | Render macOS Metal on demand and report drawable size; platform lifecycle | Commits `d508e656926a`/`6485d28b3eb4`, [macOS map view](../platform/macos/src) | App lifecycle scenario; screenshot; no unnecessary render loop; size/orientation correctness. |
| B10 | Reduce Android AAR size with header-only Prefab; performance/package | Commit `95a977e12e1a`, [Android Gradle config](../platform/android/MapLibreAndroid/build.gradle.kts) | Install/link consumer fixture; AAR size distribution; public headers complete; ABI unchanged. |
| B11 | Make Maven publication repository configurable; build/release | Commit `836b86298447`, [publish script](../platform/android/buildSrc/src/main/kotlin/maplibre.gradle-publish.gradle.kts) | Publish to an isolated fake repository; no real credentials/network; defaults remain compatible. |
| B12 | Upgrade Bazel setup/dependencies; dependency | [`MODULE.bazel`](../MODULE.bazel), [Renovate](../.github/renovate.json), commit `2e65a1940c6f` | Provenance/license/vulnerability delta and representative CMake/Bazel/platform builds. |
| B13 | Ensure MLT builds with all Qt configurations; cross-platform build | Commit `5cfd7f165097`, [Qt platform](../platform/qt), [MLT submodule](../vendor/maplibre-tile-spec) | Matrix includes enabled/disabled Rust/MLT configurations and clean source registration. |
| B14 | Rename the public C++ namespace; API/ABI | Commit `550f64be2232`, [public headers](../include/mbgl) | Hidden downstream compile fixtures, symbol diff, all build systems, migration/changelog; expected to remain T3. |
| B15 | Work around Adreno `VK_ERROR_DEVICE_LOST`; hardware-specific bug | Commit `bd92c87d3951`, [Vulkan implementation](../src/mbgl/vulkan), [Android stability app](../platform/android/MapLibreAndroidTestApp) | Target device-family trial, device-loss recovery, render equivalence, resource leak/performance checks. |
| B16 | Change offline cache schema safely; data migration | [offline database](../platform/default/src/mbgl/storage/offline_database.cpp), [offline database tests](../test/storage/offline_database.test.cpp), [benchmark](../benchmark/storage/offline_database.benchmark.cpp) | Upgrade from multiple old fixtures, interruption/retry, low disk, corrupt DB, data preservation and forward recovery. |
| B17 | Modify a release workflow without widening privilege; infrastructure | [release workflows](../.github/workflows), [release policy](../docs/mdbook/src/release-policy.md) | Actionlint, permission diff, untrusted-input attack cases, non-publishing dry run; no secret exposure. |
| B18 | Fix a render regression and decide whether a baseline change is legitimate; visual oracle | [render fixtures](../metrics/integration/render-tests), [render runner](../render-test) | Hidden semantic probes plus expected/actual/diff. The correct result may be “escalate baseline decision,” not modify expected output. |

### Experimental method

- Freeze the task, model/version, harness version, base commit, toolchain/container, network policy, and hardware class.
- Change one significant harness variable per experiment.
- Run at least five fresh, independent trials for important configurations; never reuse conversational or build state except declared read-only caches.
- Randomize task order and include hidden checks unavailable to the builder.
- Treat model upgrades, prompt changes, tool permission changes, impact-map changes, and evaluator changes as new experimental conditions.
- Report median, p90/p95, confidence intervals, and run-to-run variance, not the best run.
- Compare with the previous harness and, where measurable, the current human-reviewed process.
- Separate task failure, harness/infrastructure failure, ambiguous specification, policy refusal, and evaluator error.

### Required metrics

1. Fully certified task success rate.
2. Acceptance-criterion and hidden-check pass rate.
3. False-green certification rate.
4. Escaped defect and regression rate by severity and platform/backend.
5. Architecture, API/ABI, security, dependency, and policy violations.
6. Human interventions and human minutes per successful task, classified by reason.
7. Wall-clock time, compute, device-farm use, tokens, and cost per successful task.
8. Number of attempts, retries, flakes, timeouts, and run-to-run variance.
9. Evidence packet completeness and provenance failures.
10. Merge revert, release halt, rollback/follow-forward rate, and recovery success/time.
11. Certifier precision/recall against human review and seeded faults.
12. Post-merge and prerelease-cohort incident rate.

---

## H. Maturity gates

The numeric thresholds below are provisional governance thresholds, not claims about current performance. They should be ratified before experiments and changed only through a recorded decision, never after seeing a disappointing result.

### L2 → L3: agents may own implementation; humans still review code

- **Hypothesis:** the harness can reliably reproduce, implement, and validate bounded work while humans retain implementation review.
- **Experiment:** B01–B13, five independent runs each, with fresh workspaces and hidden acceptance checks.
- **Success:** at least 85% fully certified success for T0/T1 tasks; at least 95% acceptance-criterion pass rate; zero false-green severity-1/2, security, data-loss, or ABI results; no unauthorized file/network/secret action; median human interventions no more than one per successful task.
- **Minimum observations:** 65 task runs plus 10 live agent-authored PRs under normal human review.
- **Failure/regression:** any critical policy breach, credential exposure, destructive action, or repeated false green; otherwise remain L2 for the failing class and encode the failure.
- **Decision:** advance only T0/T1 classes meeting the threshold; others remain L2.

### L3 → L4: unattended implementation; humans validate outcomes

- **Hypothesis:** specifications, automated checks, and independent certification catch at least the failures routine code review catches for eligible classes.
- **Experiment:** repeat eligible benchmarks with builder/certifier separation; complete the 50-PR review study; run 30 live changes in shadow outcome-review mode.
- **Success:** at least 95% fully certified success and 98% acceptance pass rate; severe-finding recall meets the non-inferiority rule in section E; certifier false-positive rate below 10%; zero critical/security/data-loss/ABI false greens; fewer than 0.25 implementation interventions per task; p95 cost and wall time remain inside ratified budgets.
- **Minimum observations:** 100 independent benchmark runs, 50 historical review replays, and 30 live shadow changes.
- **Failure/regression:** a missed finding class retains human code review; two unexplained certifier disagreements or evidence contradictions suspend unattended operation for the class.
- **Decision:** advance by task class and subsystem, never repository-wide.

### L4 → L5 shadow: run the complete factory without autonomous merge

- **Hypothesis:** the full policy→build→certify→integration→recovery pipeline makes correct merge/halt/escalate decisions without implementation inspection.
- **Experiment:** run all eligible real tasks through the complete factory while the existing human process remains authoritative. Compare decisions before revealing human outcome.
- **Success:** at least 98% correct factory decisions; 100% evidence traceability; zero unauthorized capability use; zero severity-1/2 escapes; all injected recovery drills succeed; escaped-defect rate is no worse than the human-reviewed baseline at 95% confidence.
- **Minimum observations:** 100 eligible real shadow tasks across at least three T0/T1 task classes and two subsystems, plus 20 fault-injection/recovery drills.
- **Failure/regression:** any critical miss, credential boundary violation, uncontained destructive action, or recovery failure resets the affected class to L3 and requires a new five-run benchmark condition.
- **Decision:** only classes with stable confidence intervals enter constrained L5.

### L5 shadow → constrained L5: autonomous completion for low risk

- **Hypothesis:** T0 and selected T1 changes can merge without human implementation inspection while preserving or improving outcomes.
- **Experiment:** progressively expose autonomous branch creation, PR creation, merge, and non-public artifact deployment; start with T0 and enable one T1 class at a time.
- **Success:** at least 99% evidence packet completeness; zero severity-1/2 escapes, policy breaches, or unauthorized baseline/API/version changes; at least 98% task success; revert rate no worse than the matched human baseline; 100% successful revert/halt drills; human minutes are concentrated in intent/governance rather than code review.
- **Minimum observations:** 50 autonomous T0 merges, then 50 autonomous changes in each proposed T1 class.
- **Failure/regression:** one severe incident or two similar medium escaped defects in a rolling 50-change window immediately return that class to shadow mode.
- **Decision:** constrained L5 is a permission for a named class/subsystem/policy version, not a repository badge.

### Constrained L5 → broader L5

- **Hypothesis:** one additional risk dimension can be added without degrading safety or delivery.
- **Experiment:** change exactly one dimension—task class, subsystem, renderer/platform, network capability, or deployment environment—and return to shadow mode for it.
- **Success:** meets or exceeds the constrained-L5 thresholds; certifier/human-review non-inferiority remains valid for the new failure classes; recovery is demonstrated; production/prerelease signals stay within approved budgets.
- **Minimum observations:** 100 shadow or constrained tasks for the expanded envelope, including at least 20 adversarial/fault-injection cases.
- **Failure/regression:** any threshold deterioration returns the new dimension to its prior level; repeated systemic failures lower the shared harness one level.
- **Decision:** public package publication, T3, and T4 work require their own later proposal even if other classes reach L5.

---

## I. Roadmap

No dates are assigned. Progress is evidence-gated.

### P0 — Foundations

| Deliverable | Exit criteria |
| --- | --- |
| Governance contract | Community decides whether and where no-code-inspection operation is compatible with AI policy; risk owners and severity definitions are named. |
| Baseline inventory | Current check duration/noise, skipped/advisory jobs, review finding taxonomy, defect/revert history, architecture edges, human minutes, and costs are recorded. |
| Agent-legible knowledge | Root `AGENTS.md` routes every major task class; stale command/path claims are corrected; canonical engineering docs and freshness checks are agreed. |
| Specification and plan schemas | Required intent, acceptance, risk, scope, recovery, and unresolved-question fields validate mechanically; examples exist for T0–T3. |
| Canonical execution surface | `doctor/list/check/repro` wraps existing tools; at least docs, repository-fast, and Linux-core profiles have local/CI parity and stable exit codes. |
| Evidence schema | Every pilot profile emits `summary.json` on success or failure; artifact paths, environment, skipped/quarantined state, and hashes are complete. |
| Security boundary design | Builder/certifier/release identities, filesystem/network/secret policies, prompt-injection rules, audit/redaction, and release isolation receive security review. |

**P0 exit:** B01–B04 can be reproduced from clean workspaces by a human or agent using only the root map and canonical commands; evidence packets validate; no broad repository search or workflow-log scraping is required.

### P1 — Reach reliable L3

| Deliverable | Exit criteria |
| --- | --- |
| Impact map | Changed paths select checks with explicit reasons; comparison against full CI demonstrates no known missed impacted checks in the benchmark. |
| Architecture ratchet | Current dependency graph is baselined; new public/private, cross-backend, test/production, and build-registration violations fail with repair guidance. |
| Structured tests | GoogleTest, expression, query, render, benchmark, size, sanitizer, and device results required by pilot profiles are indexed in the evidence packet. |
| Reliability ledger | Every ignore, retry, disabled test, and `continue-on-error` has owner domain, reason, issue, policy, measured noise, and expiry. |
| Reproduction bundles | Core, render, and one platform/device issue can be captured with seed, renderer, toolchain, logs, action journal, images, and sanitized data. |
| L3 benchmark | L2→L3 thresholds pass for eligible T0/T1 classes; ten live PRs retain human code review. |

**P1 exit:** agents may implement qualifying tasks end-to-end, but maintainers still inspect implementation before merge.

### P2 — Reach reliable L4

| Deliverable | Exit criteria |
| --- | --- |
| Independent certifier | Separate context, hidden checks, severity taxonomy, calibrated pass/fail reasons, and no trust in builder conclusions. |
| Review replacement controls | Historical review study and seeded faults show which human findings are caught; failed categories retain human review. |
| Adversarial evidence | Targeted parser/storage fuzzing, useful mutation tests, malicious workflow/input cases, and test-weakening detection are added only where experiments show value. |
| Resumable execution | Durable checkpoints allow clean retry after infrastructure failure without reusing untrusted mutable state or duplicating external actions. |
| L4 benchmark/live shadow | L3→L4 thresholds pass for each promoted class; 30 live changes complete unattended and humans validate outcomes/evidence. |

**P2 exit:** humans can stop reading implementation for the explicitly proven L4 classes but still approve outcomes.

### P3 — Prove L5 in shadow mode

| Deliverable | Exit criteria |
| --- | --- |
| Full factory controller | Policy grants, budgets, builder, checks, certification, integration simulation, audit, and recovery operate as one inspectable state machine. |
| Protected integration | Evidence is revalidated on target-branch HEAD and cryptographically bound to the candidate commit. |
| Recovery drills | Revert, interrupted run, corrupt cache, flaky device, unavailable dependency, evaluator disagreement, and policy-revocation drills meet recovery targets. |
| Shadow ledger | 100 real tasks and 20 fault drills meet the L5-shadow gate; factory decisions are recorded before human decisions. |

**P3 exit:** the factory is proven to make bounded decisions but has no autonomous merge authority.

### P4 — Constrained production L5

Here “production” means the protected integration branch plus documentation sites or non-public/prerelease artifacts for eligible classes. It does not mean Maven/CocoaPods/npm/App Store publication.

| Exposure step | Exit criteria |
| --- | --- |
| Autonomous branch/PR | Packet and policy are visible; no external writes beyond the PR; 20 successful tasks before merge authority. |
| Autonomous T0 merge | 50 qualifying merges meet the constrained-L5 gate; automatic revert/halt is proven. |
| Autonomous T1 merge | Enable one task class/subsystem at a time; 50 changes each; no API/baseline/version/dependency expansion. |
| Non-public artifacts/test apps | Provenance/install/device checks pass; costs and external actions remain within granted budgets. |

**P4 exit:** humans normally provide intent and policy but do not write or inspect implementation for the named T0/T1 envelope.

### P5 — Expand the autonomy envelope

- Expand one risk dimension at a time and return it to shadow mode.
- Establish privacy-reviewed opt-in prerelease cohorts for representative downstream apps before claiming production observation.
- Add subsystem-specific digital twins only when a benchmark demonstrates a third-party dependency is a recurring failure source.
- Consider T2 L5 only when review replacement, runtime observation, and recovery meet the broader-L5 gate.
- Keep T3/T4 at lower levels until separate evidence and governance decisions exist.

### Continuous architecture and entropy control

Convert repeated feedback through **principle → executable rule/test → automated maintenance**:

| Entropy source | Continuous control |
| --- | --- |
| Stale docs | Validate paths, registered commands, canonical links, status, and covered-path freshness. |
| Duplicated abstractions/dead code | Report-only reachability and duplicate scans; require build/test proof before small deletions; never bulk-delete autonomously. |
| Architecture drift | Dependency-edge and exception ratchet; domain ownership; backend/public-private rules. |
| Dependency creep | SBOM/license/vulnerability/provenance delta and dependency budget. |
| Oversized modules/complexity | Trend module coupling and change hotspots first; introduce limits only after correlation with defects. |
| Weakened tests | Detect deleted assertions, new skips/ignores, coverage and mutation deltas, and changed expected images. |
| Inconsistent APIs | Header/symbol/schema diffs plus wrapper parity fixtures. |
| Recurring incidents | Post-incident fixture, invariant, recovery drill, and benchmark case required before autonomy is restored. |

Gardening begins report-only. Autonomous cleanup is limited to one rule/domain and the same T0/T1 gates as feature work.

### Keep the harness evolvable

| Component | Failure mode addressed | Benefit measure | Removal/simplification experiment |
| --- | --- | --- | --- |
| Root map | Discovery errors | Time/attempts to first correct check | Remove a route in controlled onboarding trials; retain only if failures rise. |
| Command dispatcher | Local/CI drift | First-pass CI rate, setup time | Compare direct documented commands versus wrapper at fixed tasks. |
| Impact map | Excess CI or missed checks | Miss rate and compute saved | Periodically compare selected versus full matrix; simplify rules with zero marginal value. |
| Architecture ratchet | Structural drift | New violations/exceptions and defect correlation | Disable one rule in replay; remove if it finds no meaningful failures and costs exceed benefit. |
| Independent certifier | Self-review conflict | Seeded-fault recall and false positives | Compare builder self-review, independent same-model, and cross-model conditions. |
| Mutation/fuzzing | Weak tests/hostile parsers | Unique defects found per compute | Keep only targets with demonstrated yield. |
| Digital twin | Third-party/device nondeterminism | Reproduction and false-green rate | Compare against fixtures or real sandbox; remove if it adds no fidelity. |
| Orchestrator state | Lost/repeated work | Recovery success and duplicate external actions | Prefer simple CI/workflow state until failures justify more infrastructure. |

Interfaces between model, orchestration, tools, execution, durable state, evaluation, and deployment should be versioned and replaceable. A model upgrade is never an in-place assumption; it starts a new benchmark condition.

### Human role at constrained L5

| Activity | Change |
| --- | --- |
| Routine implementation and mechanical code review | Disappears inside the proven envelope. |
| Manual log archaeology, check selection, formatting feedback, generated refreshes | Should largely disappear. |
| Product intent, prioritization, architecture principles, API direction, compatibility, licensing, security boundaries, acceptable failure | Remains human-owned. |
| Acceptance-scenario, invariant, benchmark, evaluator, observability, and recovery design | Increases. |
| Factory calibration, cost/performance analysis, evaluator disagreement, incident analysis, entropy control | Increases. |
| Public release approval, private security response, irreversible decisions, community communication | Remains human-owned until separately proven and governed. |

Engineers become specification, systems, reliability, security, and factory engineers. In an open-source project, accountable human communication and community governance do not disappear merely because implementation inspection does.

---

## J. First experiments

### 1. Baseline what human review catches

**Hypothesis:** most recurring review findings can be categorized and at least some can be converted to executable controls.

**Change:** export and blind at least 50 recent PRs; classify substantive comments; replay pre-review diffs against current checks.

**Measurement:** finding frequency/severity, current automated detection, human minutes, and candidates for spec/test/rule/evaluator replacement.

**Success threshold:** at least 80% of mechanical finding instances map to a plausible executable control; all high-severity categories have an accountable owner and proposed control.

**Next decision:** choose which task classes are credible L3/L4 pilots. If findings are predominantly irreducible product/visual judgment, keep those classes human-reviewed.

### 2. Canonical command and evidence pilot on three tasks

**Hypothesis:** a thin command surface and common result envelope reduce ambiguity without replacing established build tools.

**Change:** specify, but do not yet generalize beyond, `docs`, `repo-fast`, and `core-linux-fast` profiles; replay B01, B03, and B18.

**Measurement:** time to first valid check, incorrect command attempts, CI parity, packet completeness, setup failures, wall time.

**Success threshold:** 100% result packets on pass/fail; zero missing required child command versus CI; at least 30% reduction in median discovery/triage time versus direct repository exploration.

**Next decision:** extend to Android/Apple only if the schema and dispatcher stay thin and reliable.

### 3. Independent certification versus self-review

**Hypothesis:** a separate certifier with hidden checks catches more seeded failures than builder self-review at acceptable cost.

**Change:** run B02–B08 with correct and fault-seeded variants under self-review, independent same-model review, and—only as an experiment—cross-model review.

**Measurement:** severity-weighted recall, precision, false greens, wall time, tokens/cost, and variance.

**Success threshold:** independent review improves severity-weighted recall by at least 15 percentage points over self-review, catches 100% of critical seeds, and costs less than 2× the self-review condition.

**Next decision:** adopt the simplest winning evaluator; reject extra agents or cross-model routing if benefit is not demonstrated.

### 4. Affected-check selection shadow

**Hypothesis:** a versioned impact map can reduce feedback cost without missing required platform/backend checks.

**Change:** for at least 50 historical changes, predict the matrix before observing touched tests/workflow outcomes; compare with full CI and expert classification.

**Measurement:** false-negative rate, false-positive compute, explanation quality, and changed-path categories responsible for misses.

**Success threshold:** zero false negatives for public headers, shared renderer/core, build/release, and security paths; at least 25% reduction in unnecessary fast-loop jobs for low-risk paths.

**Next decision:** keep full matrices if selection cannot meet zero critical misses; use selection only for local feedback until proven.

### 5. Recovery-before-autonomy drill

**Hypothesis:** bounded runs can halt and recover without corrupting state or repeating external actions.

**Change:** inject compiler failure, lost worker, corrupt cache, flaky device response, certifier disagreement, budget exhaustion, and forbidden version/baseline write into B01–B04.

**Measurement:** containment, duplicate actions, preserved audit/evidence, resume success, revert success, and recovery time.

**Success threshold:** 100% containment and audit preservation; zero repeated external/state-changing action; 100% clean resume or safe abort; forbidden writes never reach a candidate commit.

**Next decision:** no autonomous merge until every drill passes.

---

## K. Failure and rollback strategy

### If the agent is confidently wrong

| Action | Failure effect | Required containment/recovery |
| --- | --- | --- |
| Worktree edit | Wrong code/docs generated | Path policy blocks out-of-scope writes; discard isolated worktree or revert candidate commit. |
| Test or golden modification | Wrong behavior becomes the oracle | Test weakening, ignores, and expected-image writes are higher-tier diffs; independent hidden checks; no autonomous rebaseline. |
| Merge to `main` | Other work builds on a defect | Automatic halt for the class; create tested revert; rerun integration packet on HEAD; notify maintainers. |
| Offline schema change | Consumer data is corrupted/unreadable | T3 only; old/new/corrupt/interrupted fixtures; forward repair; do not assume app downgrade. |
| Device/cloud action | Cost or external state leaks | Per-run identity/quota/idempotency key; one-shot action ledger; kill capability at budget. |
| Public package publish | Immutable bad artifact reaches consumers | T4 human gate; halt later channels; deprecate/yank only where safe; publish a certified forward fix; communicate compatibility. |
| Credential/permission change | Supply-chain compromise | Builder has no secrets; policy denies expansion; rotate/revoke on any suspected exposure; invalidate attestations. |

### Automatic halt and downgrade conditions

Immediately stop the run and revoke its capabilities on:

- any secret access not declared in policy;
- a write outside the allowed worktree/build paths;
- a version, tag, release, baseline, ignore, permission, public API, submodule, or dependency change not granted by the tier;
- missing, invalid, contradictory, or hash-mismatched evidence;
- unresolved severity-1/2 finding;
- budget/time/token/cost limit;
- certifier disagreement with no deterministic tie-break;
- non-idempotent retry risk;
- target branch movement that invalidates evidence.

Return an affected L5 task class to shadow after one severity-1/2 escape, one security boundary violation, one failed recovery drill, or two similar medium escapes in 50 changes. Return the shared factory to L3 after a systemic provenance/policy failure affecting multiple classes. Revalidation requires root-cause analysis, a new regression/benchmark case, five fresh trials under a new harness version, and the applicable maturity gate.

No published package is described as “rolled back” unless the target registry and downstream clients demonstrably support it. The default recovery is halt → diagnose → certified follow-forward patch → communicate.

---

## L. What not to build

Current evidence does not justify:

- a swarm of planner, coder, reviewer, tester, manager, and debate agents;
- a custom MCP server for commands already available through the filesystem, shell, GitHub Actions, or existing CLIs;
- a new build system or a second CI recipe hidden behind “agent tooling”;
- a large orchestration platform before simple workflow state and signed JSON packets fail measured recovery tests;
- a vector database/RAG layer before the root map and canonical docs are made accurate;
- automatic render-baseline updates or broad test-ignore changes;
- repository-wide mutation testing or fuzzing without target-by-target defect yield;
- full digital twins for every platform, GPU, registry, or remote tile service;
- autonomous public release, signing, security advisory handling, or credential rotation;
- hidden end-user telemetry added to the SDK for the factory;
- global auto-merge based only on green CI, code coverage, or an agent review;
- cross-model-family review as doctrine rather than an experimental condition;
- arbitrary complexity/file-size/style rules that are not tied to defects or architecture principles;
- a large approval workflow full of confirmation prompts in place of capability restriction;
- a vendor-locked model/orchestrator/evidence format when a small versioned interface suffices.

---

## M. Open human decisions

The repository-analysis agent cannot legitimately decide:

1. Whether MapLibre's AI contribution policy should permit any merge without human implementation verification.
2. Which maintainers or community bodies own autonomy policy, severity definitions, architecture exceptions, and factory shutdown authority.
3. Which subsystems and task classes are acceptable T0/T1 pilots.
4. The acceptable escaped-defect, false-green, cost, latency, and downstream-risk budgets.
5. Whether “production” may initially mean protected-main plus non-public artifacts, or whether L5 terminology must be reserved for public package publication.
6. Which public API/ABI, rendering, compatibility, licensing, security, and release decisions must remain permanently human.
7. Whether opt-in downstream prerelease telemetry/cohorts are acceptable, useful, fundable, and privacy-compliant.
8. Which devices, GPU families, OS versions, renderers, and consumer integration fixtures define supported certification coverage.
9. Whether GitHub branch/environment protections outside this repository already provide sufficient release approval and separation.
10. What artifact, log, image, trace, and reproduction-bundle retention/redaction policy is acceptable.
11. Which external network destinations, cloud budgets, and temporary credentials agents may receive.
12. Whether current release automation should be decoupled from version changes on `main` before autonomous merge is piloted.
13. What constitutes an acceptable render oracle and who may approve new or changed golden images.
14. How private security advisory work can use agents, if at all, without exposing sensitive reports or credentials.
15. How the project will fund and staff benchmark maintenance, evaluator calibration, incident response, and architecture/reliability gardening.

## API modifications

This proposal makes no MapLibre Native public API or ABI change. Any future factory implementation changes repository process, CI, documentation, and developer tooling and must preserve public compatibility unless a separate T3 design proposal approves otherwise.

## Migration and compatibility

Adoption is progressive: report-only → shadow → agent PR → constrained merge → non-public deployment → envelope expansion. Existing contributor commands remain the underlying source of execution truth while a thin dispatcher is introduced. Human review remains the default for every task class until that class passes its gate. The factory must always be able to become less autonomous.

## Final recommendation

Approve P0 as an investigation and governance program, not approval to deploy an autonomous factory. The repository already has enough high-value testing, rendering fixtures, platform CI, device infrastructure, and release automation to make a rigorous experiment worthwhile. It does not yet have the specification discipline, independent certification, containment, recovery, or downstream observation required to remove humans from routine implementation and code review.

The first success criterion is not more generated code. It is a measured answer to: **for which precise MapLibre Native decisions does repository evidence now outperform human implementation review?**
