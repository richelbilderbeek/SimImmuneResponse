# Building

SimImmuneResponse is written in C++17 and uses Qt 5 (Qt Widgets).

## Linux

On Debian or Ubuntu, install the dependencies:

```bash
sudo apt install build-essential qtbase5-dev qt5-qmake
```

Build and run the simulation:

```bash
qmake SimImmuneResponse.pro
make
./SimImmuneResponse
```

## Tests

Build and run the tests:

```bash
qmake SimImmuneResponseTest.pro
make
./SimImmuneResponseTest
```

On a computer without a screen, run the tests with
`QT_QPA_PLATFORM=offscreen ./SimImmuneResponseTest`.

## Code structure

Folder            |Content
------------------|----------------------------------------------------------------------------------------
`src`             |The simulation core (plain C++, no Qt) and the Qt user interface
`src/resources`   |The pictures of the cells
`tests`           |The tests
`docs`            |This website, built with [Zensical](https://zensical.org)

The simulation core consists of:

Class               |Description
--------------------|----------------------------------------------------------
`SimImmuneResponse` |The simulation, consisting of four locations
`Location`          |A location in the body, containing sprites
`Sprite`            |A cell or molecule, moving around in a location
`SimState`          |Which cells are where, used to give the player feedback

## This website

To view this website locally, install Zensical and serve the website:

```bash
pip install zensical
./scripts/deploy_zensical_website_locally.sh
```
