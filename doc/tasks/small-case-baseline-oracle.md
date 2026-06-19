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

- [ ] Add a brute-force MST helper guarded by a small limit such as `n <= 80`.
- [ ] Build all pairwise Manhattan edges only inside the oracle helper.
- [ ] Run MST on the complete tiny graph using DSU or an independent simple MST helper.
- [ ] Compare optimized totals against oracle totals for golden and randomized small cases.
- [ ] Include failing seed, `n`, and point list in diagnostics if a self-test path is implemented.
- [ ] Keep the oracle out of the normal large-input CLI path.

## Tests and Quality Gates

- [ ] Run oracle differential checks for deterministic seeds `20260608`, `20260609`, `1`, `2`, and `3` once a test command exists.
- [ ] Verify the oracle refuses inputs above its limit.

## Done When

- [ ] Oracle returns exact totals for hand-solvable cases.
- [ ] Optimized solver matches oracle on planned randomized small cases.
- [ ] Production execution cannot accidentally build a complete graph for large inputs.
