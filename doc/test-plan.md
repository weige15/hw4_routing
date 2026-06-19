# Test Plan

## Purpose

This plan defines how to verify the RMST solver before treating implementation as complete. "Done" means the program builds through the repository `Makefile`, accepts the assignment CLI, produces exactly the required RMST total, matches a brute-force oracle on small cases, handles documented edge cases, and shows scalable behavior without complete-graph construction in the production path.

No tests, source files, build commands, or evaluator commands exist yet. All concrete checks below are `Planned`, `Missing`, or `Unknown` unless explicitly marked otherwise.

## Source Requirements

| Source | Status | Requirements Extracted |
| --- | --- | --- |
| `doc/proposal.md` | Existing | C++ solver, fast file I/O, compact storage, small-case baseline oracle, optimized Manhattan MST candidate generation, Kruskal, sample smoke check, randomized baseline comparisons. |
| `doc/high-level-design.md` | Existing | Modules, data flow, interfaces, CLI contract, output contract, quality alignment, risks, assumptions, open questions. |
| `doc/problem-brief.md` | Existing | Assignment objective, input/output format, constraints, sample case, grading policy, deliverables, open questions. |
| `doc/repo-map.md` | Existing, partly stale | Repository is documentation-only; no source, tests, scripts, build system, or evaluator files found. Current file discovery also shows proposal, HLD, and this planned test plan. |
| `doc/quality-gates.md` | Existing | Build/test/lint/evaluator commands are missing; recommended minimum done criteria include build, sample smoke check, CLI check, output-only check, 64-bit arithmetic, and no O(n^2) production graph. |
| `p4_routing.pdf` | Existing | Assignment PDF source for RMST definition, constraints, CLI, deliverables, and grading policy. |

Missing or ambiguous source material:

- Exact compiler, C++ standard, and optimization flags are `Unknown`.
- Memory limit beyond the 1-hour grading timeout is `Unknown`.
- Exact executable name is `Unknown`, though the PDF example uses `RMST`.
- External-library policy is `Unknown`; proposal assumes standard library only.
- Malformed-input behavior is `Unknown`.
- Duplicate-coordinate implementation policy is `Unknown`, but the RMST total must be preserved.

## Test Scope

- CLI workflow: `[executable] [input file] [output file]`.
- Input parser for `n` and `n` integer coordinate pairs.
- Point storage with stable ids and signed coordinate handling.
- Manhattan distance calculations using signed 64-bit arithmetic.
- Sparse Manhattan MST candidate generation.
- Candidate edge storage with valid endpoints and 64-bit weights.
- Kruskal MST over generated candidates.
- Disjoint-set union operations.
- Output writer that emits only the final total.
- Small-case brute-force oracle for correctness checks.
- Golden, randomized, edge-case, and performance-oriented checks.
- Required assignment artifacts: source code, `Makefile`, and `readme.txt`.

## Non-Tested Scope

- Hidden grader internals and private benchmark data.
- E3 upload behavior and final tar submission, except for manual artifact checks.
- Alternative algorithms not selected by the proposal.
- Third-party library integration, because the proposal assumes standard library only.
- CI behavior, because no CI configuration exists.
- Exact memory-limit conformance beyond observable local peak memory, because the assignment does not state a memory limit.

## Smoke Tests

| Test | Status | Input | Expected Result | Pass/Fail Criteria |
| --- | --- | --- | --- | --- |
| Build through repository command | Missing | Repository after `Makefile` exists. | Executable is produced. | Pass if the documented build command exits 0 and creates the expected executable. |
| PDF sample end-to-end | Planned | `5; (0,0), (2,0), (2,3), (5,1), (6,4)` | `13` | Pass if CLI writes exactly `13` with optional final newline and no labels or extra tokens. |
| Single point | Planned | `1; (0,0)` | `0` | Pass if no edge is required and output is `0`. |
| Duplicate pair | Planned | `2; (7,-3), (7,-3)` | `0` | Pass if zero-distance duplicate connectivity is handled. |
| Missing arguments | Planned | No input or output path. | Nonzero exit or clear failure. | Pass condition depends on finalized error policy; output file must not contain a fake result. |

