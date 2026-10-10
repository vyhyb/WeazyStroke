# Dev release steps

Pushing a `v*` tag runs `.github/workflows/release.yml`: it builds, tests and packages `.rpm` and `.deb` for x86_64 and arm64, then creates the GitHub release.

## 1. Prepare

- Merge all work through pull requests, so the generated "What's Changed" list includes it.
- Update `RELEASE_NOTES.md` to match the current state of the app. It is the hand-written part of the release notes.
- Update `README.md` and `docs/*.md` if behavior changed; regenerate the HTML (`cd docs && ./md2html.sh && ./postprocess-html.sh && ./lint-md.sh`).
- Run the tests: `cmake --build build && ctest --test-dir build --output-on-failure`.
- Make sure `master` is pushed and `git status` is clean.

## 2. Dry run (optional)

In the Actions tab, run the **Release** workflow manually on `master`. It builds the four packages as artifacts but creates no release.

## 3. Tag and push

```bash
git tag vX.Y.Z
git push origin vX.Y.Z
```

The package version is taken from the tag (`v0.1.0` gives `0.1.0`).

## 4. Verify

- Actions: all four package jobs and the release job are green.
- Releases page: four packages are attached, and the notes show `RELEASE_NOTES.md` followed by "What's Changed".
- Download one package and check it: `rpm -qpl weazystroke-*.rpm` or `dpkg-deb -c weazystroke_*.deb`.

## Redo a bad release

Delete the release on GitHub, then the tag, fix the problem, and tag again:

```bash
git tag -d vX.Y.Z
git push origin :refs/tags/vX.Y.Z
```
