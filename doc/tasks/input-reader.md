# Input Reader

## Goal

Implement fast file input for the assignment format and produce stable point records for downstream modules.

## Inputs

- `doc/proposal.md`: input format, `n <= 10,000,000`, coordinate bounds, fast file input.
- `doc/high-level-design.md`: Input Reader produces Point Storage and fails on unreadable input.
- `doc/detailed-design.md`: `read_points(path, points, error)` contract and validation rules.
- `doc/test-plan.md`: parser tests for whitespace, negative coordinates, bounds, incomplete files, and malformed `n`.

## Write Scope

`main.cpp` input parsing code and any small internal parser helper structs. No external parser dependencies.

## Read Scope

`doc/problem-brief.md`, `doc/detailed-design.md`, future `main.cpp`, and parser-related test fixtures if added.

## Dependencies

Point Storage data contract. CLI and Orchestrator supplies the input path.

## Tasks

- [x] Implement buffered integer reading from a file path.
- [x] Parse `n` and reject values outside `1..10000000`.
- [x] Parse exactly `n` coordinate pairs into points with ids `0..n-1`.
- [x] Validate coordinates against `[-1000000000, 1000000000]`.
- [x] Preserve duplicate coordinates as distinct points.
- [x] Fail safely on unreadable files, missing `n`, missing coordinate pairs, and out-of-range values.

## Tests and Quality Gates

- [x] Check parser behavior on the PDF sample, extra whitespace, negative coordinates, coordinate extremes, duplicate points, too few lines, and `n = 0`.

## Done When

- [x] Valid assignment inputs produce a complete point vector with stable ids.
- [x] Invalid required parser cases fail without crashing or writing an incorrect result.
- [x] Parser checks listed in `doc/test-plan.md` pass once tests exist.

## Notes

- 2026-06-20 boundary cycle: parser accepted the PDF sample up to the solver placeholder, accepted duplicate coordinates, accepted whitespace and coordinate extremes, and rejected `n = 0`, too few coordinate pairs, and out-of-range coordinates.
