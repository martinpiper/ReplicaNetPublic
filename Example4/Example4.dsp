# Microsoft Developer Studio Project File - Name="Example4" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Example4 - Win32 DebugMT
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Example4.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Example4.mak" CFG="Example4 - Win32 DebugMT"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Example4 - Win32 DebugMT" (based on "Win32 (x86) Application")
!MESSAGE "Example4 - Win32 ReleaseMT" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Example4", XEBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Example4 - Win32 DebugMT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Example4___Win32_DebugMT"
# PROP BASE Intermediate_Dir "Example4___Win32_DebugMT"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Example4___Win32_DebugMT"
# PROP Intermediate_Dir "Example4___Win32_DebugMT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I ".\common\include" /I "..\replicanetwork\replicanet\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Common3DApp/Inc" /I "../ReplicaNetPublic/Includes" /D "_DEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /D "_CRT_SECURE_NO_DEPRECATE" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 d3dx8dt.lib replicanetdm.lib d3d8.lib d3dxof.lib dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\replicanetwork\replicanet\lib"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 dinput8.lib d3dx8dt.lib d3d8.lib d3dxof.lib dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "Example4 - Win32 ReleaseMT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Example4___Win32_ReleaseMT"
# PROP BASE Intermediate_Dir "Example4___Win32_ReleaseMT"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Example4___Win32_ReleaseMT"
# PROP Intermediate_Dir "Example4___Win32_ReleaseMT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I ".\common\include" /I "..\replicanetwork\replicanet\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../Common3DApp/Inc" /I "../ReplicaNetPublic/Includes" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /D "_CRT_SECURE_NO_DEPRECATE" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 d3dx8.lib replicanetrm.lib d3d8.lib d3dxof.lib dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:windows /machine:I386 /libpath:"..\replicanetwork\replicanet\lib"
# ADD LINK32 dinput8.lib d3dx8.lib d3d8.lib d3dxof.lib dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:windows /machine:I386

!ENDIF 

# Begin Target

# Name "Example4 - Win32 DebugMT"
# Name "Example4 - Win32 ReleaseMT"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\winmain.rc
# End Source File
# End Group
# Begin Group "Media"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\airplane 2.x"
# End Source File
# Begin Source File

SOURCE=.\bihull.bmp
# End Source File
# Begin Source File

SOURCE=.\cube.x
# End Source File
# Begin Source File

SOURCE=.\directx.ico
# End Source File
# Begin Source File

SOURCE=.\Dolphin1.x
# End Source File
# Begin Source File

SOURCE=.\seafloor.bmp
# End Source File
# Begin Source File

SOURCE=.\seafloor.x
# End Source File
# Begin Source File

SOURCE=.\wings.bmp
# End Source File
# End Group
# Begin Group "ROLFiles"

# PROP Default_Filter "rol"
# Begin Source File

SOURCE=.\_Def_Example1.rol

!IF  "$(CFG)" == "Example4 - Win32 DebugMT"

# Begin Custom Build
InputPath=.\_Def_Example1.rol
InputName=_Def_Example1

BuildCmds= \
	..\ReplicaNetPublic\RNROLCompiler\bin\rnrolcompiler.exe $(InputName).rol $(InputName).cpp $(InputName).h

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Example4 - Win32 ReleaseMT"

# Begin Custom Build
InputPath=.\_Def_Example1.rol
InputName=_Def_Example1

BuildCmds= \
	..\ReplicaNetPublic\RNROLCompiler\bin\rnrolcompiler.exe $(InputName).rol $(InputName).cpp $(InputName).h

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_RO_Camera.rol

!IF  "$(CFG)" == "Example4 - Win32 DebugMT"

# Begin Custom Build
InputPath=.\_RO_Camera.rol
InputName=_RO_Camera

BuildCmds= \
	..\ReplicaNetPublic\RNROLCompiler\bin\rnrolcompiler.exe $(InputName).rol $(InputName).cpp $(InputName).h

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Example4 - Win32 ReleaseMT"

# Begin Custom Build
InputPath=.\_RO_Camera.rol
InputName=_RO_Camera

BuildCmds= \
	..\ReplicaNetPublic\RNROLCompiler\bin\rnrolcompiler.exe $(InputName).rol $(InputName).cpp $(InputName).h

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_RO_Enemy.rol

!IF  "$(CFG)" == "Example4 - Win32 DebugMT"

# Begin Custom Build
InputPath=.\_RO_Enemy.rol
InputName=_RO_Enemy

