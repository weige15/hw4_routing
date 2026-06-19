# Point Storage

## Goal

Define and maintain the in-memory point representation used by input parsing, candidate generation, Kruskal, and oracle checks.

## Inputs

- `doc/proposal.md`: compact point storage and 64-bit distance requirements.
- `doc/high-level-design.md`: Point Storage owns coordinates and stable point identifiers.
- `doc/detailed-design.md`: `Point { int32_t x; int32_t y; uint32_t id; }` contract and immutable original coordinates.
- `doc/test-plan.md`: id preservation, duplicate vertices, and coordinate-extreme checks.

## Write Scope

`main.cpp` point struct, point vector ownership, and any reusable order vectors. No standalone storage framework.

## Read Scope

`doc/detailed-design.md`, Input Reader implementation, Candidate Generator implementation, Oracle implementation.

## Dependencies

Input Reader creates points. Candidate Generator, Edge Storage, Kruskal MST Solver, and Small-Case Baseline Oracle consume points.

## Tasks

- [x] Add the `Point` representation with signed original coordinates and stable `uint32_t` id.
- [x] Keep original point coordinates immutable after parsing.
- [x] Ensure sorting/sweep code uses ids or order arrays instead of mutating point identity.
- [x] Preserve duplicate coordinates as separate vertices.
- [x] Confirm point count and id type cover `n <= 10,000,000`.

## Tests and Quality Gates

- [x] Check duplicate coordinate inputs preserve distinct ids.
- [x] Check sorted and reverse-sorted inputs produce the same RMST total as shuffled inputs.

## Done When

- [x] All downstream modules can address points by stable id.
- [x] Coordinate extremes and duplicate points are preserved through the pipeline.
- [x] Point-storage checks from `doc/test-plan.md` pass once tests exist.

## Notes

- 2026-06-20 boundary cycle: parser assigns `uint32_t` ids from `0..n-1`; duplicate-coordinate input reached the solver placeholder instead of parser rejection. Sorting/sweep behavior is still pending candidate generation.
- 2026-06-20 final solver cycle: candidate generation keeps immutable coordinates in `points` and sorts separate id order arrays. Golden CLI and randomized oracle checks cover coordinate extremes and duplicates; explicit sorted, reversed, and shuffled collinear CLI inputs all returned `6`.
