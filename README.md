# EmbeddedFramework
# How to use:

Import this project in you Eclipse IDE (for NXP and FLS microcontroller use MCUXpresso)

File → Import → C/C++ → Existing Code as Makefile Project 

Next

Browse and select folder: EmbeddedFramework

In Toolchain for Indexer Settings select: NXP MCU Tools

Finish

# How to compile

Copy Makefile.example to Makefile.mine

Edit your Makefile.mine for compiling the project desired:

examples$(DS)kl46z_blinky

examples$(DS)se2_examples$(DS)kl46z_mma8451_basic

examples$(DS)se2_examples$(DS)kl46z_mma8451_int

If you want to create your own project, create the folder project and copy some project into this folder.
