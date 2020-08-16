Example 4

When the demonstration loads you can choose to join or create a game.
Pilot the planes using the cursor keys and CTRL to fire. 'S' and 'D' will slow down and speed up.
Each plane, projectile and enemy dolphin is a networked ReplicaObject. Even though twenty projectiles can be fired
every second the network bandwidth is kept low.

This demonstration shows: 

How the distributed network system can handle a player joining or leaving at any time.
When a master session leaves the network game then a new master session is quickly found.
Fault tolerant enemies since if a session disconnects then the dolphins allocated to that session are distributed to
other sessions. Each new player creates five new dolphins and these dolphin models are hosted on the local machine.
Game objects are demonstrated by using the GameObject class (GameObject.cpp/h).
This shows how to use inheritance in your game object classes and to interface the ReplicaObjects in to a game.
How to use the ReplicaObject::GetOpaquePointer() function to enable common code to be used in the base GameObject class.
Using ReplicaNet::SetDataBlockUpdatePolicy() and ReplicaNet::ProcessDataBlockUpdate() to synchronise the DataBlock
updates with the game update loop CMyD3DApplication::FrameMove().
How to use ReplicaNet::BeginSessionRecord() and ReplicaNet::BeginSessionPlayback() to record and playback session
recordings.
Compressing and decompressing the recording file. (MyReplicaNet.cpp/h)
Network::Network_Shutdown() demonstrates a graceful shutdown of the library.

The dialog box

When displaying the network joining dialog the user can also set various parameters for the network local session.
For example the first column of tick boxes are these:

"Can Spider"		=	Enables outgoing spider connections for this session.
"Can be master"		=	If a session fails this session can take over as the master session.
"Accept objects"	=	When objects migrate due to a fault this session can accept objects.
"Dedicated"			=	Starts a dedicated session where there is no player, only enemy objects.

The second column allows the session to decide what to do with the objects when a session fatal error happens.
A fatal error happens when the session cannot become a master and all other possible sessions leave the session.

"Migrate"			=	Allow this session to take over all replica objects.
"Delete"			=	Delete all replica objects that are not master objects.
"Leave"				=	Leave the replica objects.
"Render"			=	Toggles the rendering of 3D models.

The "Recording filename" area and "Record"/"Playback" tick boxes allow the session record/playback functionality to
be tested.

To record a session:
Ticking the "Record" box will enable the "Browse" button. Press the button and using the "Save As" window choose a
file to save to.
Search for a game or create a new game as normal.
To stop recording the session exit the application.

To playback a recording:
Ticking the "Playback" box will enable the "Browse" button. Press the button and using the "Open" window choose a
recording file to playback.
The recording will start playing back. While playing a recording the keys 1 to 5 change the speed of the playback.
Key 9 will skip forward 10 seconds and 0 (zero) will pause the playback.


Available project build configurations:
ReleaseMT
DebugMT
