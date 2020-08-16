Example 8

This example uses the ideas in Example2 but expands it to show how to call some master
network functions on a selection of sessionIDs.
This example also creates a ReplicaNet derived class called MyReplicaNet to enable the
example to detect other session joiners.

ReplicaNet::CallBackAllocate() has also been implemented to demonstrate how to use
inheritance with network capable classes. In this case an extra object class ExtendedObject
inherits from Test1 and has a unique object classID implemented by StaticGetClassID().

To run this example:
1) Run the example once to begin with and the example should start. The master session
will create an object and publish it and then wait for three other session to join.

2) Now run the example another three times.

What you will see is in Test1.cpp: void Test1::RunTest(const int numSessionIDs,const int *sessionIDs)
Two session IDs are supplied using DataBlock_Function::SetSessionIDFilter() or the
macro NOMINATED_REPLICAS_FUNCTION_CALL() and when the function is called only the two
nominated sessions will get the function call. Also the other session will not receive
a function call because it was not nominated.


Available project build configurations:
ReleaseMT
DebugMT
