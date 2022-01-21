# EmbeddedFramework
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

examples$(DS)kl46z_blinky: Simple blinking example

examples$(DS)se2_examples$(DS)kl46z_mma8451_basic: Read acceleration and toggle leds

examples$(DS)se2_examples$(DS)kl46z_mma8451_int: Configure mma8451 for interruo on acc ready

If you want to create your own project, create the folder project and copy some project into this folder.
