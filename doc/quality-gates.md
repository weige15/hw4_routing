# Quality Gates

## Environment Summary

- Repository root: `/home/kuotzuwei15/pda/hw4/hw4_routing`.
- Current repository contents discovered: `AGENTS.md`, `doc/problem-brief.md`, and `doc/repo-map.md`.
- Assignment platform: Linux.
- Preferred assignment language: C or C++.
- No source files, build files, CI files, scripts, evaluator assets, package metadata, or lockfiles were discovered.

## Build Commands

- **Missing**: no repository-defined build command was found.
- `AGENTS.md` records build command as unknown until a `Makefile` exists.
- `doc/problem-brief.md` records that the assignment requires a `Makefile`.

## Unit Test Commands

- **Missing**: no repository-defined unit test command was found.

## Integration Test Commands

- **Missing**: no repository-defined integration test command was found.

## Lint Commands

- **Missing**: no repository-defined lint command was found.

## Format Commands

- **Missing**: no repository-defined format command was found.

## Type-Check Commands

- **Missing**: no repository-defined type-check command was found.

## Static Analysis Commands

- **Missing**: no repository-defined static-analysis command was found.

## Benchmark or Evaluator Commands

- **Missing**: no repository-defined benchmark, grader, evaluator, or scoring command was found.
- Assignment grading criteria from `doc/problem-brief.md`: correctness and running time; each test case fails if runtime exceeds 1 hour.

## Smoke Test Commands

- **Missing**: no repository-defined smoke test command was found.
- `doc/problem-brief.md` includes a sample input and expected output, but no runnable script or command exists yet.

## Verified Commands

- None. No build, test, lint, format, type-check, static-analysis, benchmark, smoke-test, evaluator, package-manager, or project script command was run in this session.

## Commands Not Run

- No repository-defined quality-gate commands were available to run.
- No generic commands were run as substitutes because this document only records repo-defined gates.

## Missing Quality Gates

- Build command.
- Unit test command.
- Integration test command.
- Lint command.
- Format command.
- Type-check command.
- Static-analysis command.
- Benchmark or evaluator command.
- Smoke test command.

## Recommended Minimum Done Criteria

- After implementation adds source files and a `Makefile`, define a repository build command, likely through the required `Makefile`.
- Add one minimal sample smoke check using the PDF sample input and expected output `13`.
- Before treating implementation work as done, verify:
  - the repository-defined build command succeeds;
  - the sample smoke check succeeds;
  - the program accepts `[executable] [input file] [output file]`;
  - the output file contains only the RMST total weight;
  - distance calculations and final answer use signed 64-bit integers;
  - the implementation avoids O(n^2) complete-graph construction.
