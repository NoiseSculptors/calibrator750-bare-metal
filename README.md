
# Calibrator750 Bare-Metal Firmware Environment

Minimal, fast build system and register-level firmware framework for the
**Noise Sculptors Calibrator750** board (STM32H750 Cortex-M7). Designed for
experimentation, testing, and educational use **without HAL or CMSIS drivers.**

<p align="center">
  <img src="https://github.com/NoiseSculptors/calibrator750-zephyr/blob/main/boards/noisesculptors/calibrator750/doc/Calibrator750.webp" alt="Calibrator750 board" width="50%"/>
</p>

---

### Development Status

This is an early **beta version** of the Calibrator750 bare-metal environment —
already highly functional and suitable for experimentation, education, and
low-level testing.  Many values are still hard-coded for simplicity, but the
framework will evolve into a fully configurable board support package with
modular peripheral initialization.

---

### About the STM32H750

The **STM32H750** is a high-performance Arm® Cortex®-M7 microcontroller running
at **up to 480 MHz**, delivering over **2400 CoreMark** while maintaining
excellent power efficiency.

It combines a **double-precision FPU**, **L1 instruction/data caches**, and
**128 KB of on-chip Flash** with **1 MB of SRAM**, providing a cost-efficient
yet powerful platform for real-time signal processing, audio, and graphics
applications.  The H750 integrates rich peripherals — including USB HS/FS,
FD-CAN, Ethernet MAC, LCD-TFT, Chrom-ART™, JPEG accelerator, and multiple
high-resolution timers — and benefits from STMicroelectronics’ **10-year
product longevity commitment**, ensuring long-term availability for industrial
and creative hardware designs.

---

## Overview

This repository provides:

- Simple **CMake-based build system**
- Full **startup code** and **linker scripts** for flash, AXI SRAM, and DTCM execution
- Modular source layout (`src/`, `drivers/`, `examples/`, `include/`)
- External lightweight libraries:
  - [UGUI](https://github.com/achimdoebler/UGUI) (tiny embedded GUI)
  - [printf](https://github.com/mpaland/printf) (minimal `printf` replacement)
- Ready-to-run **examples** for timers, UART, I²C OLED display (SSD1315), and more

Everything builds directly with `arm-none-eabi-gcc` (Debian-stable tested), no
external toolchain integration or CMake needed.

---

## Build Options 

Select the desired build memory region in `makefile.conf`:

```make
# ---- Build profile selection ----
# Choose one: flash (0x08000000) | axi (0x24000000)
-DRUN = axi
````
| Profile | Memory region | Notes                            |
| ------- | ------------- | -------------------------------- |
| `flash` | 0x08000000    | Run from internal Flash (128K)   |
| `axi`   | 0x24000000    | Run from AXI SRAM (fast, 518K)   |
---

## Build Example

First build from the main directory.

```bash
# Clone with submodules 
git clone --recurse-submodules https://github.com/NoiseSculptors/calibrator750-bare-metal

# (If you already cloned without --recurse-submodules)
# git submodule sync --recursive
# git submodule update --init --recursive

cd calibrator750-bare-metal/

# Configure out-of-source (keeps tree clean; use Ninja for speed if you have it)
# change RUN=flash to axi or dtcm as needed
cmake -S . -B build \
  -G Ninja \
  -DPART=h750 \
  -DRUN=flash \
  -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi-gcc.cmake \
  -DCMAKE_COLOR_DIAGNOSTICS=ON \
  -DCMAKE_BUILD_TYPE=Release

cmake --build build

# Binaries:
# build/examples/<example>/<example>.bin

```

---

## Flashing

Use your preferred programmer, e.g.:

```bash
st-flash write oled_cube.bin 0x08000000
```

or run directly from RAM using a debugger (`stlink`, `openocd`, or `pyocd`).

Flashing **without a probe** is also possible via USB by booting (reset) with the
**BOOTSEL** button pressed — this activates the built-in STM32 DFU bootloader.

---

## Run from RAM
### Using the Calibrator750-Trampoline

If you prefer to **run applications entirely from RAM**, flash the tiny
trampoline_h750.bin once to flash memory at `0x08000000`:

For example:

```bash
st-flash build/lib/noisesculptors-core/startup/common/trampoline_h750.bin 0x08000000
```

or

```bash
pyocd load build/lib/noisesculptors-core/startup/common/trampoline_h750.bin --target stm32h750xx --base 0x8000000
```

The trampoline's job is to forward reset to a RAM-resident app by default at
`APP_BASE = 0x24000000` (AXI SRAM on STM32H750).

To be able to "flash" to 0x24000000 memory address, please add the following address region to pyocd: 

```bash
 pyocd/target/builtin/target_STM32H750xx.py
```

```diff
   #DTCM
   RamRegion(   start=0x20000000, length=0x20000,
             is_cachable=False,
             access="rw"),
+  #ram
+  RamRegion(   start=0x24000000, length=0x80000,
+            is_cachable=False,
+            access="rwx"),
   #sram1
   RamRegion(   start=0x30000000, length=0x20000,
             is_powered_on_boot=False),
```

After the trampoline is in place and your application is built with -DRUN=axi, simply load it into RAM:

```bash
pyocd load <your_firmware>.bin --target stm32h750xx --base 0x24000000
```

Alternatively, you can load it without modifying `target_STM32H750xx.py` through 
generic `cortex_m` target:

```bash
pyocd load <your_firmware>.bin --target cortex_m--base 0x24000000
```

---

## Debugging (Example using ST-LINK/V2 debugging probe)

This section shows an example workflow for debugging a bare-metal application on an STM32H7 target using ST-LINK/V2 (SWD) and OpenOCD.

Prerequisites:

An OpenOCD and gdb-multiarch installation available in your system.
A built ELF file (e.g., blink.elf in build directory), containing symbols and the program to flash.
ST-LINK/V2 connected to the target via SWD.

Step 1: Start OpenOCD using the ST-LINK/V2 interface configuration and the STM32H7 target configuration:

```bash
openocd -f /usr/share/openocd/scripts/interface/stlink-v2.cfg \
        -f /usr/share/openocd/scripts/target/stm32h7x.cfg
```

Leave this terminal running. OpenOCD will provide a GDB server interface (by default on port 3333).

Step 2: Start GDB
In a second terminal (or another window), start GDB and load the target ELF file:

```bash
gdb-multiarch <path-to-bare-metal-file>.elf
```
Step 3: Connect GDB to OpenOCD and Flash
Inside the GDB prompt, connect to the OpenOCD server and load the program:

```bash
target extended-remote :3333
load
```
target extended-remote :3333 connects GDB to OpenOCD’s GDB server.
load flashes the ELF contents to the target (and/or sets up the program as required by the OpenOCD configuration).

---

## Directory Layout

```
drivers/        -> device drivers (e.g., SSD1315 OLED)
examples/       -> standalone demo programs
include/        -> register headers
ldscripts/      -> linker scripts for Flash, AXI, DTCM
lib/            -> bundled libraries (noisesculptors-core, UGUI, printf)
src/            -> core modules (init, NVIC, syscall, etc.)
startup/        -> startup code and vector tables
tools/          -> various tools, such as pll configuration .py script
userio/         -> I/O user code
```

---

## Toolchain

Requires `arm-none-eabi-gcc` and CMake:

```bash
sudo apt install gcc-arm-none-eabi cmake
```

---

## License

© 2025-2026 Noise Sculptors

External libraries retain their respective licenses.

