# Disjoint-Set Union

## Goal

Implement the component-tracking structure used by Kruskal and the small-case oracle.

## Inputs

- `doc/proposal.md`: iterative disjoint-set with path compression and union by size/rank.
- `doc/high-level-design.md`: DSU tracks connected components during Kruskal.
- `doc/detailed-design.md`: `DSU(n)`, `find(v)`, and `unite(a, b)` interface.
- `doc/test-plan.md`: singleton, repeated union, transitive connectivity, and tie behavior checks.

## Write Scope

`main.cpp` DSU class or struct. No standalone library unless implementation later splits files.

## Read Scope

Kruskal MST Solver, Small-Case Baseline Oracle, and DSU-related tests.

## Dependencies

None for implementation. Kruskal MST Solver and Small-Case Baseline Oracle depend on this module.

## Tasks

- [x] Add DSU initialization for `n` vertices with parent and size/rank vectors.
- [x] Implement iterative `find` with path compression.
- [x] Implement `unite` that returns whether a merge occurred.
- [x] Use union by size or rank.
- [x] Keep endpoint ids as `uint32_t` and avoid recursion.

## Tests and Quality Gates

- [x] Check singleton components, repeated union on the same pair, transitive connectivity, and large initialization when memory allows.

## Done When

- [x] DSU correctly reports merges and already-connected endpoints.
- [x] DSU checks from `doc/test-plan.md` pass once tests exist.

## Notes

- 2026-06-20 core MST cycle: `make test` covers singleton root, first merge, repeated merge rejection, transitive merge, and path-compressed connectivity.
