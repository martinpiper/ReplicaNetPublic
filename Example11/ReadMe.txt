Example 11

This example requires a machine with internet access.

This example shows:
How to use the RNLobby services to advertise a ReplicaNet session.
How to use the RNLobby services to search for and join a ReplicaNet session.

Run one instance of Example11, wait about ten seconds and run another instance.
The second instance will connect to the first instance using the NATUDP Transport protocol, which internally uses the NATResolver.
After waiting a further ten seconds start another instance of Example11.
Now after about 30 seconds the first instance will quit and one of the other instances will become the new master of the session. The new master will advertise the session. Running another instance will connect to the session.
This demonstrates NAT resolving with host migration.

Available project build configurations:
ReleaseMT
DebugMT
