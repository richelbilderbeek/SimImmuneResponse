# SimImmuneResponse

[![Create Zensical website](https://github.com/richelbilderbeek/SimImmuneResponse/actions/workflows/create_zensical_website.yaml/badge.svg)](https://github.com/richelbilderbeek/SimImmuneResponse/actions/workflows/create_zensical_website.yaml)
[![Build and test](https://github.com/richelbilderbeek/SimImmuneResponse/actions/workflows/build_and_test.yaml/badge.svg)](https://github.com/richelbilderbeek/SimImmuneResponse/actions/workflows/build_and_test.yaml)

Educational simulation of the human immune response using C++ and Qt.

- [Go to the nicely rendered website](https://richelbilderbeek.github.io/SimImmuneResponse/)

![SimImmuneResponse](docs/pics/screenshot_qt.png)

The code can be found in the folder `src`,
the tests in `tests`, the website in `docs`.
See [the building page](docs/building.md) for how to build the simulation.

## Files used by continuous integration scripts

<!-- markdownlint-disable MD013 --><!-- Tables cannot be split up over lines, hence will break 80 characters per line -->

Filename                                  |Descriptions
------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------
[mlc_config.json](mlc_config.json)        |Configuration of the link checker, use `markdown-link-check --config mlc_config.json --quiet docs/**/*.md` to do link checking locally
[.spellcheck.yml](.spellcheck.yml)        |Configuration of the spell checker, use `pyspelling -c .spellcheck.yml` to do spellcheck locally
[.wordlist.txt](.wordlist.txt)            |Whitelisted words for the spell checker, use `pyspelling -c .spellcheck.yml` to do spellcheck locally
[.markdownlint.jsonc](.markdownlint.jsonc)|Configuration of the markdown linter, use `markdownlint "**/*.md"` to do markdown linting locally. The name of this file is a default name.
[.markdownlintignore](.markdownlintignore)|Files ignored by the markdown linter, use `markdownlint "**/*.md"` to do markdown linting locally. The name of this file is a default name.
[zensical.toml](zensical.toml)            |Configuration of the website, use `./scripts/deploy_zensical_website_locally.sh` to view the website locally

<!-- markdownlint-enable MD013 -->
