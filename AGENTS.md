# Development Guidelines

This is a personal fork (`vyhyb/WeazyStroke`) of `nine7nine/WeazyStroke`. Changes should stay easy to submit upstream as pull requests.

## Remotes

- `origin`: the fork (push here).
- `upstream`: nine7nine/WeazyStroke (read-only; push URL disabled).
- Sync with `git fetch upstream && git merge upstream/master`.

## Incremental development

- Work in small steps. Each step does one thing and leaves the build and tests green.
- Add or update a test with every behavior change, in `tests/` (one `eswl_*_test` target per area, registered in `CMakeLists.txt`).
- Build and run the tests before moving on:
  `cmake --build build && ctest --test-dir build --output-on-failure`
- Do not start the next step until the current one is verified.
- One logical change per commit, with a short imperative message. Do not mix refactors, features and docs in one commit.

## Recognition core

- Keep the original EasyStroke recognition core intact: `src/stroke.c`, `src/stroke.h` and the matching logic in `src/gesture_recognizer.*`.
- Do not reformat, rename or restructure these files. Any unavoidable change must be minimal, isolated in its own commit and covered by `tests/recognition_test.cc`.
- Add new behavior around the core (config, input, actions), not inside it.

## Code quality

- Prefer simple, readable code over clever or generic code.
- Follow the existing style and file layout. Match naming and formatting of the surrounding code.
- Keep each file focused on one responsibility. Do not add helpers, abstractions or new files for one-off use.
- No speculative features, no unrelated cleanups, no drive-by reformatting.
- Comments only state what the code cannot show, in one short line.

## Keeping upstream PRs simple

- Keep diffs small and local. Avoid touching files that do not need to change.
- Keep fork-specific changes (personal docs such as `WeazyStroke-Fedora.md`, local packaging tweaks) in separate commits from general improvements, so general ones can be cherry-picked.
- Branch from the latest `upstream/master` for anything intended for a PR.
- Develop each separate feature or fix on its own branch and merge it through a pull request. The release workflow builds its "What's Changed" notes from merged PRs, so direct commits to `master` would be missing from them.
- Before tagging a release, update `RELEASE_NOTES.md` (the hand-written part of the release notes) to match the current state of the app.
- Update docs (`README.md`, `docs/*.md`) in the same change as the behavior they describe.
- Edit the `docs/*.md` sources, never the generated `docs/*.gen.html`. Regenerate them with `docs/md2html.sh` and `docs/postprocess-html.sh`, and run `docs/lint-md.sh` before committing.
- Keep docs accurate: remove or fix text that no longer matches the code, and document new config options and action types where they are listed.
- Never commit build output (`build/`) or scratch files (`cmake_err.txt`).
