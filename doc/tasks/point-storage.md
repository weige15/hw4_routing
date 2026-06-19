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

- [ ] Add the `Point` representation with signed original coordinates and stable `uint32_t` id.
- [ ] Keep original point coordinates immutable after parsing.
- [ ] Ensure sorting/sweep code uses ids or order arrays instead of mutating point identity.
- [ ] Preserve duplicate coordinates as separate vertices.
- [ ] Confirm point count and id type cover `n <= 10,000,000`.

## Tests and Quality Gates

- [ ] Check duplicate coordinate inputs preserve distinct ids.
- [ ] Check sorted and reverse-sorted inputs produce the same RMST total as shuffled inputs.

## Done When

- [ ] All downstream modules can address points by stable id.
- [ ] Coordinate extremes and duplicate points are preserved through the pipeline.
- [ ] Point-storage checks from `doc/test-plan.md` pass once tests exist.
