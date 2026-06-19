# Kruskal MST Solver

## Goal

Compute the RMST total by sorting candidate edges and accepting component-connecting edges with DSU.

## Inputs

- `doc/proposal.md`: Kruskal over generated candidates with signed 64-bit accumulation.
- `doc/high-level-design.md`: Kruskal consumes Edge Storage and determines the output integer.
- `doc/detailed-design.md`: `compute_mst_total(n, edges, total, error)` contract and disconnected-candidate failure.
- `doc/test-plan.md`: known graph tests, `n = 1`, duplicate zero edges, ties, early stop, and golden cases.

## Write Scope

`main.cpp` MST solver function and edge sorting comparator. May reuse DSU from `disjoint-set-union.md`.

## Read Scope

Edge Storage, Disjoint-Set Union, Candidate Generator, Small-Case Baseline Oracle, and golden cases.

## Dependencies

Edge Storage and Disjoint-Set Union. Candidate Generator must supply a connected candidate graph for valid inputs.

## Tasks

- [x] Implement `n <= 1` handling with total `0`.
- [x] Sort candidate edges by weight and deterministic endpoint tie-breakers.
- [x] Use DSU to accept only edges that connect different components.
- [x] Accumulate accepted edge weights in signed 64-bit total.
- [x] Stop after `n - 1` accepted edges.
- [x] Report failure if candidates do not connect all vertices.

## Tests and Quality Gates

- [x] Verify all golden test cases through the final CLI once available.
- [x] Verify known weighted candidate graphs and tie-heavy cases through unit or self-test checks.

## Done When

- [x] Kruskal returns exact totals for golden cases and oracle-generated small cases.
- [x] Disconnected candidate sets fail instead of producing an incomplete total.

## Notes

- 2026-06-20 core MST cycle: `make test` covers a connected known graph with MST total `6` and a disconnected candidate graph that fails.
- 2026-06-20 final solver cycle: `make test` covers optimized-vs-oracle randomized cases and all golden cases; CLI golden suite also passed after rebuilding `RMST`.
