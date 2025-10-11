/* program executes from Flash (0x08000000), data+stack in DTCM */
ENTRY(Reset_Handler)

INCLUDE memory_map.ld

/* Stack in DTCM */
_estack = ORIGIN(DTCM) + LENGTH(DTCM);
PROVIDE(__StackTop   = _estack);
PROVIDE(__StackLimit = ORIGIN(DTCM));

SECTIONS
{
  /* Vector table must be at Flash base so bootloader can find it */
  .isr_vector ORIGIN(FLASH) :
  {
    . = ALIGN(128);
    KEEP(*(.isr_vector))
    . = ALIGN(128);
  } > FLASH

  /* Code and const data live in Flash */
  .text :
  {
    . = ALIGN(8);
    *(.text*)
    *(.glue_7*) *(.glue_7t*)
    *(.init) *(.fini)
    *(.rodata*)
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    *(.ARM.exidx*)
    . = ALIGN(8);
  } > FLASH

  /* Initialized data section:
     LMA = Flash (load), VMA = DTCM (run)
     The startup code will copy it. */
  .data :
  {
    . = ALIGN(8);
    __data_start__ = .;
    *(.data*)
    __data_end__ = .;
    . = ALIGN(8);
  } > DTCM AT > FLASH

  /* Define copy source (LMA) symbol for startup */
  __data_load_start__ = LOADADDR(.data);

  /* Zero-init */
  .bss (NOLOAD) :
  {
    . = ALIGN(8);
    __bss_start__ = .;
    *(.bss*) *(COMMON)
    __bss_end__ = .;
    . = ALIGN(8);
  } > DTCM

  /* Optional heap (can place in AXI if large buffers needed) */
  PROVIDE(__heap_start__ = ORIGIN(AXI));
}

/* Symbols used by startup */
PROVIDE(__VectorTable = ORIGIN(FLASH));

