# Detailed Design

## Purpose

This document turns the proposal, high-level design, and test plan into an implementation-ready design for the RMST solver. It specifies module responsibilities, internal interfaces, shared data contracts, the planned Manhattan MST candidate algorithm, failure behavior, and test coverage without writing production code.

The design target is a compact C++ implementation, likely one `main.cpp` plus the required `Makefile` and `readme.txt`, unless implementation complexity later justifies splitting files.

## Source Proposal Summary

The proposal requires a Linux C++ program for NYCU PDA Programming Assignment #4. The program reads one input file containing up to 10,000,000 integer 2D points, computes the total Rectilinear Minimum Spanning Tree weight under Manhattan distance, and writes only that total to an output file.

Key proposal constraints:

- Avoid O(n^2) complete-graph construction in production.
- Use signed 64-bit integers for distances and the final RMST total.
- Use compact point, edge, and disjoint-set storage.
- Generate a sparse Manhattan MST candidate edge set, then run Kruskal.
- Keep a brute-force baseline only for small development checks.
- Use only the C++ standard library unless the user approves otherwise.
- Provide source code, `Makefile`, and `readme.txt`.

## HLD Summary

The HLD defines a single-process batch pipeline:

```text
CLI and Orchestrator
  -> Input Reader
  -> Point Storage
  -> Manhattan MST Candidate Generator
  -> Edge Storage
  -> Kruskal MST Solver
  -> Output Writer
```

It also defines two support modules:

- `Small-Case Baseline Oracle` for local correctness checks.
- `Build and Submission Artifacts` for the assignment build and usage deliverables.

This detailed design preserves those module names and dependency directions.

## Design Goals

- Implement the assignment CLI: `[executable] [input file] [output file]`.
- Keep the normal execution path file-based and non-interactive.
- Compute the exact RMST total under Manhattan distance.
- Generate only sparse candidate edges for production inputs.
- Target `O(n log n)` behavior from sorting and sweep phases.
- Keep memory usage linear in the number of points and candidate edges.
- Avoid per-point heap allocation in the candidate-generator hot path.
- Make the algorithm testable against a brute-force oracle for small inputs.
- Keep the implementation standard-library-only.

## Non-Goals

- No full RMST edge-list output.
- No complete graph outside the small-case oracle.
- No external geometry or graph libraries.
- No CI, grader integration, or packaging automation beyond the required `Makefile` and `readme.txt`.
- No multi-test input format; the assignment defines one point set per input file.
- No detailed malformed-input UX beyond safe failure, because the assignment does not specify invalid input behavior.

## Architecture Overview

The implementation should use a small set of internal functions and structs inside one C++ translation unit unless splitting becomes necessary for readability.

Planned internal flow:

1. `main` checks command-line arguments.
2. `Input Reader` loads all points into `Point Storage`.
3. `Manhattan MST Candidate Generator` creates a linear-size candidate edge vector:
   - add zero-weight edges between exact duplicate consecutive points after coordinate sorting;
   - run four directional sweep passes over transformed coordinates;
   - each sweep uses a vector-backed active-key structure with coordinate compression and a Fenwick tree for "next active key" lookup.
4. `Kruskal MST Solver` sorts candidate edges and uses `Disjoint-Set Union` to accumulate the RMST total.
5. `Output Writer` writes one decimal integer to the output file.

The sweep design uses the standard ordered-active-set Manhattan MST sweep. The current implementation uses the reference `std::map` active set because it matched the brute-force oracle; replacing it with a vector-backed equivalent remains a performance optimization, not a different algorithm.

## Shared Data Contracts

