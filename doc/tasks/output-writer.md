# Output Writer

## Goal

Write the final signed 64-bit RMST total to the requested output file with no extra result text.

## Inputs

- `doc/proposal.md`: output file must contain only the RMST total weight.
- `doc/high-level-design.md`: Output Writer serializes the Kruskal result.
- `doc/detailed-design.md`: `write_total(path, total, error)` contract.
- `doc/test-plan.md`: output-only format checks and sample/64-bit output checks.

## Write Scope

`main.cpp` output writing helper and any minimal error reporting in orchestration.

## Read Scope

CLI and Orchestrator, Kruskal MST Solver, output-format tests, and `readme.txt` usage text if needed.

## Dependencies

Kruskal MST Solver supplies the final total. CLI and Orchestrator supplies the output path.

## Tasks

- [x] Add output file opening and checked write/close behavior.
- [x] Write the total as one decimal integer with optional final newline.
- [x] Keep diagnostics out of the output file.
- [x] Return failure on unwritable output paths or flush errors.
- [x] Ensure the output file is written only after solver success.

## Tests and Quality Gates

- [x] Verify sample output file contains `13` only.
- [x] Verify 64-bit output file contains `4000000000` only for the coordinate-extreme golden case.

## Done When

- [x] Successful runs create the requested output file with one integer.
- [x] Failed runs do not create a valid-looking incorrect result.

## Notes

- 2026-06-20 boundary cycle: single-point input wrote exactly `0`; invalid parser inputs and the multi-point solver placeholder did not create result files; directory output path returned code 4.
- 2026-06-20 final solver cycle: CLI sample wrote `13`; coordinate-extreme case wrote `4000000000`.
