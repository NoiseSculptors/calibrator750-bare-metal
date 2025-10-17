
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

- Simple **Makefile-based build system**
- Full **startup code** and **linker scripts** for flash, AXI SRAM, and DTCM execution
- Modular source layout (`src/`, `drivers/`, `examples/`, `include/`)
- External lightweight libraries:
  - [UGUI](https://github.com/achimdoebler/UGUI) (tiny embedded GUI)
  - [printf](https://github.com/mpaland/printf) (minimal `printf` replacement)
- Ready-to-run **examples** for timers, UART, I²C OLED display (SSD1315), and more

Everything builds directly with `arm-none-eabi-gcc` (Debian-stable tested), no
external toolchain integration or CMake needed.

---

## Build Configuration

Select the desired memory region in `makefile.conf`:

```make
# ---- Build profile selection ----
# Choose one: flash (0x08000000) | axi (0x24000000) | dtcm (0x20000000)
RUN = axi
````
| Profile | Memory region | Notes                            |
| ------- | ------------- | -------------------------------- |
| `flash` | 0x08000000    | Run from internal Flash (128K)   |
| `axi`   | 0x24000000    | Run from AXI SRAM (fast, 518K)   |
| `dtcm`  | 0x20000000    | Run from DTCM (very fast, 128K)  |
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
  -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi-gcc.cmake \
  -DBOARD=calibrator750 \
  -DRUN=flash \
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
[Calibrator750-trampoline](https://github.com/NoiseSculptors/calibrator750-trampoline) once to flash memory at `0x08000000`:

For example:

```bash
st-flash write trampoline.bin 0x08000000
```

or

```bash
pyocd load trampoline.bin --target stm32h750xx --base 0x08000000
```

The trampoline’s job is to forward reset to a RAM-resident app — by default at
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

After the trampoline is in place, simply load your real application into RAM:

```bash
pyocd load <your_firmware>.bin --target stm32h750xx --base 0x24000000
```

---

## Directory Layout

```
include/        → register headers
src/            → core modules (init, NVIC, syscall, etc.)
drivers/        → device drivers (e.g., SSD1315 OLED)
examples/       → standalone demo programs
lib/            → bundled libraries (noisesculptors-core, UGUI, printf)
ldscripts/      → linker scripts for Flash, AXI, DTCM
startup/        → startup code and vector tables
```

---

## Toolchain

Requires `arm-none-eabi-gcc` and standard GNU make:

```bash
sudo apt install gcc-arm-none-eabi make
```

---

## License

© 2025 Noise Sculptors with help of ChatGPT-5

External libraries retain their respective licenses.

