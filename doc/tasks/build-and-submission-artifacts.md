# Build and Submission Artifacts

## Goal

Provide the assignment-required build and usage artifacts without adding unnecessary project infrastructure.

## Inputs

- `doc/proposal.md`: required source code, `Makefile`, `readme.txt`, and sample smoke-check path.
- `doc/high-level-design.md`: Build artifacts provide the executable and usage instructions.
- `doc/detailed-design.md`: planned files `main.cpp`, `Makefile`, `readme.txt`, assumed executable `RMST`, and C++17/g++ default.
- `doc/test-plan.md`: build gate, smoke test, `readme.txt` manual verification, and unknown evaluator commands.

## Write Scope

Repository root `Makefile`, `main.cpp`, `readme.txt`, and optional minimal sample/test files if implementation chooses to add them.

## Read Scope

Assignment PDF facts in `doc/problem-brief.md`, all planning docs, future source files, and future test commands.

## Dependencies

All implementation module tasks. Open decisions: executable name, compiler/C++ standard, student ID for packaging.

## Tasks

- [x] Add a `Makefile` that builds the solver executable, assumed `RMST` unless the user changes it.
- [x] Use Linux-compatible C++ compiler flags and document the chosen compiler standard.
- [x] Add `readme.txt` with build and assignment-style run instructions only.
- [x] Keep required assignment artifacts in the repository root.
- [ ] Add or document the smallest sample smoke-check path once the executable exists.
- [x] Record unresolved packaging details such as student ID outside source code.

## Tests and Quality Gates

- [x] Run the future build command and confirm the executable is produced.
- [ ] Run the PDF sample smoke check and confirm output `13`.
- [x] Manually verify `readme.txt` documents `./RMST input.dat output.dat` style usage.

## Done When

- [x] `Makefile`, source, and `readme.txt` exist in the required location.
- [ ] Build and sample smoke gates pass once implementation exists.
- [x] No unrequested CI, package manager, or external dependency infrastructure is added.

## Notes

- Build verified with `make`; it produced `RMST`.
- The executable is only a safe scaffold now. It validates argument count, then returns nonzero because solver modules are not implemented yet.
- Sample smoke remains unchecked until the solver can produce the PDF sample output `13`.
- Packaging detail: student ID is unresolved, so final tar folder/name is not recorded in source code.
