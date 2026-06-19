# Proposal: Very Large-Scale Rectilinear MST Solver

## Objective

Implement a Linux C++ program for NYCU PDA Programming Assignment #4 that reads points from an input file, computes the total Rectilinear Minimum Spanning Tree (RMST) weight under Manhattan distance, and writes only that total to an output file.

## Source Inputs

- `../p4_routing.pdf`: assignment specification.
- `doc/problem-brief.md`: extracted assignment facts, constraints, sample case, assumptions, and open questions.
- `doc/repo-map.md`: current repository state.
- `doc/quality-gates.md`: current quality-gate inventory.

## Current Project State

- The repository is documentation-only.
- Existing files: `AGENTS.md`, `doc/problem-brief.md`, `doc/repo-map.md`, and `doc/quality-gates.md`.
- Missing required assignment artifacts: source code, `Makefile`, and `readme.txt`.
- No tests, scripts, CI, evaluator, or dependency metadata exist yet.

## Problem Summary

Given up to 10,000,000 integer 2D points, compute the total MST weight where edge cost is Manhattan distance. The implementation must avoid O(n^2) complete-graph construction and must use signed 64-bit integers for distances and the final answer.

## Constraints

- Input format: first line `n`, followed by `n` lines of `x y` coordinates.
- `1 <= n <= 10,000,000`.
- `-10^9 <= x, y <= 10^9`.
- Duplicate coordinates are allowed.
- CLI contract: `[executable] [input file] [output file]`.
- Output file must contain only the RMST total weight.
- Runtime limit: each grading test fails above 1 hour.
- Platform: Linux.
- Preferred language: C or C++.

## Proposed Approach

Build a single C++ solver with:

- Fast file input and output.
- Compact point, edge, and disjoint-set storage.
- A baseline MST path for small tests only, used as a correctness oracle during development.
- An optimized Manhattan MST candidate-edge generator followed by Kruskal's algorithm.
- A required `Makefile`, `readme.txt`, and one sample smoke-check path.

Use only the C++ standard library unless the user later approves external dependencies.

## Algorithm Strategy

Baseline method:

- For small `n`, build all pairwise Manhattan edges and run Kruskal.
- Use this only for local correctness tests and randomized brute-force comparisons.
- Do not use it for large assignment inputs.

Intended optimized method:

- Generate a sparse candidate edge set using standard Manhattan MST geometric transformations and directional sweeps.
- For each transformed direction, sort points and maintain a sweep structure keyed by transformed coordinates to find nearest candidate neighbors.
- Add only candidate edges that can participate in the Manhattan MST.
- Run Kruskal over the generated candidates with a disjoint-set union structure.
- Accumulate the MST total in signed 64-bit integers.

Correctness strategy:

- Compare optimized results against the baseline on many small deterministic and randomized cases.
- Include edge cases: `n = 1`, duplicate points, negative coordinates, same `x`, same `y`, and the PDF sample.
- Preserve the MST invariant by using Kruskal on candidate edges produced by the Manhattan MST neighbor theorem.

Performance strategy:

- Avoid complete graph construction.
- Target `O(n log n)`-style sorting and sweep phases plus Kruskal over a linear-size candidate set.
- Store points and candidate edges in contiguous vectors.
- Use iterative disjoint-set operations with path compression and union by size/rank.
- Avoid per-point heap allocations in hot paths.

## Alternatives Considered

- Complete graph plus Kruskal: simple and useful as a small-case oracle, but forbidden for production because it is O(n^2).
- Prim over implicit Manhattan distances: still requires too many distance checks without additional geometry.
- Divide-and-conquer RMST: valid in principle, but more complex to implement and validate under assignment pressure than the transformation-and-sweep candidate-edge approach.
- External computational geometry libraries: unnecessary unless later approved; the repo currently has no dependency policy or package metadata.

## Module Candidates

- `main.cpp`: CLI parsing, file I/O, solver orchestration, and final output.
- `Makefile`: build the required executable.
- `readme.txt`: build and usage instructions for submission.
- Optional `tests/` or `sample/`: small local inputs and expected outputs, if approved during implementation.

## Milestones

1. Add C++ project skeleton: `main.cpp`, `Makefile`, and `readme.txt`.
2. Implement input/output handling and signed 64-bit distance helpers.
3. Implement disjoint-set union and small-case baseline MST.
4. Implement optimized Manhattan MST candidate generation.
5. Add Kruskal integration over generated candidates.
6. Add sample smoke check and small randomized baseline comparisons.
7. Tune memory layout and remove debug-only paths from normal execution if needed.
8. Finalize `readme.txt` and submission packaging instructions.

## Validation Plan

- Build using the repository-defined `Makefile` once it exists.
- Run the PDF sample:

```text
input:
5
0 0
2 0
2 3
5 1
6 4

expected output:
13
```

- Run baseline-vs-optimized comparisons for small `n`.
- Run edge-case tests for duplicate points, one point, negative coordinates, collinear points, and repeated coordinate values.
- Check that output files contain only one integer.
- Check that no complete graph is built in the production path.

## Risks and Tradeoffs

- The optimized candidate generator is the main correctness risk; it needs careful validation against brute force on small cases.
- Memory pressure is a risk at 10,000,000 points; candidate edge storage must stay close to linear.
- Sorting large arrays dominates runtime; implementation should minimize extra copies.
- Duplicate points may require special handling to preserve zero-distance connectivity without bloating candidate edges.
- The PDF does not state a memory limit, compiler version, or external-library policy.

## Assumptions

- C++ is acceptable because the PDF says C or C++ is preferred.
- The implementation should use only the C++ standard library until the user approves otherwise.
- The executable name can be chosen in the `Makefile` unless the user provides a required name.
- Duplicate coordinates can be handled in a way that preserves the same RMST total, including zero-weight connectivity.
- Local tests may include helper data or scripts if approved in a later implementation step.

## Open Questions

- What student ID should be used for final folder and tar names?
- Should the executable be named exactly `RMST`?
- What C++ standard and compiler should the `Makefile` target?
- Is there a memory limit beyond the 1-hour runtime limit?
- Are third-party libraries disallowed, or merely unnecessary?
- How strict should error handling be for malformed input or missing command-line arguments?