## Unit Tests by Module

| HLD Module | Status | Planned Verification |
| --- | --- | --- |
| CLI and Orchestrator | Missing | Check exact argument count, input/output path propagation, nonzero failure on missing paths, and no accidental stdin/stdout-only behavior. |
| Input Reader | Missing | Parse valid files, extra whitespace, negative coordinates, coordinate limits, duplicates, incomplete files, and too few coordinate lines. |
| Point Storage | Missing | Preserve point count, coordinates, stable ids, duplicate vertices, and coordinate values at `-1e9` and `1e9`. |
| Manhattan MST Candidate Generator | Missing | For small cases, verify generated candidates allow Kruskal to match the brute-force oracle; verify endpoint ids are valid and candidate count remains sparse. |
| Edge Storage | Missing | Verify edge weights are 64-bit Manhattan distances, endpoints are in range, duplicate edges do not change MST total, and sorting by weight is deterministic enough for total correctness. |
| Disjoint-Set Union | Missing | Verify initial singleton components, repeated unions, path compression behavior, union by size/rank, and no merge when endpoints already share a component. |
| Kruskal MST Solver | Missing | Verify known weighted candidate graphs, `n = 1`, duplicate zero-weight edges, tie weights, early stop after `n - 1` accepted edges, and 64-bit total accumulation. |
| Output Writer | Missing | Verify output file contains one decimal integer, optional final newline only, and no logs or diagnostics mixed into the output file. |
| Small-Case Baseline Oracle | Missing | Verify brute-force baseline on hand-solvable cases; enforce a tiny-input guard so it is not used for large production inputs. |
| Build and Submission Artifacts | Missing | Verify `Makefile` builds the executable and `readme.txt` documents build and assignment-style usage. |

## Integration Tests

| Test | Status | Modules Covered | Pass/Fail Criteria |
| --- | --- | --- | --- |
| End-to-end sample | Planned | CLI, Input Reader, Point Storage, Candidate Generator, Edge Storage, Kruskal, Output Writer | Input file from PDF produces output file containing `13` only. |
| End-to-end golden suite | Planned | Full production pipeline | Every golden case in this plan produces the exact expected total. |
| Optimized-vs-baseline small suite | Planned | Candidate Generator, Edge Storage, Kruskal, Baseline Oracle | For bounded small inputs, optimized total equals brute-force total. |
| Output-file contract | Planned | CLI, Output Writer | Result is written to the requested output file and stdout is not required for correctness. |
| Invalid invocation behavior | Planned | CLI, Input Reader, Output Writer | Missing paths or unreadable input fail without writing a valid-looking incorrect answer. Exact diagnostics remain open. |
| Large structured input | Planned | Input Reader, Point Storage, Candidate Generator, Kruskal, Output Writer | Known-structure large cases complete and produce expected totals without complete-graph memory growth. |

## Golden Test Cases

| Name | Status | Input Points | Expected Output | Reason |
| --- | --- | --- | --- | --- |
| PDF sample | Planned | `(0,0), (2,0), (2,3), (5,1), (6,4)` | `13` | Assignment-provided sample. |
| One point | Planned | `(0,0)` | `0` | Lower bound on `n`. |
| Two distant points | Planned | `(-1000000000,-1000000000), (1000000000,1000000000)` | `4000000000` | Verifies 64-bit distance beyond signed 32-bit. |
| Duplicate and one neighbor | Planned | `(0,0), (0,0), (2,0)` | `2` | Duplicate zero edge plus one positive edge. |
| Unit square | Planned | `(0,0), (0,1), (1,0), (1,1)` | `3` | Many equal-weight MST choices with same total. |
| X-axis collinear | Planned | `(0,0), (2,0), (5,0), (6,0)` | `6` | Collinear Manhattan MST equals sorted adjacent gaps. |
| Same x coordinate | Planned | `(3,-2), (3,0), (3,5)` | `7` | Vertical collinear case. |
| Negative coordinates | Planned | `(-1,-1), (2,-1), (2,3)` | `7` | Mixed signs with hand-checkable MST. |
| Duplicate clusters | Planned | `(0,0), (0,0), (0,0), (5,5), (5,5)` | `10` | Zero edges within clusters, one inter-cluster edge. |
| Center and corners | Planned | `(0,0), (10,0), (0,10), (10,10), (5,5)` | `40` | Equal-distance ties across many candidate edges. |

