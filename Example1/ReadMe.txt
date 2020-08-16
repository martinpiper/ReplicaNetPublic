Example 1

Original Microsoft DX8 shadow volume demonstration expanded to use ReplicaNet libraries and make a
multiplayer game example.
Each plane is now a C++ object that can be used with the ReplicaNet library.
Each Plane object is also fault tolerant so if a session is quit then that object is transfered
to another session.
When each plane is rendered if the plane object is owned by the session, i.e. if it is a master
ReplicaObject, then the object is rendered double size.

For the graph at the bottom of the screen the red line indicates sent data, the green line indicates data received.
When the large master airplane is near the camera the sent data rate increases, when it is further away the sent
data rate decreases.

Pressing Y will cause each master plane (owned by the session) to call a network function signalled
by the plane pointing vertically upwards.

Pressing U will cause each master plane (owned by the session) to call Unpublish() causing the objects
to be unlinked from the ReplicaNet object lists. The planes on all nodes will continue to move as master
instances. Moving the window or pressing Y will show the unlinked master objects being out of sync
with the network session since they are not published to the session.

Pressing Q/A will increase/decrease emulated packet latency.
Pressing W/S will increase/decrease emulated packet loss.

Look in MainGame.cpp for the main block of code that used ReplicaNet. The other folders in this project are:


Resource Files:			The resource files used for the windows and dialogs
Common:					A common DX8 interface from the MS DX8 SDK
Media:					The plane object and texture files
ROLFiles:				The ROL files for each object used in the game
GameObjects:			The classes for each type of game object used in the example
CompiledGameObjects:	The compiled files produced by the ROL compiler
GameClass:				The code that deals with the DX8 common interface


The files in the ROLFiles directory are automatically compiled with custom build rules setup for this folder.
The compiler takes an input file and two output files. For example RNROLCompiler.exe <input.rol> <output.cpp> <output.h>
The output cpp and h files can then be included in a project.
To use the compiler with MS VC++ you can use custom build rules per file or folder.
Example:
To build _RO_Object.rol and include this in the project use the custom build rule:
..\ReplicaNetPublic\RNROLCompiler\bin\rnrolcompiler.exe $(InputName).rol $(InputName).cpp $(InputName).h
And for the file output box use:
$(InputName).cpp
$(InputName).h
For MS VC++ 6.0 The carriage return between the cpp and h file is important otherwise MSVC will get confused with
build dependency and may rebuild the output files unnecessarily.
Note: This assumes the RNROLCompiler directory is one directory 'below' your project build directory and for this
example this is correct.

A graceful shutdown of the ReplicaNet library is shown in the CMyD3DApplication::FinalCleanup()
function found in the GameClass.cpp file.

Available project build configurations:
ReleaseMT
DebugMT
