# Microsoft Developer Studio Project File - Name="Example6" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Example6 - Win32 DebugMT
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Example6.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Example6.mak" CFG="Example6 - Win32 DebugMT"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Example6 - Win32 DebugMT" (based on "Win32 (x86) Console Application")
!MESSAGE "Example6 - Win32 ReleaseMT" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Example6 - Win32 DebugMT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Example6___Win32_DebugMT"
# PROP BASE Intermediate_Dir "Example6___Win32_DebugMT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Example6___Win32_DebugMT"
# PROP Intermediate_Dir "Example6___Win32_DebugMT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../ReplicaNetPublic/Includes" /D "_DEBUG" /D "_CONSOLE" /D "WIN32" /D "_MBCS" /D "_CRT_SECURE_NO_DEPRECATE" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ReplicaNetDM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"../ReplicaNetPublic/ReplicaNet/lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "Example6 - Win32 ReleaseMT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Example6___Win32_ReleaseMT"
# PROP BASE Intermediate_Dir "Example6___Win32_ReleaseMT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Example6___Win32_ReleaseMT"
# PROP Intermediate_Dir "Example6___Win32_ReleaseMT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../ReplicaNetPublic/Includes" /D "NDEBUG" /D "_CONSOLE" /D "WIN32" /D "_MBCS" /D "_CRT_SECURE_NO_DEPRECATE" /FR /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ReplicaNetRM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /libpath:"../ReplicaNetPublic/ReplicaNet/lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ENDIF 

# Begin Target

# Name "Example6 - Win32 DebugMT"
# Name "Example6 - Win32 ReleaseMT"
# Begin Group "ROLFiles"

# PROP Default_Filter "rol"
# Begin Source File

SOURCE=.\_Def_Example6.rol

!IF  "$(CFG)" == "Example6 - Win32 DebugMT"

# Begin Custom Build
InputPath=.\_Def_Example6.rol
InputName=_Def_Example6

BuildCmds= \
	..\ReplicaNetPublic\RNROLCompiler\bin\rnrolcompiler.exe $(InputName).rol $(InputName).cpp $(InputName).h

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Example6 - Win32 ReleaseMT"

# Begin Custom Build
InputPath=.\_Def_Example6.rol
InputName=_Def_Example6

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

SOURCE=.\_RO_AnObject.rol

!IF  "$(CFG)" == "Example6 - Win32 DebugMT"

# Begin Custom Build
InputPath=.\_RO_AnObject.rol
InputName=_RO_AnObject

BuildCmds= \
	..\ReplicaNetPublic\RNROLCompiler\bin\rnrolcompiler.exe $(InputName).rol $(InputName).cpp $(InputName).h

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "Example6 - Win32 ReleaseMT"

# Begin Custom Build
InputPath=.\_RO_AnObject.rol
InputName=_RO_AnObject

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

SOURCE=.\STL.lang
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\WatchedFloat.lang
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "CompiledObjects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\_Def_Example6.cpp
# End Source File
# Begin Source File

SOURCE=.\_Def_Example6.h
# End Source File
# Begin Source File

SOURCE=.\_RO_AnObject.cpp
# End Source File
# Begin Source File

SOURCE=.\_RO_AnObject.h
# End Source File
# End Group
# Begin Group "Objects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AnObject.cpp
# End Source File
# Begin Source File

SOURCE=.\AnObject.h
# End Source File
# End Group
# Begin Group "DataBlockExtensions"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataBlock_GenericSTLVirtual.cpp
# End Source File
# Begin Source File

SOURCE=.\DataBlock_GenericSTLVirtual.h
# End Source File
# Begin Source File

SOURCE=.\DataBlock_STL.h
# End Source File
# Begin Source File

SOURCE=.\DataBlock_WatchedFloat.h
# End Source File
# Begin Source File

SOURCE=.\NetworkVector3.cpp
# End Source File
# Begin Source File

SOURCE=.\NetworkVector3.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\ReplicaNetPublic\AmalgamatedReplicaNet.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
