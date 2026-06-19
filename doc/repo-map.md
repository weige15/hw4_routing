# Repository Map

## Repository Summary

- Repository path: `/home/kuotzuwei15/pda/hw4/hw4_routing`.
- Remote source stated earlier by the user: `git@github.com:weige15/hw4_routing.git`.
- The repository currently contains setup documentation only.
- No implementation files, build system, tests, scripts, data files, or assets were found by discovery.
- The assignment context is RMST total weight computation for NYCU Physical Design Automation Spring 2026 Programming Assignment #4.

## Directory Structure

```text
.
├── AGENTS.md
└── doc/
    └── problem-brief.md
```

## Main Source Files

- None found.

## Existing Tests

- None found.

## Build System

- No `Makefile`, package metadata, lockfile, or build configuration was found.
- `AGENTS.md` records the build command as unknown until a `Makefile` exists.
- `doc/problem-brief.md` records that the assignment requires a `Makefile` for submission.

## Runtime or CLI Entry Points

- No executable source file or script entry point exists yet.
- Required future CLI shape from the assignment: `[executable file name] [input file name] [output file name]`.

## Data and Assets

- No data or asset files were found inside the repository.
- The assignment PDF is outside the repository at `../p4_routing.pdf` relative to the repo root.

## Existing Documentation

- `AGENTS.md`: project guardrails for future Codex sessions, including assignment constraints, permission rules, forbidden commands, and coding rules.
- `doc/problem-brief.md`: source-grounded assignment brief extracted from `../p4_routing.pdf`.

## Detected Dependencies

- None found.
- No C/C++ compiler settings, standard version, third-party libraries, or package manager metadata are present.

## Important Scripts

- None found.

## Current Git State

- `git status --short` showed:

```text
?? AGENTS.md
?? doc/
```

- There are no commits yet on `main` according to earlier setup discovery.
- `AGENTS.md` and `doc/problem-brief.md` are untracked.
- `doc/repo-map.md` is being added by this reconnaissance step.

## Missing or Ambiguous Areas

- Missing implementation source files.
- Missing `Makefile`.
- Missing `readme.txt`.
- Missing tests or sample-check script.
- Missing exact compiler, C/C++ standard, optimization flags, and memory limit.
- Missing student ID for final submission folder and tar file naming.
- Missing policy on external libraries.
- Missing confirmed handling policy for malformed input and duplicate coordinates.

## Notes for Future Skills

- Use `doc/problem-brief.md` as the factual source for proposal and design work.
- Do not assume build, test, lint, or format commands until files define them.
- Future implementation should preserve the required assignment CLI, output-only result, 64-bit arithmetic, and no-complete-graph constraint.
- Keep changes small and rooted in the current empty-repo state.
