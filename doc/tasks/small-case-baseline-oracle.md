# Small-Case Baseline Oracle

## Goal

Implement a tiny-input brute-force MST oracle for local correctness checks against the optimized solver.

## Inputs

- `doc/proposal.md`: complete graph plus Kruskal is allowed only as a small-case oracle.
- `doc/high-level-design.md`: Oracle is development/test-only and compares against Candidate Generator and Kruskal.
- `doc/detailed-design.md`: `brute_force_mst_total(points, total)` with a hard small `n` limit.
- `doc/test-plan.md`: deterministic randomized tests, golden cases, oracle constraints, and regression strategy.

## Write Scope

`main.cpp` oracle helper or a future local self-test section. May add a minimal self-test path only if implementation chooses that route.

## Read Scope

Point Storage, Edge Storage, DSU, Kruskal MST Solver, Candidate Generator, and golden/randomized test cases.

## Dependencies

Point Storage and Disjoint-Set Union. Optional dependency on Edge Storage helper for weight calculation.

## Tasks

- [x] Add a brute-force MST helper guarded by a small limit such as `n <= 80`.
- [x] Build all pairwise Manhattan edges only inside the oracle helper.
- [x] Run MST on the complete tiny graph using DSU or an independent simple MST helper.
- [x] Compare optimized totals against oracle totals for golden and randomized small cases.
- [x] Include failing seed, `n`, and point list in diagnostics if a self-test path is implemented.
- [x] Keep the oracle out of the normal large-input CLI path.

## Tests and Quality Gates

- [x] Run oracle differential checks for deterministic seeds `20260608`, `20260609`, `1`, `2`, and `3` once a test command exists.
- [x] Verify the oracle refuses inputs above its limit.

## Done When

- [x] Oracle returns exact totals for hand-solvable cases.
- [x] Optimized solver matches oracle on planned randomized small cases.
- [x] Production execution cannot accidentally build a complete graph for large inputs.

## Notes

- 2026-06-20 core MST cycle: `make test` covers one-point, coordinate-extreme two-point, duplicate-plus-neighbor, and unit-square oracle totals, plus refusal at 81 points.
- 2026-06-20 final solver cycle: `make test` compares optimized totals against the brute-force oracle for all golden cases and 120 cases for each seed `20260608`, `20260609`, `1`, `2`, and `3`.
