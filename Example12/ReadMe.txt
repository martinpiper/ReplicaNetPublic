Example 12

This example requires a machine with internet access.

This demonstration shows:
How to use the StatClient class from RNLobby to update client's persistent data.
Inform the statistics server of values from a client session.
Write a stat server script to accumulate these client values into the client's persistent data.
Use C++ with the simple script compiler and the stat server.
Verify the client's persistent data.


The Example12Script.txt file is compiled with RNSimpleScriptCompiler.exe
To compile Example12Script from the Example12 directory:
..\ReplicaNetPublic\RNSimpleScriptCompiler\RNSimpleScriptCompiler.exe -O1 -vO Example12Script.txt
It's possible to check the script with:
..\ReplicaNetPublic\RNSimpleScriptCompiler\RNSimpleScriptDebugger.exe -cls -dump -cycles -source Example12Script.bin 0 Example12Script.bnd
Note it should take around 128 cycles to complete.
If the "-O1" is not used for the compiler then the debugger will report the script takes more cycles to execute, as expected.



Example12Script2.cpp is an example of how to use C++ with the StatServer.
This C++ will be compiled with the simple script compiler after converting to GCC ASM ARM code.
Install the GCC ARM embedded compiler is installed from https://launchpad.net/gcc-arm-embedded
This example uses version 4.7-2012-q4-major from:
https://launchpad.net/gcc-arm-embedded/4.7/4.7-2012-q4-major/+download/gcc-arm-none-eabi-4_7-2012q4-20121208-win32.exe
To compile to a GCC ARM ASM file from the Example12 directory:
(Use a suitable path to arm-none-eabi-gcc.exe for the compiler version that was downloaded.)
"C:\Program Files (x86)\GNU Tools ARM Embedded\4.7 2012q4\bin\arm-none-eabi-gcc.exe" -O3 -S -mcpu=arm2 -fno-rtti -fno-exceptions -I../ReplicaNetPublic/Includes Example12Script2.cpp
Next to compile the GCC ARM output file Example12Script2.s
..\ReplicaNetPublic\RNSimpleScriptCompiler\RNSimpleScriptCompiler.exe -O1 -vO -o Example12Script2.bin BootStatGCC Stat ARMGCCstdlib Example12Script2.s
Note that the output file is set along with the GCC stat related includes.
The order of the includes list "BootStatGCC Stat ARMGCCstdlib" is important because it effectively compiles these first into the simple script memory.
The file Example12Script2.s is after the standard includes list.


The compiled outputs of the two scripts are already located on the stat server.
To maintain the integrity of this example the server scripts cannot be updated by this user and the user's password and email address cannot be changed.
Using an account with relevant access it is possible to use the RNAdminTool to upload the compiled binary files.
For example:
..\ReplicaNetPublic\RNAdminTool\RNAdminTool.exe 12 Example12Key "a user name" "user's password" -ud 1 Example12Script.bin
..\ReplicaNetPublic\RNAdminTool\RNAdminTool.exe 12 Example12Key "a user name" "user's password" -ud 2 Example12Script2.bin
Note the script ID changes for the two scripts.


Available project build configurations:
ReleaseMT
DebugMT
