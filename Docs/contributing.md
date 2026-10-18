
# Contributing

## Branches

We use GitFlow.

| Branch | What it is |
|--------|------------|
| `main` | The released version. A push here publishes a release. |
| `dev` | Where the work of everybody is gathered. |
| `feature/...` | One task. Start it from `dev` and merge it back into `dev`. |

A pull request to `main` can only come from `dev`. The CI checks it.

!!! tip "Merging dev into your branch"
    Run `git fetch origin` first. Without it, Git compares with an old copy and says "Already up to date" even if `dev` moved. To see what you are missing: `git log HEAD..origin/dev`.

## What the CI checks

| Workflow | What it does |
|----------|--------------|
| `build` | Builds the project |
| `ci` | Checks the file names (PascalCase) and the GitFlow rule |
| `release` | Publishes a release when `main` changes |
| mirror | Copies the repository to the Epitech one |

## File names

- Source files, headers, assets and folders use **PascalCase**: `MenuRenderer.cpp`, `RTypeTitle.png`. No underscore, no space.
- Not checked: `*.md`, files starting with a dot, `CMakeLists.txt`, `*.cmake`, `vcpkg.json` and the other build files.
- If the CI refuses a name, rename the file with `git mv` and update where it is used (an `#include`, a `GetTexture("...")`, a line in a `CMakeLists.txt`).

## Code style

- The format is defined by `.clang-format` (4 spaces, 100 columns). Run it before committing.
- A function does one thing and stays short. If you need a comment to separate the steps of a function, cut it into several.
- Everything a header can declare goes in the header: constants, types, member variables. The `.cpp` only contains definitions.
- Do not leave code that nobody calls.
- Comments and documentation are in English.
- An invalid command line ends the program with the exit code **84**.

## Documentation

This website is written in Markdown in the `Docs` folder of the repository, and the menu is in `mkdocs.yml` at the root. It is built with [MkDocs](https://www.mkdocs.org/) and the [Material](https://squidfunk.github.io/mkdocs-material/) theme.

To see your changes on your computer:

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r Docs/requirements.txt
mkdocs serve
```

Then open the address printed in the terminal. The pages update when you save. The `.venv` and `site` folders are generated: do not commit them.
