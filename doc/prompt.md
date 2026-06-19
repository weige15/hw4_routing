# Implementation Prompt

## Objective

Implement the NYCU PDA Spring 2026 PA4 RMST solver end to end in this repository. The final program must read a point set from an input file, compute the total Rectilinear Minimum Spanning Tree weight under Manhattan distance, and write only that integer to an output file.

Target deliverables are repository-root `main.cpp`, `Makefile`, and `readme.txt`, plus any minimal tests or sample checks needed to prove correctness. Keep the implementation boring, compact, standard-library-only, and faithful to the planning docs.

## Inputs to Read First

Read these files before editing code:

- `AGENTS.md` - present; repository guardrails, assignment constraints, permissions, and git rules.
- `doc/problem-brief.md` - present; assignment facts, constraints, sample case, and open questions.
- `doc/repo-map.md` - present but partly stale; originally reported documentation-only repo and no build/test files.
- `doc/quality-gates.md` - present; all build/test/lint/evaluator commands are currently missing until implementation adds them.
- `doc/proposal.md` - present; product intent, algorithm strategy, validation plan, risks, and milestones.
- `doc/high-level-design.md` - present; module boundaries, relationships, data flow, and quality alignment.
- `doc/test-plan.md` - present; golden cases, oracle strategy, randomized/property tests, edge cases, and minimum done criteria.
- `doc/detailed-design.md` - present; implementation-ready module designs, data contracts, algorithm details, and assumptions.
- `doc/tasks/progress.md` - present; progress tracker for all module tasks and full-project gates.
- `doc/tasks/cli-and-orchestrator.md` - present.
- `doc/tasks/input-reader.md` - present.
- `doc/tasks/point-storage.md` - present.
- `doc/tasks/manhattan-mst-candidate-generator.md` - present.
- `doc/tasks/edge-storage.md` - present.
- `doc/tasks/disjoint-set-union.md` - present.
- `doc/tasks/kruskal-mst-solver.md` - present.
- `doc/tasks/output-writer.md` - present.
- `doc/tasks/small-case-baseline-oracle.md` - present.
- `doc/tasks/build-and-submission-artifacts.md` - present.

Optional source artifact:

- `p4_routing.pdf` - present in repository root; assignment PDF.

## Current Implementation

Current repository files discovered:

- `README.md`: contains only `# hw4_routing`.
- `AGENTS.md`: project instructions and assignment constraints.
- `p4_routing.pdf`: assignment PDF.
- `doc/*.md`: planning documents.
- `doc/tasks/*.md`: module task files and progress tracker.

There are currently no implementation files, no `main.cpp`, no `Makefile`, no `readme.txt`, no tests, no fixtures, no scripts, no package metadata, and no evaluator assets. `doc/prompt.md` is this implementation prompt.

Known commands:

- Build command: missing until `Makefile` exists.
- Unit/integration/smoke/benchmark/evaluator commands: missing.
- Lint, format, type-check, static analysis: missing.

Current git status at prompt creation showed untracked planning docs under `doc/`. Do not revert or overwrite user or prior-agent changes.

## Hard Constraints

- Language/platform: C or C++ preferred; use C++ with the standard library only unless the user approves otherwise.
- CLI: `[executable] [input file] [output file]`; default executable name should be `RMST` unless user says otherwise.
- Input: first line `n`, followed by `n` lines of `x y`.
- Bounds: `1 <= n <= 10,000,000`; `-1000000000 <= x,y <= 1000000000`.
- Duplicate coordinates are allowed and must preserve the same RMST total.
- Output: one integer only, the RMST total weight, written to the output file.
- Metric: Manhattan distance `|xi - xj| + |yi - yj|`.
- Use signed 64-bit integers for transformed coordinates, distances, edge weights, and final total.
- Do not construct the complete graph in the production path.
- Complete-graph MST is allowed only in a guarded small-case oracle.
- Runtime target: assignment tests fail above 1 hour.
- Required submission artifacts: source code, `Makefile`, `readme.txt`.
- Keep assignment artifacts in the repository root unless later docs say otherwise.

## Non-Goals

- Do not output RMST edges.
- Do not add external dependencies.
- Do not add CI, package managers, large frameworks, or broad project scaffolding.
- Do not implement multi-case input; the assignment defines one point set per file.
- Do not create final student-ID tar packaging unless explicitly asked and student ID is known.
- Do not spend effort on elaborate invalid-input UX; safe nonzero failure is enough unless a stricter requirement appears.