| Data | Contract |
| --- | --- |
| Coordinate | Input coordinate is in `[-1e9, 1e9]`; store original coordinates in at least signed 32-bit, and transformed/summed values in signed 64-bit. |
| Point id | Use an unsigned integer type large enough for `n <= 10,000,000`; `uint32_t` is sufficient. Point ids remain stable after sorting and transformations. |
| Point | Original coordinate pair plus stable id. Duplicate coordinates remain valid separate vertices unless a later implementation explicitly deduplicates while preserving total. |
| Work coordinate | Signed 64-bit transformed `x` and `y` used by sweep passes. |
| Edge | Endpoint ids `u`, `v` and signed 64-bit weight `w = |x_u - x_v| + |y_u - y_v|`. |
| Candidate edge set | Linear-size edge vector; duplicate candidate edges are allowed because Kruskal ignores already-connected endpoints. |
| RMST total | Signed 64-bit integer. Maximum possible total is within `int64_t` for assignment limits. |
| Input file | First integer `n`, followed by exactly `n` coordinate pairs. Extra trailing tokens are unspecified. |
| Output file | One decimal integer, with optional final newline and no labels. |
| Failure | Return nonzero from the executable. Diagnostics may go to `stderr`; the output file must not contain a fake success value. |

## Module Designs

### CLI and Orchestrator

#### Responsibility

Own program entry, validate the assignment-style argument count, sequence the pipeline, and return the process exit code.

#### Non-Responsibility

- Does not parse point data itself.
- Does not compute RMST internals.
- Does not own algorithm data structures beyond top-level vectors and status flow.

#### Inputs and Outputs

- Input: `argc`, `argv`.
- Output: process exit code.
- Success path: calls output writer with the RMST total.
- Failure path: returns nonzero and may write diagnostics to `stderr`.

#### Public Interface

Proposed internal shape:

```text
int main(int argc, char** argv)
int run(const char* input_path, const char* output_path)
```

Exact function names may change, but the argument and ownership contract should stay the same.

#### Data Structures

- Input path string/pointer.
- Output path string/pointer.
- `vector<Point>` for parsed points.
- `vector<Edge>` for candidate edges.

#### Internal Design

- Require exactly three command-line tokens including executable name.
- Read all input before writing the final output.
- Do not print the answer to stdout as the primary result; the assignment requires output file writing.

#### Algorithm Details

```text
if argc != 3:
    print usage to stderr
    return failure
points = read_points(argv[1])
edges = generate_candidates(points)
total = kruskal(points.size, edges)
write_total(argv[2], total)
return success
```

#### Dependencies

- Input Reader
- Manhattan MST Candidate Generator
- Kruskal MST Solver
- Output Writer

#### Failure Handling

- Missing arguments: nonzero exit.
- Input read failure: nonzero exit.
- Solver failure such as disconnected candidate graph: nonzero exit. This should not occur if candidate generation is correct.
- Output write failure: nonzero exit.

#### Independent Test Plan

- Invoke with missing arguments and expect nonzero exit.
- Invoke with valid sample paths and expect output file creation.
- Verify no result is required from stdout.

#### Open Questions

- Should the executable be named exactly `RMST`? This design assumes yes for the `Makefile`, matching the PDF example.
- How much malformed-input diagnostics are desired?

### Input Reader

#### Responsibility

Read the assignment input format from a file and produce point storage.

#### Non-Responsibility

- Does not validate geometric correctness beyond bounds and count.
- Does not generate candidate edges.
- Does not write output.

#### Inputs and Outputs

- Input: input file path.
- Output: vector of points with stable ids.

#### Public Interface

Proposed internal shape:

```text
bool read_points(const char* path, vector<Point>& points, Error& error)
```

`Error` may be a simple string/status code. Exact representation can stay minimal.

#### Data Structures

- Buffered file reader.
- `Point { int32_t x; int32_t y; uint32_t id; }`.

#### Internal Design

- Use a large file buffer and integer parser over bytes, or standard C buffered I/O if it is fast enough in testing.
- Reserve `n` points after reading `n`.
- Assign ids from `0` to `n - 1` in input order.
- Store coordinates after checking they fit assignment bounds.

#### Algorithm Details

```text
open file
read n
if n not in [1, 10000000]: fail
reserve n points
for i in 0..n-1:
    read x, y
    if missing or out of bounds: fail
    append Point{x, y, i}
```

#### Dependencies

- None beyond standard file I/O and Point Storage data contract.

#### Failure Handling

- Unreadable file: fail.
- Missing `n`: fail.
- `n` outside assignment bounds: fail.
- Missing coordinate pair: fail.
- Coordinate outside assignment bounds: fail.
- Extra trailing tokens: unspecified by assignment; simplest behavior is to ignore them unless implementation chooses strict validation.

