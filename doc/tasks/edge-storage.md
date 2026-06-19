# Edge Storage

## Goal

Provide compact candidate edge records with valid endpoint ids and signed 64-bit Manhattan weights.

## Inputs

- `doc/proposal.md`: compact edge storage and signed 64-bit distance requirements.
- `doc/high-level-design.md`: Edge Storage receives generated candidates and feeds Kruskal.
- `doc/detailed-design.md`: `Edge { uint32_t u; uint32_t v; int64_t w; }` and `add_edge(points, u, v, edges)` contract.
- `doc/test-plan.md`: endpoint validation, duplicate edge tolerance, and 64-bit golden case.

## Write Scope

`main.cpp` edge struct, edge vector use, and edge creation helper. No global edge database.

## Read Scope

Point Storage, Candidate Generator, Kruskal MST Solver, and golden test cases.

## Dependencies

Point Storage for original coordinates. Candidate Generator appends edges. Kruskal MST Solver sorts and consumes edges.

## Tasks

- [ ] Add the `Edge` representation with two `uint32_t` endpoints and signed 64-bit weight.
- [ ] Add an edge helper that skips self-edges and computes weights from original point coordinates.
- [ ] Use signed 64-bit arithmetic for coordinate differences and total edge weight.
- [ ] Allow duplicate candidate edges and equal weights without changing MST correctness.
- [ ] Add optional debug/self-test validation for endpoint ranges if a self-test path is created.

## Tests and Quality Gates

- [ ] Verify the two-point coordinate-extreme golden case outputs `4000000000`.
- [ ] Verify duplicate coordinates can produce zero-weight edges between distinct ids.

## Done When

- [ ] Every generated edge has valid endpoints and correct Manhattan weight.
- [ ] 64-bit edge-weight checks from `doc/test-plan.md` pass once tests exist.
