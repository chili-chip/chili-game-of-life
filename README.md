# Conway's Game of Life

[![Build](https://github.com/chili-chip/chili-game-of-life/actions/workflows/build.yml/badge.svg)](https://github.com/chili-chip/chili-game-of-life/actions/workflows/build.yml)

Conway's cellular automaton running full screen on the ChiliChip **VGC Zero** (128x128), built on the
[chilichip fork of the 32blit SDK](https://github.com/chili-chip/32blit-sdk).
It also runs on desktop (SDL) and in the browser: [try it on chilichip.eu](https://chilichip.eu/games/game-of-life/).

Every pixel is a cell. The board starts from a random seed and wraps around at the edges. A new
generation is computed every 100 ms with the usual rules: a live cell with two or three live
neighbours survives, a dead cell with exactly three comes alive, everything else dies.

## Controls

| Button | Action |
| --- | --- |
| MENU | Restart with a new random seed |

On desktop, **Esc** is MENU.

## Code layout

Everything is in `game.cpp`: two 128x128 buffers that are swapped each generation, the neighbour
count with toroidal wrap, and a renderer that draws runs of live cells as single rectangles.

## Building

New to the SDK? Start with the [Linux quickstart](https://chilichip.eu/docs/game-sdk/quickstart/linux-quickstart/) on chilichip.eu. It installs the toolchain and clones `32blit-sdk`, `pico-sdk` and `pico-extras` next to this repo. CMake 3.28 or newer is needed for the presets in `CMakePresets.json`.

Desktop (SDL):
```
mkdir build && cd build
cmake -D32BLIT_DIR=../../32blit-sdk ..
make
./game
```

VGC Zero:
```
mkdir build.vgc && cd build.vgc
cmake .. \
  -D32BLIT_DIR=../../32blit-sdk \
  -DPICO_SDK_PATH=../../pico-sdk \
  -DPICO_BOARD=chilichip_vgc \
  -DPICO_PLATFORM=rp2350-arm-s \
  -DCMAKE_TOOLCHAIN_FILE=../../32blit-sdk/pico2.toolchain
make
```
This produces a `.uf2` you can copy onto the console's `RP2350` drive. To build an installable `.blit` for the VGC Zero loader instead, add `-DBLIT_EXECUTABLE_PICO_BLIT=1` and install it with `32blit install game.blit`. See the [VGC build guide](https://github.com/chili-chip/32blit-sdk/blob/master/docs/vgc.md) for details, including flashing over SWD.

## Licence

MIT, see [LICENSE](LICENSE).