#### Independent Test Plan

- Valid sample input.
- Extra whitespace and blank lines.
- Negative coordinates.
- Coordinates at `-1e9` and `1e9`.
- Too few coordinate pairs.
- `n = 0` as malformed input.

#### Open Questions

- Should trailing tokens be rejected or ignored?

### Point Storage

#### Responsibility

Own original coordinates and stable point ids for all downstream modules.

#### Non-Responsibility

- Does not own transformed sweep state.
- Does not decide MST edges.
- Does not write files.

#### Inputs and Outputs

- Input: parsed coordinate pairs.
- Output: indexed point collection available to candidate generation, distance calculation, oracle checks, and Kruskal endpoint count.

#### Public Interface

Proposed data contract:

```text
Point.x, Point.y: original coordinates
Point.id: stable id equal to vector index unless implementation documents otherwise
points.size(): number of vertices
```

#### Data Structures

- `vector<Point> points`.
- Optional `vector<uint32_t> order` reused by sorting modules.

#### Internal Design

- Keep original coordinates immutable after input.
- Transformations should use separate working arrays so distance calculations always use original coordinates.
- Duplicate points remain represented unless implementation later chooses a proven total-preserving deduplication path.

#### Algorithm Details

No standalone algorithm. The main invariant is stable id preservation.

#### Dependencies

- Input Reader writes it.
- Candidate Generator and Kruskal Solver read it.
- Baseline Oracle reads it.

#### Failure Handling

- Allocation failure is a process-level failure.
- Point count overflow is prevented by the assignment limit and `uint32_t` ids.

#### Independent Test Plan

- Check ids remain stable after sorting order vectors.
- Check duplicate coordinates preserve distinct ids.
- Check coordinate extremes remain unchanged.

#### Open Questions

- Deduplication is not the default in this design. If memory pressure requires it, detailed handling must be updated and revalidated.

### Manhattan MST Candidate Generator

#### Responsibility

Generate a sparse candidate edge set that contains enough edges for Kruskal to recover the Manhattan MST.

#### Non-Responsibility

- Does not sort final edges for Kruskal by weight, except any local sort needed for sweeps.
- Does not compute the final MST total.
- Does not perform complete-graph generation.

#### Inputs and Outputs

- Input: original point vector.
- Output: vector of candidate edges.

#### Public Interface

Proposed internal shape:

```text
void generate_candidates(const vector<Point>& points, vector<Edge>& edges)
```

The function appends edges. The caller owns the output vector.

#### Data Structures

- `vector<int64_t> wx`, `wy`: current transformed coordinates by point id.
- `vector<uint32_t> order`: point ids sorted for each sweep pass.
- ordered active set keyed by transformed `x` for each pass.
- `vector<Edge>& edges`: shared candidate edge storage.

#### Internal Design

Use the standard Manhattan MST neighbor theorem: for each point, it is sufficient to consider nearest neighbors in the fixed Manhattan octants; the union of those nearest-neighbor edges contains an MST. The design implements this through four transformed directional sweeps, each producing candidate edges for a pair of symmetric octants.

Before the directional sweeps, add zero-weight edges for exact duplicate coordinates:

1. Sort ids by original `(x, y, id)`.
2. For each consecutive pair with identical `(x, y)`, append `Edge{prev_id, curr_id, 0}`.

This preserves separate duplicate vertices without requiring full deduplication.

For each sweep pass:

1. Sort `order` by `(wx[id] + wy[id], wx[id], wy[id], id)`.
2. Clear the ordered active set keyed by transformed `x` in descending order.
3. Visit points in sorted order.
4. Start from the active point with the largest transformed `x` that is still `<= wx[id]`.
5. While `wx[id] - wy[id] <= wx[other] - wy[other]`, append the Manhattan edge and erase that active point from the sweep state.
6. Insert or replace the current point at key `wx[id]`.
7. Apply the next coordinate transform and repeat until all four passes are complete.

#### Algorithm Details

Directional sweep pseudocode:

