# Contributing

## Code style

- The format comes from `.clang-format` (LLVM base, 4 spaces, 80 columns). Run `clang-format` before committing. Everyone uses the same version, the one of the CI.
- Everything is in a class: no free functions. A helper is a `static` member of the class that needs it.
- Keep functions short, **25 lines at most**: one function does one thing. If a function needs a comment to explain its steps, cut it into smaller functions.
- Everything a header can declare goes in the header: constants, types, member variables. The `.cpp` only holds definitions.
- Comments and documentation are written in English.

## File names

- Source and header files use PascalCase (`MenuRenderer.cpp`). The CI refuses underscores. The `.md` files, dotfiles, `CMakeLists.txt`, `*.cmake` and `vcpkg.json` are not checked.
- The same rule applies to assets: write `RTypeTitle.png`, not `R_Type_Title.png`.

## Git

- GitFlow: `main` is the release branch, `dev` is where the work is gathered, and each task has its own `feature/...` branch.
- A pull request to `main` only comes from `dev`. Feature branches are merged into `dev`.
- Before merging `dev` into your branch, run `git fetch origin`, otherwise Git says "already up to date" on an old copy.
- Write commit messages that say what changed and why. One pull request, one subject.

## CI

The CI runs on GitHub Actions:

| Workflow | What it does |
|----------|--------------|
| `build` | Compiles the project |
| `ci` | Checks the file names and the GitFlow rule |
| `release` | Publishes a release on a push to `main` |
| `docs` | Builds this site, and publishes it on a push to `main` |
| mirror | Copies the repository to the Epitech one |

## Documentation

This site is written in Markdown in the `Docs` folder, and built with [MkDocs](https://www.mkdocs.org/) and the Material theme. To see your changes on your machine:

```bash
pip install -r Docs/requirements.txt
mkdocs serve
```

Then open `http://127.0.0.1:8000`. Rules for the pages:

- Give every image a real alternative text that says what the image shows, not just `image`.
- Give every diagram a short text description next to it.
- Use meaningful link texts (`the protocol page`), never `click here`.
