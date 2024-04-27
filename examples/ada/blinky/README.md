# Ada example #
This is a very simple example to compile a project in C with a file written in Ada. It is only for educational purposes. The Ada runtime is ZFP, so we cannot use tasking in Ada.

"If you want to compile and run this example, you need to:

1. Install gprbuild by running:
sudo apt install gprbuild
sudo apt install gnat

3. Navigate to the directory of the EmbeddedFramework:
cd path/to/EmbeddedFramework

4. Build the library with the following command:
make BOARD=pcSim PROJECT_PATH=examples/ada/blinky ada_build

5. Compile the project using:
make BOARD=pcSim PROJECT_PATH=examples/ada/blinky

6. Finally, run the project with:
make BOARD=pcSim PROJECT_PATH=examples/ada/blinky run

Enjoy!