```text
initialize wx[id] = points[id].x, wy[id] = points[id].y
for pass in 0..3:
    order = all point ids
    sort order by (wx + wy, wx, wy, id)
    active.clear()

    for id in order:
        it = active.lower_bound(wx[id]) in descending-x order
        while it exists:
            other = it.id
            if wx[id] - wy[id] > wx[other] - wy[other]:
                break
            append edge using original coordinates of id and other
            erase it and advance
        active[wx[id]] = id

    transform working coordinates for next pass:
        pass 0: swap wx and wy
        pass 1: negate wx
        pass 2: swap wx and wy
        pass 3: done
```

This active-set form follows the standard Manhattan MST sweep described by cp-algorithms/KACTL and is validated by oracle and golden tests.

Complexity target:

- Four sorts of `n` ids: `O(n log n)`.
- Four ordered active sweeps: `O(n log n)`.
- Candidate edge count: expected linear, bounded by the duplicate prepass plus a small constant factor per point.

#### Dependencies

- Point Storage for original coordinates and ids.
- Edge Storage for appending candidate edges.

#### Failure Handling

- If there are zero or one points, append no positive candidate edges; Kruskal returns `0`.
- Allocation failure is a process-level failure.
- If candidate generation is incomplete, Kruskal may fail to connect all vertices; treat that as a solver failure and add the case to regression tests.

#### Independent Test Plan

- For each golden case, verify Kruskal over generated candidates matches expected output.
- For randomized small cases, compare optimized candidate result against brute-force oracle.
- Verify every emitted edge has valid endpoint ids and correct 64-bit Manhattan weight.
- Verify duplicate coordinates emit enough zero-weight edges to connect duplicates.
- Verify candidate count grows linearly on large duplicate and collinear workloads.

#### Open Questions

- A vector-backed active set can replace `std::map` later if large-input profiling shows allocation overhead matters.

### Edge Storage

#### Responsibility

Own candidate edges from the generator until Kruskal consumes them.

#### Non-Responsibility

- Does not decide whether an edge is accepted into the MST.
- Does not require global edge deduplication unless memory or tests show it is needed.

#### Inputs and Outputs

- Input: endpoint ids and computed weights.
- Output: sortable candidate edge vector.

#### Public Interface

Proposed internal shape:

```text
Edge { uint32_t u; uint32_t v; int64_t w; }
add_edge(points, u, v, edges)
```

`add_edge` computes weight from original point coordinates to prevent transformed-coordinate mistakes.

#### Data Structures

- `vector<Edge> edges`.

#### Internal Design

- Permit repeated edges and equal weights.
- Skip self-edges unless duplicate handling explicitly requires a separate representation; zero duplicate edges should still have different ids.
- Keep edge endpoints as point ids, not pointers or references.

#### Algorithm Details

```text
if u == v: skip
w = abs64(points[u].x - points[v].x) + abs64(points[u].y - points[v].y)
append Edge{u, v, w}
```

#### Dependencies

- Candidate Generator appends.
- Kruskal MST Solver sorts and reads.

#### Failure Handling

- Allocation failure is a process-level failure.
- Invalid endpoint ids are programming errors; tests should catch them.

#### Independent Test Plan

- Verify 64-bit two-point edge weight `4000000000`.
- Verify zero-weight duplicate edge.
- Verify endpoint validation on generated edges in debug/self-test mode if implemented.

#### Open Questions

- Whether to deduplicate identical candidate edges before Kruskal is left as a performance optimization, not a correctness requirement.

### Disjoint-Set Union

#### Responsibility

Track connected components for Kruskal and small-case oracle MST calculations.

#### Non-Responsibility

- Does not know coordinates or edge weights.
- Does not sort edges.
- Does not write results.

#### Inputs and Outputs

- Input: number of vertices; pairs of endpoint ids.
- Output: whether a union merged two different components.

#### Public Interface

Proposed internal shape:

```text
DSU(size_t n)
uint32_t find(uint32_t v)
bool unite(uint32_t a, uint32_t b)
```

#### Data Structures

- `vector<uint32_t> parent`.
- `vector<uint32_t> size` or `rank`.
- Optional component count.

