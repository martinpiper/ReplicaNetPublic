Demo 1

Run GameLobby.exe to start the integrated user and statistics tracking demonstration.
After creating an account or logging in to an existing account you may use the toolbar buttons.
The Home button will display the statistics for this demonstration using ASP generated reports.

The "Host game" button will start Demo1.exe.

When the Demo1.exe demonstration loads you can choose to join or create a game.
Pilot the planes using the cursor keys and CTRL to fire. 'S' and 'D' will slow down and speed up.
Each plane, projectile and enemy dolphin is a networked ReplicaObject. Even though twenty projectiles can be fired every second the network bandwidth is kept low.

This demonstration shows: 

How the distributed network system can handle a player joining or leaving at any time.
When a master session leaves the network game then a new master session is quickly found.
Fault tolerant enemies since if a session disconnects then the dolphins allocated to that session are distributed to other sessions. Each new player creates five new dolphins and these dolphin models are hosted on the local machine.

When displaying the network joining dialog the user can also set various parameters for the network local session.
For example the first column of tick boxes are these:

"Can Spider"		=	Enables outgoing spider connections for this session
"Can be master"		=	If a sesion fails this session can take over as the master session
"Accept objects"	=	When objects migrate due to a fault this session can accept objects

The second column allows the session to decide what to do with the objects when a session fatal error happens.
A fatal error happens when the session cannot become a master and all other possible sessions leave the session.

"Migrate"			=	Allow this session to take over all replica objects
"Delete"			=	Delete all replica objects that are not master objects
"Leave"				=	Leave the replica objects.

Network emulation can be enabled or disabled for all packets sent from this node using the "Enable" check box.
The packet latency and jitter times are in milliseconds. The packet loss is expressed as a percentage. The values are clamped to maximum and minimum ranges.