## Execution Model

1. Read all docs listed above before editing.
2. Update `doc/tasks/progress.md` when starting, completing, blocking, or verifying each module/workstream. Add short timestamped checkpoint notes if useful.
3. Implement one module or small workstream at a time.
4. Prefer a single compact `main.cpp` as planned. Split files only if it clearly reduces implementation risk or testability.
5. Keep write scopes small and avoid unrelated refactors.
6. Do not revert user or prior-agent edits.
7. After each module, run the smallest available check. If no command exists yet, mark that gate missing in progress and continue.
8. Once `Makefile` exists, use `make` as the build gate unless the file defines a different command.
9. At the end, run all configured quality gates and summarize command outputs.
10. Stop and ask only for true blockers: conflicting docs, destructive git actions, missing credentials, external services, or an unresolvable algorithm/spec decision.

## Module Workstreams

Use the task files as the source of truth for concrete steps.

| Workstream | Primary Task File | Expected Writes | Ownership |
| --- | --- | --- | --- |
| Build skeleton | `doc/tasks/build-and-submission-artifacts.md` | `Makefile`, `readme.txt`, initial `main.cpp` | Defines executable name, build command, and usage docs. |
| Core data contracts | `doc/tasks/point-storage.md`, `doc/tasks/edge-storage.md` | `main.cpp` structs/helpers | Owns `Point`, `Edge`, stable ids, and 64-bit distance helper. |
| Input/output boundary | `doc/tasks/input-reader.md`, `doc/tasks/output-writer.md`, `doc/tasks/cli-and-orchestrator.md` | `main.cpp`, `readme.txt` if needed | Owns file parser, output writer, CLI, and pipeline status handling. |
| DSU and Kruskal | `doc/tasks/disjoint-set-union.md`, `doc/tasks/kruskal-mst-solver.md` | `main.cpp` | Owns component tracking and MST accumulation over candidates. |
| Small-case oracle | `doc/tasks/small-case-baseline-oracle.md` | `main.cpp`, optional self-test path | Owns guarded brute-force MST for tiny correctness checks only. |
| Candidate generator | `doc/tasks/manhattan-mst-candidate-generator.md` | `main.cpp` | Owns duplicate zero-edge prepass, directional sweeps, and candidate emission. |
| Verification fixtures/checks | `doc/test-plan.md`, `doc/tasks/progress.md` | Optional `tests/`, optional `sample/`, `Makefile` test target if added | Owns golden cases, randomized oracle checks, and progress updates. |

Keep generated implementation minimal. If adding tests, prefer the smallest runnable check that catches real failures: PDF sample, golden cases, and deterministic oracle comparisons.

## Subagent Plan

Because the planned implementation is mostly one `main.cpp`, concurrent code-writing subagents are not a good default; they would overlap heavily. The main agent should own `main.cpp` integration.

Useful optional subagents:

- Algorithm review subagent: read-only review of the Manhattan MST sweep design in `doc/detailed-design.md` and the implemented candidate generator. It may report issues but should not edit files.
- Test-data subagent: may create or update only test fixtures/scripts if the main agent first chooses a `tests/` or `sample/` layout. It must not edit `main.cpp`.
- Documentation/build subagent: may edit only `Makefile` and `readme.txt` after the main agent defines executable name and source layout.

If the future run deliberately splits source files, define disjoint ownership before spawning subagents, for example:

- `src/io.*`: Input Reader, Output Writer, CLI helpers.
- `src/graph.*`: Edge, DSU, Kruskal, oracle.
- `src/manhattan_mst.*`: Candidate Generator.

The main agent must merge and run integration checks after any subagent work.

## Implementation Order

1. Update `doc/tasks/progress.md` to mark Build and Submission Artifacts started.
2. Add `Makefile`, initial `main.cpp`, and `readme.txt`.
   - Local check after this step: `make` should build `RMST`.
3. Implement Point Storage and Edge Storage.
   - Local check: compile with `make`; verify a small self-test or manual inspection for 64-bit distance helper.
4. Implement Input Reader, Output Writer, and CLI/Orchestrator.
   - Local check: run the PDF sample only after a temporary or partial solver can produce a known value; otherwise compile and missing-argument check.
5. Implement DSU and Kruskal MST Solver.
   - Local check: use known internal candidate graph/self-test or golden tiny cases if the oracle path exists.
6. Implement Small-Case Baseline Oracle.
   - Local check: golden cases from `doc/test-plan.md` through oracle.