BuildCmds= \
	..\ReplicaNetPublic\RNROLCompiler\bin\rnrolcompiler.exe $(InputName).rol $(InputName).cpp $(InputName).h

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Example4 - Win32 ReleaseMT"

# Begin Custom Build
InputPath=.\_RO_Enemy.rol
InputName=_RO_Enemy

BuildCmds= \
	..\ReplicaNetPublic\RNROLCompiler\bin\rnrolcompiler.exe $(InputName).rol $(InputName).cpp $(InputName).h

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_RO_Plane.rol

!IF  "$(CFG)" == "Example4 - Win32 DebugMT"

# Begin Custom Build
InputPath=.\_RO_Plane.rol
InputName=_RO_Plane

BuildCmds= \
	..\ReplicaNetPublic\RNROLCompiler\bin\rnrolcompiler.exe $(InputName).rol $(InputName).cpp $(InputName).h

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Example4 - Win32 ReleaseMT"

# Begin Custom Build
InputPath=.\_RO_Plane.rol
InputName=_RO_Plane

BuildCmds= \
	..\ReplicaNetPublic\RNROLCompiler\bin\rnrolcompiler.exe $(InputName).rol $(InputName).cpp $(InputName).h

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_RO_Projectile.rol

!IF  "$(CFG)" == "Example4 - Win32 DebugMT"

# Begin Custom Build
InputPath=.\_RO_Projectile.rol
InputName=_RO_Projectile

BuildCmds= \
	..\ReplicaNetPublic\RNROLCompiler\bin\rnrolcompiler.exe $(InputName).rol $(InputName).cpp $(InputName).h

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Example4 - Win32 ReleaseMT"

# Begin Custom Build
InputPath=.\_RO_Projectile.rol
InputName=_RO_Projectile

BuildCmds= \
	..\ReplicaNetPublic\RNROLCompiler\bin\rnrolcompiler.exe $(InputName).rol $(InputName).cpp $(InputName).h

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "GameObjects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Camera.h
# End Source File
# Begin Source File

SOURCE=.\Enemy.cpp
# End Source File
# Begin Source File

SOURCE=.\Enemy.h
# End Source File
# Begin Source File

SOURCE=.\Plane.cpp
# End Source File
# Begin Source File

SOURCE=.\Plane.h
# End Source File
# Begin Source File

SOURCE=.\Projectile.cpp
# End Source File
# Begin Source File

SOURCE=.\Projectile.h
# End Source File
# End Group
# Begin Group "CompiledGameObjects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\_Def_Example1.cpp
# End Source File
# Begin Source File

SOURCE=.\_Def_Example1.h
# End Source File
# Begin Source File

SOURCE=.\_RO_Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\_RO_Camera.h
# End Source File
# Begin Source File

SOURCE=.\_RO_Enemy.cpp
# End Source File
# Begin Source File

SOURCE=.\_RO_Enemy.h
# End Source File
# Begin Source File

SOURCE=.\_RO_Plane.cpp
# End Source File
# Begin Source File

SOURCE=.\_RO_Plane.h
# End Source File
# Begin Source File

SOURCE=.\_RO_Projectile.cpp
# End Source File
# Begin Source File

SOURCE=.\_RO_Projectile.h
# End Source File
# End Group
# Begin Group "GameClass"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DIInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\DIInterface.h
# End Source File
# Begin Source File

SOURCE=.\External.h
# End Source File
# Begin Source File

SOURCE=.\GameClass.cpp
# End Source File
# Begin Source File

SOURCE=.\GameClass.h
# End Source File
# Begin Source File

SOURCE=.\GameDatabase.cpp
# End Source File
# Begin Source File

SOURCE=.\GameDatabase.h
# End Source File
# Begin Source File

SOURCE=.\GameObject.cpp
# End Source File
# Begin Source File

SOURCE=.\GameObject.h
# End Source File
# Begin Source File

SOURCE=.\MyReplicaNet.cpp
# End Source File
# Begin Source File

SOURCE=.\MyReplicaNet.h
# End Source File
# Begin Source File

SOURCE=.\Network.cpp
# End Source File
# Begin Source File

SOURCE=.\Network.h
# End Source File
# Begin Source File

SOURCE=.\Shadow.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\ReplicaNetPublic\AmalgamatedReplicaNet.cpp
# ADD CPP /YX
# End Source File
# Begin Source File

SOURCE=..\Example1\BandwidthGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\MainGame.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
