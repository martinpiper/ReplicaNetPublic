Example 6

This example is a quick demonstration that creates ReplicaObject objects and uses extended DataBlocks

This demonstration shows: 

How to extend DataBlocks with your own class.
How to expand the ROL files to allow an extended DataBlock to be registered using the ROL.
How to use the automatic DataBlock registering function to allow an extended DataBlock to be registered without using the ROL extensions.
How to create a class that contains compound DataBlock members such as a 3D vector type class called NetworkVector3.
How to create a DataBlock derived class that updates an STL container.
Highlights thread safety issues regarding multithreaded use of ReplicaObject derived classes. Look for "THREAD SAFETY:" comments.

Available project build configurations:
ReleaseMT
DebugMT