#### Internal Design

- Use iterative path compression.
- Use union by size or rank.
- Initialize `parent[i] = i`.

#### Algorithm Details

Standard DSU:

```text
find(v):
    follow parents to root
    compress path
    return root

unite(a, b):
    ra = find(a), rb = find(b)
    if ra == rb: return false
    attach smaller tree under larger
    return true
```

#### Dependencies

- Kruskal MST Solver.
- Small-Case Baseline Oracle.

#### Failure Handling

- Endpoint out of range is a programming error; generated tests should validate endpoints before Kruskal.

#### Independent Test Plan

- Singleton initialization.
- Repeated union on same pair.
- Transitive connectivity.
- Tie-rank/size behavior.
- Large `n` initialization smoke check when memory allows.

#### Open Questions

- None identified.

### Kruskal MST Solver

#### Responsibility

Sort candidate edges by weight, accept component-connecting edges, and accumulate the total RMST weight.

#### Non-Responsibility

- Does not generate candidate edges.
- Does not compute edge weights from coordinates.
- Does not write output.

#### Inputs and Outputs

- Input: point count and candidate edge vector.
- Output: signed 64-bit MST total, plus success/failure status.

#### Public Interface

Proposed internal shape:

```text
bool compute_mst_total(size_t n, vector<Edge>& edges, int64_t& total, Error& error)
```

The function may sort `edges` in place.

#### Data Structures

- Candidate edge vector sorted by `(w, u, v)` for deterministic behavior.
- DSU instance.
- Accepted edge count.
- Signed 64-bit running total.

#### Internal Design

- If `n <= 1`, total is `0`.
- Sort candidate edges by weight.
- Iterate until `n - 1` edges are accepted.
- Add weights using signed 64-bit total.
- If the loop ends before accepting `n - 1` edges, report candidate-generation failure.

#### Algorithm Details

```text
if n <= 1:
    total = 0
    return success
sort edges by weight
dsu = DSU(n)
total = 0
accepted = 0
for edge in edges:
    if dsu.unite(edge.u, edge.v):
        total += edge.w
        accepted += 1
        if accepted == n - 1:
            return success
return failure: candidate graph disconnected
```

#### Dependencies

- Edge Storage.
- Disjoint-Set Union.

#### Failure Handling

- Disconnected candidate graph: nonzero solver failure and regression case.
- 64-bit overflow is not expected under assignment bounds; no saturation behavior is needed.

#### Independent Test Plan

- Known weighted candidate graph.
- `n = 1`.
- Duplicate zero-weight edges.
- Equal-weight tie cases.
- Early stop after `n - 1` accepted edges.
- 64-bit total case from golden tests.

#### Open Questions

- None identified.

### Output Writer

#### Responsibility

Write the final RMST total to the requested output file.

#### Non-Responsibility

- Does not compute the total.
- Does not decide success of earlier phases.
- Does not print diagnostics into the result file.

#### Inputs and Outputs

- Input: output file path and signed 64-bit total.
- Output: file containing one decimal integer and optional final newline.

#### Public Interface

Proposed internal shape:

```text
bool write_total(const char* path, int64_t total, Error& error)
```

#### Data Structures

- File output handle or stream.

#### Internal Design

- Open output file only after solver success, or overwrite only when writing the final known-good result.
- Write the total as decimal text.
- Do not include timing, labels, or debug text.

#### Algorithm Details

```text
open output path for writing
write total
write newline
close and check success
```

#### Dependencies

- CLI and Orchestrator.

#### Failure Handling

- Unwritable path or flush failure: nonzero exit.

#### Independent Test Plan

- Output file contains `13` for sample.
- Output file contains `4000000000` for 64-bit golden case.
- Output file has no extra tokens.
- Unwritable path behavior if easy to test locally.

#### Open Questions

- None identified.

### Small-Case Baseline Oracle

#### Responsibility

Provide a simple correctness oracle for tiny inputs by building all pairwise Manhattan edges and running MST.

#### Non-Responsibility

- Must not run for large production inputs.
- Must not be required by normal grading execution.
- Does not optimize memory or runtime.

