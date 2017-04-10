This is a project for serial communication between the VESC and Embedded Linux Devices, such as
Beaglebone Black and Raspberry Pi. It will also work with any Linux Computer. The provided 
examples in main.c and main.cpp demonstrate how to use UART1 on Beaglebone Black to 
communicate with the VESC. 

These serial APIs are based on the original stm32f4 UART project by vedderb:
https://github.com/vedderb/bldc_uart_comm_stm32f4_discovery 

He has written a tutorial on how to use and port this code to other platforms here:
http://vedder.se/2015/10/communicating-with-the-vesc-using-uart/

I have included a c++ wrapper that condenses the amount of code
required to perform control tasks over serial. To use the wrapper, modify
motortypes.h to include paramaters relevant to the specific motor and
settings you would like to use.

Examples have been provided for both API testing and Wrapper testing
in main.c and main.cpp. 

main.c demonstrates how to use the original
APIs from vedderb to do control over serial. main.cpp demonstrates how to
use the wrapper that I have implemented for serial control.

main.cpp must be compiled with -lrt flag. Timers are used in main.cpp to implement
a state-machine for receiving data. These timers require the -lrt compiler flag.
Linker errors will occur without -lrt. The rx state-machine in main.cpp only runs when the user 
requests data, but it could be implemented inside a thread to read continuously if desired.

Serial APIs are implemented in C, but can be compiled with either C or C++ compilers. 
The wrapper can only be compiled using C++. You can use the APIs in C
without the wrapper.

To use this library with a linux device other than Beaglebone Black, 
change the serial port passed to the init function while initializing serial interface.

If using Beaglebone Black, enable UART1 in /boot/uEnv.txt. Make sure to use 
the correct line for your version of the Linux Kernel.

