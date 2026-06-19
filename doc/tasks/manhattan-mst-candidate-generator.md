# Manhattan MST Candidate Generator

## Goal

Generate a sparse candidate edge set that is sufficient for Kruskal to recover the exact Manhattan RMST without constructing the complete graph.

## Inputs

- `doc/proposal.md`: optimized Manhattan MST candidate-edge generator using geometric transformations and directional sweeps.
- `doc/high-level-design.md`: Candidate Generator reads Point Storage and writes Edge Storage.
- `doc/detailed-design.md`: duplicate zero-edge prepass, four directional sweep passes, compressed active keys, and Fenwick-backed active sweep.
- `doc/test-plan.md`: optimized-vs-brute-force oracle comparisons, golden cases, edge cases, and linear candidate-count checks.

## Write Scope

`main.cpp` candidate generation functions, working coordinate arrays, ordering arrays, compressed key handling, Fenwick/active sweep helper, and edge emission calls.

## Read Scope

`doc/detailed-design.md`, Point Storage, Edge Storage, Kruskal MST Solver, Small-Case Baseline Oracle, and randomized/golden test cases.

## Dependencies

Point Storage, Edge Storage, Kruskal MST Solver for validation, and Small-Case Baseline Oracle for correctness comparison.

## Tasks

- [ ] Add duplicate-coordinate prepass that emits zero-weight edges between consecutive identical coordinate points.
- [ ] Implement reusable working-coordinate and order-array setup for sweep passes.
- [ ] Implement compressed active keys and Fenwick-backed lookup for first active key at or after a position.
- [ ] Implement the four directional sweep passes from `doc/detailed-design.md`.
- [ ] Emit candidate edges only through the Edge Storage helper so weights use original coordinates.
- [ ] Validate generated candidate edges against brute-force oracle on all golden and randomized small cases.
- [ ] Add a regression note or update the detailed design if the transform schedule needs correction.

## Tests and Quality Gates

- [ ] Run optimized-vs-oracle checks for deterministic seeds `20260608`, `20260609`, `1`, `2`, and `3` once a test command exists.
- [ ] Check candidate count stays near linear on duplicate-heavy and collinear large workloads.

## Done When

- [ ] Kruskal over generated candidates matches every golden expected output.
- [ ] Randomized small cases match the brute-force oracle.
- [ ] Production candidate generation does not build all pairwise edges.
