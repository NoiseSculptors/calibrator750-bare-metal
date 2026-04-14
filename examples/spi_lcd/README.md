
# ST7789T3 320x240 (RGB565) Example

This is an example of using the **ST7789T3 320x240** display in **RGB565** (16-bit) mode.

## Notes about the current implementation

- Display data is transmitted using **DMA**.
- However, **every DMA transaction is followed by a delay**, so the overall behavior is essentially **blocking**.
- To achieve a more truly **non-blocking** flow, an approach is to schedule the next action using a **SysTick-based service** (or similar timer/event mechanism).

## Non-blocking approach

An example of how to do that using a **SysTick service** is provided in the **`userio`** directory (for an OLED display).

## What to look at

- How the ST7789T3 initialization commands are sent
- How frame/pixel data is pushed to the display in **RGB565**
- Where the DMA completion / delays are handled
- Compare with the `userio` example and non-blocking updates for OLED display (SysTick-driven)

