# <a name="HowToUse"></a>Inception

Inception is a framework for advanced dynamic analyses of embedded systems firmware programs.
It's split in three main parts which are :
 * Inception-debugger : allow near real-time communication with device under test
 * Inception-Analyzer : execute IR code in a symbolic virtual machine
 * Inception-Compiler : compile C/CPPP/S source files in LLVM IR

This project is used to send command to the Inception-debugger in order to communicate with
the real hardware to use real hardware peripherals or get hardware interrupts.

# <a name="HowToUse"></a>How to use

This project supports 3 execution mode :
 * As a C library where the entry point is src/superspeedjtag.cpp
 * With a python interactive mode, see src/interactive.py
 * As a stand-alone program : ./Debug/inception 


# <a name="Python"></a>Inception

You can use rlwrap if installed to get a real Python console.
``$ rlwrap python3.5 interactive.py``

# <a name="Stand Alone"></a>Inception


Different arguments are supported : 
 * -Debug, verbose information
 * -Debug2, more verbose information
 * -info, retrieve the IDCODE of the MPU after the reset command
 * --benchmark_io, test READ/WRITE commands 100 000 times with random value
 * -Some other tests are available, check the code ;-)
