# CLI and Orchestrator

## Goal

Implement the assignment entry point that accepts input and output file paths, sequences the solver pipeline, and returns a correct process status.

## Inputs

- `doc/proposal.md`: CLI contract `[executable] [input file] [output file]`, output-only result, C++ solver.
- `doc/high-level-design.md`: CLI owns lifecycle order and calls reader, solver, and writer.
- `doc/detailed-design.md`: `main(argc, argv)` and `run(input_path, output_path)` shape with safe failure behavior.
- `doc/test-plan.md`: missing-arguments check, end-to-end sample, output-file contract.

## Write Scope

`main.cpp` entry point and orchestration functions. May edit `readme.txt` usage examples after the executable name is finalized.

## Read Scope

`doc/detailed-design.md`, `doc/test-plan.md`, future `main.cpp`, future `Makefile`, and assignment CLI examples.

## Dependencies

Input Reader, Manhattan MST Candidate Generator, Kruskal MST Solver, Output Writer, and the executable-name decision from Build and Submission Artifacts.

## Tasks

- [ ] Add `main(int argc, char** argv)` with exact two-path argument validation.
- [ ] Add a `run(input_path, output_path)` style function that owns top-level pipeline sequencing.
- [ ] Wire reader, candidate generation, Kruskal, and output writer through explicit success/failure status.
- [ ] Route diagnostics to `stderr` only; never write diagnostics into the output file.
- [ ] Return nonzero for missing arguments, read failure, solver failure, or output failure.
- [ ] Verify the valid sample path creates the requested output file.

## Tests and Quality Gates

- [ ] Run the future CLI missing-arguments check and confirm nonzero exit.
- [ ] Run the PDF sample through the final executable and confirm output file contains `13` only.

## Done When

- [ ] The executable accepts `[executable] [input file] [output file]`.
- [ ] Top-level failures return nonzero without producing a fake RMST total.
- [ ] CLI smoke checks from `doc/test-plan.md` pass once build/test commands exist.
