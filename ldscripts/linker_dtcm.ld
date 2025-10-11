/* run entirely from DTCM, stack in DTCM */
ENTRY(Reset_Handler)

INCLUDE memory_map.ld

/* Stack at top of DTCM */
_estack = ORIGIN(DTCM) + LENGTH(DTCM);
PROVIDE(__StackTop   = _estack);
PROVIDE(__StackLimit = ORIGIN(DTCM));

SECTIONS
{
  /* Vector table at start of DTCM so VTOR can point here */
  .isr_vector ORIGIN(DTCM) :
  {
    . = ALIGN(128);           /* M7 VTOR alignment */
    KEEP(*(.isr_vector))
    . = ALIGN(128);
  } > DTCM

  /* Code + read-only data */
  .text :
  {
    . = ALIGN(8);
    *(.text*)
    *(.glue_7*) *(.glue_7t*)

    /* C runtime entry/exit (if present) */
    KEEP(*(.init))
    KEEP(*(.fini))

    /* Read-only data */
    *(.rodata*)

    /* Unwind info (harmless if unused) */
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    *(.ARM.exidx*)

    . = ALIGN(8);
  } > DTCM

  /* C++ static constructors/destructors (if any) */
  .preinit_array :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN(__preinit_array_start = .);
    KEEP(*(.preinit_array*))
    PROVIDE_HIDDEN(__preinit_array_end   = .);
  } > DTCM

  .init_array :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN(__init_array_start = .);
    KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
    KEEP(*(.init_array*))
    PROVIDE_HIDDEN(__init_array_end   = .);
  } > DTCM

  .fini_array :
  {
    . = ALIGN(8);
    PROVIDE_HIDDEN(__fini_array_start = .);
    KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*)))
    KEEP(*(.fini_array*))
    PROVIDE_HIDDEN(__fini_array_end   = .);
  } > DTCM

  /* Initialized data — running from RAM, so no ROM/LMA */
  .data :
  {
    . = ALIGN(8);
    __data_start__ = .;
    *(.data*)
    __data_end__ = .;
    . = ALIGN(8);
  } > DTCM

  /* Zero-init */
  .bss (NOLOAD) :
  {
    . = ALIGN(8);
    __bss_start__ = .;
    *(.bss*) *(COMMON)
    __bss_end__ = .;
    . = ALIGN(8);
  } > DTCM

  /* Optional heap: place it in AXI if you need space/speed balance */
  /* Comment this out or move it if you prefer a DTCM heap instead. */
  .heap (NOLOAD) :
  {
    PROVIDE(__heap_start__ = ORIGIN(AXI));
  } > AXI
}

/* Symbols the startup uses */
PROVIDE(__VectorTable = ORIGIN(DTCM));  /* Vector table base (0x20000000) */

