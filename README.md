# Keyboards

This repos contains my custom keyboard firmware and layouts for Keyboardio [Kaleidoscope](https://github.com/keyboardio/Kaleidoscope) powered keyboards.

In order to build custom firmware that is compatible with the specific version of Kaleidoscope, the Kaleidoscope version is linked as a [git submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules) into this repo. See below for special steps when cloning and building the firmware.

## First Time Setup

1. Start by cloning this repo and fetching the Kaleidoscope sources:

    ```shell
    git clone --recurse-submodules https://github.com/mmercurio/keyboards
    ```

2. Then ask Kaleidoscope to install the compiler toolchain, arduino-cli and platform support:

    ```shell
    cd keyboards/Kaleidoscope
    make setup
    cd ..
    ```

3. Set `KALEIDOSCOPE_DIR` to the location of the Kaleidoscope installation:

    ```shell
    export KALEIDOSCOPE_DIR=$PWD/Kaleidoscope
    ```

## Building and Flashing Custom Firmware

Once the First Time Setup is completed and `KALEIDOSCOPE_DIR` is set, custom keyboard firmware can be built and flashed by executing `make flash` in the keyboard model directory.

For example, to build and flash firmware for the Model 100:

```shell
cd Model100
make flash
```
