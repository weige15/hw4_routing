# AGENTS.md

## Project Context

- Assignment: NYCU Physical Design Automation Spring 2026, Programming Assignment #4.
- Goal: implement Rectilinear Minimum Spanning Tree (RMST) total weight under Manhattan distance.
- Input: `n` points, `1 <= n <= 10,000,000`, coordinates are integers in `[-1e9, 1e9]`, duplicate coordinates are allowed.
- Output: one integer: the total RMST weight.
- Required CLI shape from the assignment: `[executable] [input file] [output file]`.
- Required submission contents from the assignment: source code, `Makefile`, and `readme.txt`.
- Platform: Linux.
- Preferred language from the assignment: C or C++.
- All distances and final RMST weight must use signed 64-bit integers.

## Repository Rules

- This repository was cloned from `git@github.com:weige15/hw4_routing.git`.
- Current branch at setup: `main`.
- At setup time, the repository had no tracked project files.
- Keep assignment artifacts in the repository root unless a later project convention is documented.
- Do not assume unverified commands, dependencies, or file layouts.

## Read-Only Discovery Commands

- `pwd`
- `git status --short --branch`
- `git branch --show-current`
- `git worktree list`
- `rg --files`
- `sed -n '1,220p' AGENTS.md`
- `sed -n '1,220p' README.md`
- `pdftotext -layout ../p4_routing.pdf -`

## Commands Requiring Permission

- Creating, editing, moving, renaming, or deleting files.
- Installing dependencies.
- Running build, test, lint, format, generation, benchmark, or server commands.
- Changing branches, creating branches, committing, pushing, rebasing, merging, stashing, or applying patches.
- Any command that writes outside the repository root or `/tmp`.

## Forbidden Commands

Do not run these unless the user explicitly requests the exact operation:

- `rm -rf`
- `git reset --hard`
- `git clean -fd`
- `git checkout -- .`
- `git restore .`
- `git push --force`
- `git push --force-with-lease`
- `chmod -R`
- `chown -R`
- `sudo`

## Build, Test, and Quality Gates

- Build command: Unknown until a `Makefile` exists.
- Test command: Unknown until tests or sample checks are added.
- Lint command: Unknown.
- Format command: Unknown.
- Type-check command: Unknown.
- Future implementations should include the smallest runnable correctness check for the sample case from the PDF.
- Performance-sensitive code should avoid O(n^2) graph construction; the assignment requires scalability up to 10,000,000 points.

## Documentation Rules

- Keep `readme.txt` focused on how to build and run the program.
- Do not copy secrets, tokens, private credentials, or local environment values into documentation.
- If detailed workflow notes become necessary, create `doc/workflow-rules.md` only with user approval.

## Coding Rules

- Prefer C or C++ unless the user chooses another language.
- Use signed 64-bit integers for edge weights, distance calculations, and the final RMST weight.
- The executable must accept exactly the assignment-style input and output file arguments unless the user changes the interface.
- The program should write only the RMST total weight to the output file.
- Avoid constructing the complete graph.
- Keep memory usage explicit and bounded; large-input paths should be designed for up to 10,000,000 points.
- Favor simple, verifiable code over speculative abstractions.

## Git and Commit Rules

- Do not commit, push, branch, merge, rebase, or stash without explicit user approval.
- Before any commit, show `git status --short` and summarize intended changes.
- Do not overwrite or revert user changes unless explicitly requested.

## Uncertainty Protocol

- Mark unverified repository facts as `Unknown`.
- If the PDF, user request, and repository files conflict, stop and ask for direction.
- If a command may write files or change git state, ask before running it.
