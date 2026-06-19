# Manhattan MST Candidate Generator

## Goal

Generate a sparse candidate edge set that is sufficient for Kruskal to recover the exact Manhattan RMST without constructing the complete graph.

## Inputs

- `doc/proposal.md`: optimized Manhattan MST candidate-edge generator using geometric transformations and directional sweeps.
- `doc/high-level-design.md`: Candidate Generator reads Point Storage and writes Edge Storage.
- `doc/detailed-design.md`: duplicate zero-edge prepass, four directional sweep passes, and ordered active sweep.
- `doc/test-plan.md`: optimized-vs-brute-force oracle comparisons, golden cases, edge cases, and linear candidate-count checks.

## Write Scope

`main.cpp` candidate generation functions, working coordinate arrays, ordering arrays, ordered active sweep helper, and edge emission calls.

## Read Scope

`doc/detailed-design.md`, Point Storage, Edge Storage, Kruskal MST Solver, Small-Case Baseline Oracle, and randomized/golden test cases.

## Dependencies

Point Storage, Edge Storage, Kruskal MST Solver for validation, and Small-Case Baseline Oracle for correctness comparison.

## Tasks

- [x] Add duplicate-coordinate prepass that emits zero-weight edges between consecutive identical coordinate points.
- [x] Implement reusable working-coordinate and order-array setup for sweep passes.
- [x] Implement ordered active lookup for each sweep pass.
- [x] Implement the four directional sweep passes from `doc/detailed-design.md`.
- [x] Emit candidate edges only through the Edge Storage helper so weights use original coordinates.
- [x] Validate generated candidate edges against brute-force oracle on all golden and randomized small cases.
- [x] Add a regression note or update the detailed design if the transform schedule needs correction.

## Tests and Quality Gates

- [x] Run optimized-vs-oracle checks for deterministic seeds `20260608`, `20260609`, `1`, `2`, and `3` once a test command exists.
- [x] Check candidate count stays near linear on duplicate-heavy and collinear large workloads.

## Done When

- [x] Kruskal over generated candidates matches every golden expected output.
- [x] Randomized small cases match the brute-force oracle.
- [x] Production candidate generation does not build all pairwise edges.

## Notes

- 2026-06-20 implementation: the earlier `-wy`/Fenwick pseudocode missed required unit-square edges. The implemented sweep follows the cp-algorithms/KACTL active-map variant: sort by transformed `x + y`, scan active points by descending transformed `x`, break on `x - y`, then rotate through four transforms. `make test` covers all golden cases plus deterministic randomized oracle comparisons.
