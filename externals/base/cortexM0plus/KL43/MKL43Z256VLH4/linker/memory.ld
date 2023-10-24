/*
 * GENERATED FILE - DO NOT EDIT
 * Copyright (c) 2008-2013 Code Red Technologies Ltd,
 * Copyright 2015, 2018-2019 NXP
 * (c) NXP Semiconductors 2013-2023
 * Generated linker script file for MKL43Z256xxx4
 * Created from memory.ldt by FMCreateLinkMemory
 * Using Freemarker v2.3.30
 * MCUXpresso IDE v11.7.0 [Build 9198] [2023-01-17] on 17 oct. 2023 20:36:40
 */

MEMORY
{
  /* Define each memory region */
  PROGRAM_FLASH (rx) : ORIGIN = 0x0, LENGTH = 0x40000 /* 256K bytes (alias Flash) */
  SRAM (rwx) : ORIGIN = 0x1fffe000, LENGTH = 0x8000 /* 32K bytes (alias RAM) */
}

  /* Define a symbol for the top of each memory region */
  __base_PROGRAM_FLASH = 0x0  ; /* PROGRAM_FLASH */
  __base_Flash = 0x0 ; /* Flash */
  __top_PROGRAM_FLASH = 0x0 + 0x40000 ; /* 256K bytes */
  __top_Flash = 0x0 + 0x40000 ; /* 256K bytes */
  __base_SRAM = 0x1fffe000  ; /* SRAM */
  __base_RAM = 0x1fffe000 ; /* RAM */
  __top_SRAM = 0x1fffe000 + 0x8000 ; /* 32K bytes */
  __top_RAM = 0x1fffe000 + 0x8000 ; /* 32K bytes */