#### Inputs and Outputs

- Input: small point vector.
- Output: exact MST total for comparison.

#### Public Interface

Proposed internal shape:

```text
bool brute_force_mst_total(const vector<Point>& points, int64_t& total)
```

This can be compiled into a local self-test path or a separate test helper, depending on implementation.

#### Data Structures

- Temporary complete edge vector for tiny `n`.
- DSU instance.

#### Internal Design

- Guard the oracle with a small maximum `n`, such as `n <= 80`.
- Build every pairwise edge.
- Run Kruskal with the same DSU implementation or a separate simple MST implementation.
- Keep it independent from optimized candidate generation.

#### Algorithm Details

```text
if n > ORACLE_LIMIT: fail oracle invocation
for every pair i < j:
    append Manhattan edge
run Kruskal on complete edge list
return total
```

#### Dependencies

- Point Storage.
- Disjoint-Set Union or equivalent local MST helper.

#### Failure Handling

- Refuse inputs larger than the oracle limit.

#### Independent Test Plan

- Hand-solvable golden cases.
- Random small inputs.
- Duplicate-heavy small grid.
- Verify oracle refuses large `n`.

#### Open Questions

- Should the oracle be exposed through `make test`, a self-test executable flag, or only used in local development code?

### Build and Submission Artifacts

#### Responsibility

Provide the assignment-required build and usage files.

#### Non-Responsibility

- Does not implement solver logic.
- Does not perform final E3 upload.

#### Inputs and Outputs

- Input: source file(s).
- Output: executable, `readme.txt`, and eventual submission-ready root files.

#### Public Interface

Planned files:

```text
Makefile
main.cpp
readme.txt
```

Assumed build target:

```text
make
./RMST input.dat output.dat
```

#### Data Structures

None.

#### Internal Design

- `Makefile` should build one executable named `RMST` unless the user changes the name.
- Use Linux-compatible C++ compiler settings.
- This design assumes `g++` and C++17 as practical defaults, but exact compiler and standard remain open.
- `readme.txt` should only explain how to build and run the program.

#### Algorithm Details

Not applicable.

#### Dependencies

- Source file names chosen during implementation.
- Assignment CLI and output contracts.

#### Failure Handling

- Build failure blocks submission.
- Missing `readme.txt` or `Makefile` blocks submission readiness.

#### Independent Test Plan

- `make` succeeds once `Makefile` exists.
- Executable exists after build.
- `readme.txt` includes build command and `RMST input.dat output.dat` style usage.
- Required files are in the repository root unless a later convention changes this.

#### Open Questions

- Confirm executable name.
- Confirm compiler and C++ standard.
- Confirm student ID for final packaging.

## Cross-Module Contracts

| Contract | Producer | Consumer | Verification |
| --- | --- | --- | --- |
| Valid point vector with stable ids | Input Reader | Point Storage, Candidate Generator, Oracle | Parser unit tests and id-stability tests. |
| Original coordinates immutable | Point Storage | Edge Storage, Oracle | Golden 64-bit distance and randomized tests. |
| Candidate edges have valid ids and 64-bit weights | Candidate Generator, Edge Storage | Kruskal Solver | Endpoint validation and oracle comparison. |
| Candidate graph connects all vertices | Candidate Generator | Kruskal Solver | Kruskal accepted edge count and randomized oracle tests. |
| DSU merge status is correct | Disjoint-Set Union | Kruskal Solver, Oracle | DSU unit tests and known graph tests. |
| Output contains only one integer | Kruskal Solver, Output Writer | Grader/user | Smoke, golden, and output-format tests. |
| Build command exists | Build Artifacts | User/grader | Quality gate after `Makefile` exists. |

## End-to-End Workflow

1. User runs `./RMST input.dat output.dat`.
2. CLI validates exactly two file path arguments.
3. Input Reader parses `n` and all points.
4. Point Storage provides immutable original coordinates and stable ids.
5. Candidate Generator appends duplicate zero edges and directional sweep edges.
6. Kruskal sorts candidate edges and uses DSU to accumulate total.
7. Output Writer writes the total to `output.dat`.
8. Process returns success.

