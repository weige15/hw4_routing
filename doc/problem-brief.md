# Problem Brief

## Source Documents

- `../p4_routing.pdf` - read. NYCU Physical Design Automation Spring 2026 Programming Assignment #4, dated June 8, 2026.
- User message - read. Requested `$problem-brief-generator p4_routing.pdf`.

## Assignment Objective

Compute the total weight of the Rectilinear Minimum Spanning Tree (RMST) for a set of 2D points under Manhattan distance. The program must avoid complete-graph construction and scale to very large inputs.

## Required Inputs

- Input file path is passed as the second command-line argument: `[executable file name] [input file name] [output file name]`.
- The input file format:
  - First line: integer `n`.
  - Next `n` lines: integer coordinates `xi yi` for each point.
- Constraints from the PDF:
  - `1 <= n <= 10,000,000`.
  - `-10^9 <= xi, yi <= 10^9`.
  - Multiple points may share identical coordinates.

## Required Outputs

- Output file path is passed as the third command-line argument.
- Output a single integer: the total weight of the RMST.
- The PDF states the program should output only the total RMST weight.

## Constraints

- Manhattan distance between points `pi = (xi, yi)` and `pj = (xj, yj)` is `|xi - xj| + |yi - yj|`.
- O(n^2) algorithms are not acceptable.
- The implementation must avoid constructing the complete graph.
- All distance calculations and the final RMST weight must use signed 64-bit integers.
- The implementation should emphasize scalability, memory efficiency, and robustness.
- Each test case fails if the program takes more than 1 hour.

## Evaluation or Grading Criteria

- Graded based on correctness and running time.
- A test case is failed if runtime exceeds 1 hour.
- No specific evaluator command, memory limit, hidden test format, or scoring weights beyond correctness and running time are provided.

## Required Deliverables

- A tar file submitted to E3.
- The tar should contain a folder named with the student ID.
- Required contents inside the folder:
  - Source code.
  - `Makefile`.
  - `readme.txt` explaining how to build and use the program.
- The PDF gives the packaging command: `tar cvf Student_ID.tar Student_ID`.

## Relevant Methods From Papers

None found in the provided sources.

The assignment mentions scalable geometric approaches as expected examples: sweep-line methods, divide-and-conquer techniques, geometric transformations combined with Kruskal's algorithm, or other provably correct approaches for large-scale RMST construction.

## Data, Benchmarks, or Test Cases

- Sample input from the PDF:

```text
5
0 0
2 0
2 3
5 1
6 4
```

- Sample output from the PDF:

```text
13
```

- No benchmark files or hidden test descriptions are provided.

## Implementation Environment

- Platform: Linux.
- Preferred language: C or C++.
- Command-line usage example from the PDF: `RMST input.dat output.dat`.
- Exact compiler, C/C++ standard, optimization flags, and memory limit are not provided.

## Confirmed Facts

- The task is RMST total weight under Manhattan distance.
- The input can contain up to 10,000,000 points.
- Coordinates are integer values in `[-10^9, 10^9]`.
- Duplicate coordinates are allowed.
- Distances and final answer must use signed 64-bit integers.
- The output should contain only the total RMST weight.
- Complete graph construction and O(n^2) algorithms are not acceptable.
- The program must accept input and output file paths from command-line arguments.
- Submission requires source code, `Makefile`, and `readme.txt`.

## Assumptions

- The repository root will hold the assignment source files unless the user later chooses a different layout.
- The implementation will use C or C++ because the PDF says C or C++ is preferred.
- The sample input spacing in the PDF text extraction is interpreted as coordinate pairs such as `0 0`, `2 0`, `2 3`, `5 1`, and `6 4`.

## Open Questions

- What student ID should be used for the final folder and tar file names?
- What exact compiler and C++ standard should the `Makefile` use?
- Is there a memory limit in addition to the 1-hour runtime limit?
- Are there constraints on external libraries, or should the solution use only the C/C++ standard library?
- How should malformed input or missing command-line arguments be handled for grading?
- Are duplicate points expected to be treated as separate vertices connected by zero-weight edges, or can exact duplicate coordinates be deduplicated if the RMST total is unchanged?

## Notes for Proposal Generation

- Start proposal work from the confirmed RMST requirements and the open questions above.
- Do not propose an O(n^2) complete-graph approach.
- Preserve the command-line contract, output-only result requirement, 64-bit arithmetic requirement, and required deliverables.
- The proposal should choose and justify a scalable geometric algorithm, but this brief intentionally does not select one.
