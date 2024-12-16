# 8085-Processor
When you run the code in the compiler, you will see the line _STUDENT_85, and the code will begin processing using the memory location specified by the variable M. Once you enter a value for the location, you will see the contents of that memory location displayed as M(your location value): (previous data) - (new data that you can enter). To add a value to a different memory location, press the 'Enter' key. To stop the program, enter 'EF'. To close the program, enter '$'. To execute the program, enter G(location value). To view the contents of the registers, enter R and press the 'Enter' key.

Commands
The program supports the following commands:

'M' or 'm': Modify memory contents
'R' or 'r': Modify register contents
'G' or 'g': Execute the program starting from a particular memory location
Modify Memory Contents
To modify the contents of memory, enter 'M' or 'm' followed by the memory location you want to modify. The program will then prompt you to enter the new value for that memory location.

If the memory location you enter is invalid, the program will report an error and prompt you to try again.

Modify Register Contents
To modify the contents of registers, enter 'R' or 'r' followed by the register you want to modify. The program will then prompt you to enter the new value for that register.

If the register you enter is invalid, the program will report an error and prompt you to try again.

Execute Program
To execute the program, enter 'G' or 'g' followed by the memory location you want to start executing from. The program will then execute the program starting from that memory location.

If the memory location you enter is invalid, the program will report an error and prompt you to try again.