Failure workflow:

- On argument, input, allocation, solver, or output failure, return nonzero.
- Do not write a valid-looking incorrect RMST total.

## Test Strategy Mapping

| Test-Plan Requirement | Coverage Location |
| --- | --- |
| Build through repository command | Build and Submission Artifacts; Quality Gates. |
| Assignment CLI | CLI and Orchestrator unit tests; end-to-end smoke tests. |
| Input parser | Input Reader unit tests; integration tests with sample/golden files. |
| Point id and coordinate preservation | Point Storage unit tests; randomized oracle tests. |
| Manhattan distance 64-bit arithmetic | Edge Storage unit tests; two-distant-points golden case. |
| Candidate generation correctness | Candidate Generator unit checks plus optimized-vs-oracle randomized tests. |
| DSU behavior | DSU unit tests and Kruskal known-graph tests. |
| Kruskal MST correctness | Kruskal unit tests, golden cases, oracle differential tests. |
| Output-only result file | Output Writer tests and end-to-end output-format checks. |
| PDF sample `13` | Smoke test and regression test. |
| Golden cases | End-to-end CLI tests. |
| Randomized/property tests | Baseline Oracle plus optimized solver comparison. |
| Edge cases | Module tests and end-to-end golden suite. |
| Performance benchmarks | Candidate Generator and Kruskal scaling checks; quality gates when commands exist. |
| No complete graph in production | Manual verification and performance/memory trend checks. |
| `readme.txt` content | Build and Submission Artifacts manual check. |

## Quality Gates

Current repository quality gates are missing because no source, `Makefile`, tests, or evaluator exist yet.

Required future gates:

- Build command: `make` or the exact command documented by the future `Makefile`.
- Sample smoke: run the PDF sample and verify output `13`.
- Golden CLI suite: run every golden case from `doc/test-plan.md`.
- Oracle differential suite: run deterministic small randomized cases.
- Output-format check: ensure only one integer is written.
- Manual source review: confirm no production complete graph construction.
- Performance check: run at least one medium or large structured case, or mark blocked by hardware constraints.

Do not report any gate as verified until it has been run.

## Risks and Mitigations

| Risk | Mitigation |
| --- | --- |
| Sweep transform bug misses required candidate edges | Keep transform schedule isolated, compare heavily against brute-force oracle, and promote failures to regression tests. |
| Duplicate points break candidate generation | Add explicit zero-edge duplicate prepass before directional sweeps. |
| Memory pressure from edge storage | Keep edge count linear, skip full dedup initially, and add dedup only if measurements show it is needed. |
| Per-point heap allocations slow large inputs | Use vector-backed active sweep with Fenwick tree instead of `std::map` in the hot path. |
| 32-bit overflow | Use signed 64-bit for transformed coordinates, edge weights, and totals; include 64-bit golden tests. |
| Unknown grader compiler | Use conservative standard-library C++ and document compiler assumptions in `Makefile`/`readme.txt`. |
| Oracle accidentally used on large inputs | Guard oracle with a hard small `n` limit and keep it out of the normal CLI path. |

## Assumptions

- The executable will be named `RMST` unless the user says otherwise.
- The implementation will use C++ and the C++ standard library only.
- `g++` with C++17 is an acceptable default until compiler requirements are clarified.
- Assignment artifacts stay in the repository root.
- Invalid input can fail with nonzero exit and `stderr` diagnostics.
- Duplicate input points remain separate vertices, connected by zero-weight edges; this avoids relying on deduplication policy.
- A single `main.cpp` is acceptable if the code remains readable.

## Open Questions

- Confirm executable name: `RMST` or another name?
- Confirm compiler and C++ standard for `Makefile`.
- Is there a memory limit beyond the 1-hour runtime limit?
- Are third-party libraries disallowed by policy, or merely unnecessary?
- Should malformed input be rejected strictly, or is grading-only best effort enough?
- Should trailing tokens after the expected `n` coordinate pairs be ignored or rejected?
- Should tests be exposed through `make test`, a self-test mode, or kept as local helper commands?
- What student ID should be used for final packaging checks?