7. Implement Manhattan MST Candidate Generator.
   - Local check: optimized-vs-oracle on golden cases and deterministic random seeds.
8. Add the smallest practical test/check path.
   - Preferred: `make test` if it stays simple; otherwise document exact sample and oracle commands in `readme.txt`.
9. Run final quality gates:
   - `make`
   - sample run using the PDF input, expecting `13`
   - golden cases
   - deterministic oracle comparisons
   - at least one medium/structured performance check if practical
10. Update `doc/tasks/progress.md` after each verified workstream and before final response.

## Testing and Quality Gates

Configured commands currently missing:

- Build: missing until `Makefile`.
- Unit tests: missing.
- Integration tests: missing.
- Smoke tests: missing.
- Lint/format/type/static analysis: missing.
- Evaluator/benchmark: unknown/missing.

Required future checks once implementation defines commands:

- Build: run `make`.
- CLI sample:

```text
input:
5
0 0
2 0
2 3
5 1
6 4

expected output:
13
```

- Golden cases from `doc/test-plan.md`: one point `0`, two distant points `4000000000`, duplicates, unit square, collinear cases, negative coordinates, duplicate clusters, center and corners.
- Optimized-vs-oracle randomized checks using seeds `20260608`, `20260609`, `1`, `2`, `3`.
- Output format: output file contains one integer only, with optional final newline.
- Manual/code review gate: production path must not build complete graph.
- Performance sanity: duplicate-heavy and collinear structured workloads should show near-linear candidate count and avoid O(n^2) behavior.

If a command is still missing, do not claim it passed. Record it as missing or blocked in `doc/tasks/progress.md` and final response.

## Progress Tracking

Use `doc/tasks/progress.md` as the live tracker.

Required updates:

- Mark a module/workstream started before editing its files.
- Mark task checkboxes complete only after the work is actually done.
- Add short notes for blocked items, failed checks, algorithm changes, or command gaps.
- Mark module status complete only after its implementation and module-specific verification pass.
- Mark full-project gates complete only after running the actual gate.

Preserve existing checked boxes unless the current design invalidates them.

## Commit or Checkpoint Strategy

Do not commit, push, branch, merge, rebase, or stash unless the user explicitly asks.

If commits are requested later:

- Show `git status --short` first.
- Group commits/checkpoints by coherent workstream, such as build skeleton, parser/IO, graph core, candidate generator, tests/docs.
- Do not include unrelated changes.

Without commits, keep the final diff easy to review by grouping changes by module in the final response.

## Acceptance Criteria

The implementation is done only when:

- `main.cpp`, `Makefile`, and `readme.txt` exist in the repository root.
- `make` or the repository-defined build command succeeds.
- The executable accepts `[executable] [input file] [output file]`.
- The output file contains only the RMST total weight.
- The PDF sample produces exactly `13`.
- Golden cases from `doc/test-plan.md` pass.
- Small randomized optimized-vs-oracle comparisons pass for the planned deterministic seeds.
- The production path avoids complete-graph construction and O(n^2) behavior.
- Distances, edge weights, transformed coordinates, and final total use signed 64-bit arithmetic where needed.
- Duplicate coordinates are handled correctly.
- Module task files in `doc/tasks/` are completed or explicitly marked blocked with reason.
- Full-project gates in `doc/tasks/progress.md` are updated honestly.
- `readme.txt` explains how to build and run the program.
- No unrelated files are changed.

## Uncertainty Protocol

Make conservative documented assumptions when safe:

- Default executable name: `RMST`.
- Default compiler: `g++`.
- Default C++ standard: C++17.
- Default dependency policy: C++ standard library only.
- Default invalid input behavior: nonzero exit with `stderr` diagnostics.
- Default duplicate handling: preserve duplicate vertices and add zero-weight duplicate edges.

Ask the user before:

- Using third-party libraries.
- Performing destructive git or filesystem operations.
- Changing the CLI or output format.
- Requiring a specific student ID/package name.
- Ignoring a conflict between assignment PDF, planning docs, and repo files.
- Making a large architecture split that contradicts the detailed design.

## Final Response Requirements

The final response must be concise and include:

- What was implemented, grouped by workstream.
- Changed files, grouped by module or artifact.
- Tests and quality gates run, with command summaries and pass/fail status.
- Any commands not run and why.
- Known limitations or unresolved open questions.
- Any follow-up required before submission.

Do not claim a gate passed unless it was actually run.
