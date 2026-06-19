# Review Notes

## Review Scope

- Reviewed the current scaffold implementation after the Build and Submission Artifacts cycle.
- Scope covered assignment requirements, planning docs, task progress, root `Makefile`, `main.cpp`, and `readme.txt`.
- No broad repair was attempted because the missing RMST solver is full implementation work, not a small review fix.

## Files Reviewed

- `doc/problem-brief.md`
- `doc/proposal.md`
- `doc/high-level-design.md`
- `doc/test-plan.md`
- `doc/detailed-design.md`
- `doc/tasks/progress.md`
- `doc/tasks/build-and-submission-artifacts.md`
- `doc/tasks/cli-and-orchestrator.md`
- `Makefile`
- `main.cpp`
- `readme.txt`

## Commands Run

- `git status --short` - pass; repository has untracked implementation/planning files, including `Makefile`, `RMST`, `main.cpp`, `readme.txt`, and `doc/tasks/`.
- `git diff -- Makefile main.cpp readme.txt doc/tasks/build-and-submission-artifacts.md doc/tasks/progress.md` - pass; no output because reviewed files are untracked.
- `rg --files` - pass; listed root implementation artifacts and planning docs.
- `make` - pass; target was already up to date and `RMST` exists.
- `./RMST` - pass for missing-argument behavior; exited nonzero and printed usage to `stderr`.
- `printf '5\n0 0\n2 0\n2 3\n5 1\n6 4\n' > /tmp/rmst_review_sample.in` - pass; created a temporary PDF sample input.
- `./RMST /tmp/rmst_review_sample.in /tmp/rmst_review_sample.out` - fail for assignment smoke test; exited `2` with `RMST solver pipeline is not implemented yet.`
- `test -x RMST` - pass; executable exists.
- `test ! -e /tmp/rmst_review_sample.out` - pass; failed scaffold run did not create a bogus output file.

## Summary

- The current implementation is a buildable C++17 scaffold, not an RMST solver.
- Build artifacts are present and minimal.
- CLI missing-argument behavior is safe.
- Valid assignment invocations fail because the solver pipeline is intentionally not implemented.

## Requirement Match

- Matches: root `Makefile` exists and builds `RMST`.
- Matches: root `readme.txt` documents `make` and `./RMST input.dat output.dat`.
- Partially matches: `main.cpp` accepts exactly two file-path arguments.
- Does not match: no input parser, RMST computation, candidate generation, Kruskal solver, or output writer exists.
- Does not match: PDF sample does not produce `13`.
- Does not match: no golden, randomized oracle, output-format, or performance gates can pass yet.

## Module Boundary Check

- Build artifact changes stay in the expected root files.
- `main.cpp` only contains CLI scaffolding and does not cross into unimplemented modules.
- `doc/tasks/progress.md` correctly leaves all module status checkboxes unchecked.
- `doc/tasks/build-and-submission-artifacts.md` correctly leaves sample smoke incomplete.

## Test Coverage Check

- Build and missing-argument behavior have manual command evidence.
- There is no repository test target.
- There are no unit, integration, golden, randomized, oracle, lint, format, static-analysis, evaluator, or benchmark checks configured.

## Edge Cases

- `unresolved`: `n = 1`, duplicate points, coordinate extremes, negative coordinates, collinear cases, and malformed input are not handled because no parser or solver exists.
- `unresolved`: output-file content cannot be validated on successful runs because no successful solver path exists.

## Performance Concerns

- `unresolved`: no production algorithm exists, so scalability to `n <= 10,000,000` is unproven.
- `unresolved`: no memory layout, candidate edge bounds, or O(n log n) sweep behavior is implemented.

## Bugs Found

- `unresolved`: `main.cpp:5` to `main.cpp:9` always reports the RMST solver pipeline as not implemented and returns `2`, so every valid assignment invocation fails.
- `unresolved`: `main.cpp:14` to `main.cpp:20` validates argument count but does not sequence input reading, candidate generation, Kruskal, or output writing.
- `unresolved`: `readme.txt:4` to `readme.txt:5` documents the final run shape, but the executable currently cannot complete that command successfully.
- `needs-user-decision`: final packaging folder/tar name still needs the student ID.

## Repairs Made

- None. The only substantive failures require implementing planned modules, which is outside a narrow review repair.

## Remaining Issues

- `unresolved`: implement Input Reader.
- `unresolved`: implement Point Storage and Edge Storage.
- `unresolved`: implement DSU and Kruskal MST Solver.
- `unresolved`: implement Small-Case Baseline Oracle.
- `unresolved`: implement Manhattan MST Candidate Generator.
- `unresolved`: implement Output Writer.
- `unresolved`: add or document a sample smoke-check path once the solver can produce `13`.
- `unresolved`: add focused checks for golden cases and deterministic oracle comparisons.
- `needs-user-decision`: provide student ID for final packaging instructions.

## Recommended Next Steps

- Continue with the next implementation-loop cycle for core data contracts and input/output boundary.
- Keep `make` as the build gate.
- Add the smallest executable correctness check once the solver can return a value.

## Final Readiness

- Not ready
