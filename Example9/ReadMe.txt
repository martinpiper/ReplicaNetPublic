Example 9

This example shows how to use the ReplicaObject session propagation filter mechanism.
The objects in this example deliberately do not use distance based extrapolation to
aid in making the example clearer to follow.

This example gives a rough tutorial on how server controlled massively multiplayer
game objects can choose what clients to propagate to.



To run this example:
1) Run the example once to begin with and create a session. You should see four planes
moving in a circle. This session can be thought of as a server node in a game running
several enemy objects. Each plane scans the session for dolphin objects and if the
dolphin object is within a certain distance then the plane will use
ReplicaObject::SessionPropagationAddSession() with the session ID of the dolphin. This
has the effect of making the plane objects only update to a certain session when it
meets user defined criteria. When the plane decides that it is far away from a dolphin
it will then call ReplicaObject::SessionPropagationRemoveSession() and the plane will
no longer update to the session controlling the dolphin.


2) Run the example a second time and join the session created in step 1. You should see
a dolphin begin to move in the window. When one of the planes from the server come
in to range of the dolphin then the plane object will display on this node.


Available project build configurations:
ReleaseMT
DebugMT