Each golden input should be checked through the final CLI, not only internal functions.

## Oracle or Reference Implementation Strategy

Status: `Planned`.

Use a brute-force small-case oracle that builds all pairwise Manhattan edges and runs a simple MST algorithm. This oracle is allowed only for bounded local tests and must not be used for production-sized inputs.

Oracle constraints:

- Use signed 64-bit weights and totals.
- Run only for small `n`, such as `n <= 80`, unless a stricter implementation guard is chosen.
- Keep it independent from the optimized candidate generator.
- Fail the test if optimized and oracle totals differ.
- Include the failing seed, `n`, and full point list in diagnostics so the case can become a regression test.

The oracle checks correctness only. It does not prove large-input performance.

## Randomized or Property Tests

Status: `Planned`.

Randomized differential tests:

- Deterministic seeds: `20260608`, `20260609`, `1`, `2`, `3`.
- Input size range: `1 <= n <= 80`.
- Coordinate range: `-50 <= x,y <= 50`.
- Duplicate-heavy mode: sample from a smaller grid such as `-3 <= x,y <= 3`.
- For each case, compare optimized total against the brute-force oracle.

Properties to check:

- Point order does not affect the RMST total.
- Translating every point by the same `(dx, dy)` does not affect the total.
- Swapping `x` and `y` for every point does not affect the total.
- Reflecting all points across an axis does not affect the total.
- Adding a duplicate of an existing point does not change the total.
- Scaling all coordinates by a nonnegative integer `k` scales the total by `k`.

Shrinking expectation:

- When a randomized case fails, reduce by removing points, reducing coordinate magnitudes, and preserving at least one mismatch against the oracle.

## Edge Cases

| Case | Status | Expected Behavior |
| --- | --- | --- |
| `n = 1` | Planned | Output `0`. |
| `n = 10,000,000` | Planned benchmark | Completes within grading timeout on suitable hardware; exact local threshold is `Unknown`. |
| All points identical | Planned | Output `0`. |
| Many duplicate groups | Planned | Preserve zero-weight connectivity and correct total between unique coordinate groups. |
| Coordinates at `-1e9` and `1e9` | Planned | Distances and total remain signed 64-bit. |
| Total exceeds signed 32-bit | Planned | Output correct 64-bit decimal value. |
| Same `x` coordinate | Planned | Equivalent to 1D vertical MST. |
| Same `y` coordinate | Planned | Equivalent to 1D horizontal MST. |
| Already sorted input | Planned | Same result as shuffled input. |
| Reverse sorted input | Planned | Same result as shuffled input. |
| Extra whitespace and blank lines | Planned | Parse if compatible with normal integer scanning. |
| Too few coordinate lines | Planned | Fail without producing a valid-looking wrong answer. Exact error contract is open. |
| Too many trailing tokens | Unknown | Behavior not specified by assignment; decide during implementation. |
| `n = 0` | Unknown | Outside constraints; malformed-input behavior is open. |
| Multi-case input | Not in scope | Assignment defines one point set per file. |

## Performance Benchmarks

Status: `Planned`, with commands `Unknown`.

Benchmark goals:

- Confirm production implementation does not show O(n^2) memory or time behavior.
- Measure runtime and peak memory for sorting, candidate generation, and Kruskal-heavy workloads when instrumentation exists.
- Confirm no complete graph is built in the production path.

