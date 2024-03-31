# EmbeddedFramework # [![CI](https://github.com/EmbeddedFramework/EmbeddedFramework/workflows/CI/badge.svg)](https://github.com/EmbeddedFramework/EmbeddedFramework/actions)

# How to use:

Import this project in your Eclipse IDE (for NXP and FLS microcontroller use MCUXpresso)

File → Import → C/C++ → Existing Code as Makefile Project 

Next

Browse and select folder: EmbeddedFramework

In Toolchain for Indexer Settings select: NXP MCU Tools

Finish

# How to compile

Copy Makefile.example to Makefile.mine

Edit your Makefile.mine for compiling the project desired:

examples$(DS)blinky: Simple blinking example (runs on cpSim, frdmkl43, frdmkl46, nucleoF767ZI)

examples$(DS)timeStat: runtime statics on FreeRTOS (runs on frdmkl43, frdmkl46, nucleoF767ZI)

examples$(DS)timers: software timers example (runs on frdmkl43, frdmkl46, nucleoF767ZI)

If you want to create your own project, create the folder project and copy some project into this folder.
