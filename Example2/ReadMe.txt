Example 2

This example shows:
How to define functions that can be called on other network objects.
How to add custom types so that they can be used for networked functions.
How to use the ROL to cause an object to recover from a fault.
How to shutdown ReplicaNet and ensure all internal threads are stopped and all memory is free.

This example is kept deliberately small so you can see what is really going on.
The demonstration starts by searching for a network session and if one is not found then
one is created using the defaults chosen by ReplicaNet.
The networked class member functions are called from the Test1::RunTest() function.
The Test1 object is also able to recover from a fault where a session quits. This is made
possible because the object returns 'true' from the ApproveFault() virtual function.


Available project build configurations:
ReleaseMT
DebugMT