Planned workloads:

| Workload | Size | Expected Result or Check | Purpose |
| --- | --- | --- | --- |
| Random small | `1e3` to `1e4` | Compare against oracle where feasible. | Catch correctness issues before large runs. |
| Random medium | `1e5` to `1e6` | Completes and writes one integer. | Check scaling and memory trend. |
| Collinear large | Up to available memory, ideally `1e6+` | Output equals `max_coordinate - min_coordinate` for sorted unique line points. | Large case with known answer. |
| All-duplicate large | Up to available memory, ideally `1e6+` | Output `0`. | Duplicate handling and memory behavior. |
| Assignment-scale stress | `10,000,000` | Completes within 1 hour on grading-like hardware. | Matches stated maximum constraint. |

Exact benchmark commands are `Unknown` until the `Makefile`, executable name, and any benchmark input generator are defined.

## Evaluator or Grading Commands

| Command or Gate | Status | Notes |
| --- | --- | --- |
| Build command | Missing | No `Makefile` exists yet; expected future command is repository-defined by `Makefile`, but not verified. |
| Unit test command | Missing | No tests or runner exist yet. |
| Integration test command | Missing | No integration tests or runner exist yet. |
| Smoke test command | Missing | PDF sample exists in docs, but no runnable smoke-test command exists. |
| Benchmark command | Missing | No benchmark scripts or evaluator assets exist. |
| Official grader command | Unknown | Not provided by the PDF or repository docs. |

No build, test, benchmark, or evaluator command has been run or verified as part of this plan.

## Regression Tests

Status: `Planned`.

Add any failing randomized or edge case as a stable regression once discovered. Initial regression candidates should include:

- PDF sample output `13`.
- 64-bit two-point distance output `4000000000`.
- All-duplicate output `0`.
- Duplicate clusters output `10`.
- Same `x` and same `y` collinear cases.
- Any candidate-generator bug found by optimized-vs-brute-force comparison.

Each regression should record the exact input, expected output, and reason it was added.

## Manual Verification

Status: `Planned`.

Manual checks before submission:

- Inspect production code path to confirm it does not construct all pairwise edges for normal inputs.
- Inspect distance and total types to confirm signed 64-bit arithmetic is used.
- Inspect duplicate-coordinate handling and confirm RMST total preservation.
- Inspect `Makefile` for Linux-compatible compiler and optimization flags.
- Inspect `readme.txt` for build command and assignment-style usage.
- Confirm output file contains only the integer result, not timing logs or debug output.
- Confirm required files for submission are in the repository root unless a later convention says otherwise.

## Minimum Done Criteria

Implementation is not done until all applicable criteria pass:

- `Makefile` exists and the repository-defined build command succeeds.
- The executable accepts `[executable] [input file] [output file]`.
- The PDF sample produces exactly `13`.
- Every golden test case in this plan passes through the CLI.
- Small randomized optimized-vs-oracle tests pass for the planned deterministic seeds.
- Edge cases for one point, duplicates, negative coordinates, collinear points, coordinate extremes, and 64-bit totals pass.
- Output files contain only one integer with no labels or debug text.
- Production code avoids complete-graph construction and O(n^2) behavior.
- Performance benchmarks are run or explicitly marked blocked by missing hardware or unknown grader constraints.
- `readme.txt` documents how to build and run the program.

This plan is ready to feed detailed design when every HLD module has at least one planned verification method and all unknown commands remain clearly marked as `Missing` or `Unknown`.

## Open Questions

- Should the executable be named exactly `RMST`?
- What compiler and C++ standard should the `Makefile` use?
- Is there a memory limit beyond the 1-hour runtime limit?
- Are third-party libraries disallowed, or just unnecessary?
- What malformed-input behavior should be required for grading?
- Can exact duplicate coordinates be deduplicated internally if the RMST total is unchanged?
- What student ID should be used for final packaging checks?
- Should implementation include a `make test` target, a standalone self-test mode, or only manual sample commands?
