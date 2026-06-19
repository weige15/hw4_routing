# Task Progress

## Module Status

- [x] CLI and Orchestrator (`doc/tasks/cli-and-orchestrator.md`)
- [x] Input Reader (`doc/tasks/input-reader.md`)
- [x] Point Storage (`doc/tasks/point-storage.md`)
- [x] Manhattan MST Candidate Generator (`doc/tasks/manhattan-mst-candidate-generator.md`)
- [x] Edge Storage (`doc/tasks/edge-storage.md`)
- [x] Disjoint-Set Union (`doc/tasks/disjoint-set-union.md`)
- [x] Kruskal MST Solver (`doc/tasks/kruskal-mst-solver.md`)
- [x] Output Writer (`doc/tasks/output-writer.md`)
- [x] Small-Case Baseline Oracle (`doc/tasks/small-case-baseline-oracle.md`)
- [x] Build and Submission Artifacts (`doc/tasks/build-and-submission-artifacts.md`)

## Full-Project Gates

- [x] Build passes
- [x] Unit tests pass
- [ ] Lint passes
- [ ] Format check passes
- [ ] Type/static analysis passes if configured
- [ ] Evaluator or benchmark passes if configured

## Notes

- Build and Submission Artifacts: partial. Added root `Makefile`, `main.cpp`, and `readme.txt`; `make` passed and `RMST` exists. Module remains incomplete because the PDF sample smoke check cannot pass until the solver pipeline is implemented.
- CLI scaffold check: `./RMST` with missing arguments returned nonzero and printed usage to `stderr`.
- 2026-06-20: Started boundary implementation cycle covering CLI orchestration, Point Storage, Input Reader, and Output Writer. The RMST solver itself remains intentionally incomplete beyond trivial single-point input.
- 2026-06-20 boundary evidence: `make` passed; `./RMST` returned code 1 for missing args; one-point input wrote exactly `0`; `n = 0`, too few coordinate pairs, and out-of-range coordinates returned code 2 without result files; the PDF sample and duplicate multi-point input reached the solver placeholder with code 3; directory output path returned code 4.
- 2026-06-20 resolved blocker: PDF sample smoke was previously blocked until Candidate Generator, DSU, and Kruskal were implemented; it now passes with output `13`.
- 2026-06-20 core MST evidence: added `make test`; `make` and `make test` passed. Self-tests cover DSU singleton/repeated/transitive unions, Kruskal known connected and disconnected graphs, 64-bit brute-force oracle total `4000000000`, duplicate zero-distance handling, and oracle limit refusal.
- 2026-06-20 final solver evidence: implemented Manhattan candidate generation and wired the full solver. `make`, `make test`, full CLI golden suite, 100k collinear (`99999`, elapsed `0.05s`, maxrss `16640 KB`), and 100k all-duplicate (`0`, elapsed `0.04s`, maxrss `15208 KB`) passed.
- 2026-06-20 implementation note: the detailed design's original `-wy`/Fenwick sweep was corrected to the cp-algorithms/KACTL ordered-active-set sweep after oracle tests exposed a unit-square miss. The current implementation uses `std::map`; a vector-backed active set is a future performance optimization if profiling requires it.
