/* run entirely from AXI SRAM, stack in DTCM */
ENTRY(Reset_Handler)

INCLUDE memory_map.ld

/* Stack at top of DTCM */
_estack = ORIGIN(DTCM) + LENGTH(DTCM);
PROVIDE(__StackTop = _estack);
PROVIDE(__StackLimit = ORIGIN(DTCM));

SECTIONS
{
  /* Vector table at start of AXI, keep 128-byte alignment for VTOR */
  .isr_vector ORIGIN(AXI) :
  {
    . = ALIGN(128);
    KEEP(*(.isr_vector))
    . = ALIGN(128);
  } > AXI

  /* Code + const in AXI */
  .text :
  {
    . = ALIGN(8);
    *(.text*)
    *(.glue_7*) *(.glue_7t*)

    /* C runtime bits often end up here */
    KEEP(*(.init))
    KEEP(*(.fini))

    /* Read-only data */
    *(.rodata*)   /* (single occurrence is enough) */

    /* Unwind info (if used) */
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    *(.ARM.exidx*)
    
    . = ALIGN(8);
  } > AXI

  /* C++ static constructors/destructors (if any) */
  .preinit_array :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN(__preinit_array_start = .);
    KEEP(*(.preinit_array*))
    PROVIDE_HIDDEN(__preinit_array_end = .);
  } > AXI

  .init_array :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN(__init_array_start = .);
    KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
    KEEP(*(.init_array*))
    PROVIDE_HIDDEN(__init_array_end = .);
  } > AXI

  .fini_array :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN(__fini_array_start = .);
    KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*)))
    KEEP(*(.fini_array*))
    PROVIDE_HIDDEN(__fini_array_end = .);
  } > AXI

  /* Data lives in AXI already — no LMA/ROM copy */
  .data :
  {
    . = ALIGN(8);
    __data_start__ = .;
    *(.data*)
    __data_end__ = .;
    . = ALIGN(8);
  } > AXI

  /* Zero-init */
  .bss (NOLOAD) :
  {
    . = ALIGN(8);
    __bss_start__ = .;
    *(.bss*) *(COMMON)
    __bss_end__ = .;
    . = ALIGN(8);
  } > AXI

  /* Optional heap start marker */
  PROVIDE(__heap_start__ = .);
}

/* Handy symbols for startup */
PROVIDE(__VectorTable = ORIGIN(AXI));

