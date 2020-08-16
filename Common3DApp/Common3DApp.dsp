# Microsoft Developer Studio Project File - Name="Common3DApp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Common3DApp - Win32 DebugMTDLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Common3DApp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Common3DApp.mak" CFG="Common3DApp - Win32 DebugMTDLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Common3DApp - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Common3DApp - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Common3DApp - Win32 DebugMT" (based on "Win32 (x86) Static Library")
!MESSAGE "Common3DApp - Win32 ReleaseMT" (based on "Win32 (x86) Static Library")
!MESSAGE "Common3DApp - Win32 ReleaseMTDLL" (based on "Win32 (x86) Static Library")
!MESSAGE "Common3DApp - Win32 DebugMTDLL" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Common3DApp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "./Inc" /D "NDEBUG" /D "_LIB" /D "_CRT_SECURE_NO_DEPRECATE" /D "WIN32" /D "_MBCS" /Fr /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Lib\Common3DAppRS.lib"

!ELSEIF  "$(CFG)" == "Common3DApp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /GX /ZI /Od /I "./Inc" /D "_DEBUG" /D "_LIB" /D "_CRT_SECURE_NO_DEPRECATE" /D "WIN32" /D "_MBCS" /Fr /YX /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Lib\Common3DAppDS.lib"

!ELSEIF  "$(CFG)" == "Common3DApp - Win32 DebugMT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Common3DApp___Win32_DebugMT"
# PROP BASE Intermediate_Dir "Common3DApp___Win32_DebugMT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Common3DApp___Win32_DebugMT"
# PROP Intermediate_Dir "Common3DApp___Win32_DebugMT"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".\Inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /ZI /Od /I "./Inc" /D "_DEBUG" /D "_LIB" /D "_CRT_SECURE_NO_DEPRECATE" /D "WIN32" /D "_MBCS" /Fr /YX /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Lib\Common3DAppDS.lib"
# ADD LIB32 /nologo /out:"Lib\Common3DAppDM.lib"

!ELSEIF  "$(CFG)" == "Common3DApp - Win32 ReleaseMT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Common3DApp___Win32_ReleaseMT"
# PROP BASE Intermediate_Dir "Common3DApp___Win32_ReleaseMT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Common3DApp___Win32_ReleaseMT"
# PROP Intermediate_Dir "Common3DApp___Win32_ReleaseMT"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I ".\Inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "./Inc" /D "NDEBUG" /D "_LIB" /D "_CRT_SECURE_NO_DEPRECATE" /D "WIN32" /D "_MBCS" /Fr /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Lib\Common3DAppRS.lib"
# ADD LIB32 /nologo /out:"Lib\Common3DAppRM.lib"

!ELSEIF  "$(CFG)" == "Common3DApp - Win32 ReleaseMTDLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Common3DApp___Win32_ReleaseMTDLL"
# PROP BASE Intermediate_Dir "Common3DApp___Win32_ReleaseMTDLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Common3DApp___Win32_ReleaseMTDLL"
# PROP Intermediate_Dir "Common3DApp___Win32_ReleaseMTDLL"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I ".\Inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./Inc" /D "NDEBUG" /D "_LIB" /D "_CRT_SECURE_NO_DEPRECATE" /D "WIN32" /D "_MBCS" /Fr /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Lib\Common3DAppRM.lib"
# ADD LIB32 /nologo /out:"Lib\Common3DAppRMDLL.lib"

!ELSEIF  "$(CFG)" == "Common3DApp - Win32 DebugMTDLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Common3DApp___Win32_DebugMTDLL"
# PROP BASE Intermediate_Dir "Common3DApp___Win32_DebugMTDLL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Common3DApp___Win32_DebugMTDLL"
# PROP Intermediate_Dir "Common3DApp___Win32_DebugMTDLL"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I ".\Inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "./Inc" /D "_DEBUG" /D "_LIB" /D "_CRT_SECURE_NO_DEPRECATE" /D "WIN32" /D "_MBCS" /Fr /YX /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Lib\Common3DAppDM.lib"
# ADD LIB32 /nologo /out:"Lib\Common3DAppDMDLL.lib"

!ENDIF 

# Begin Target

# Name "Common3DApp - Win32 Release"
# Name "Common3DApp - Win32 Debug"
# Name "Common3DApp - Win32 DebugMT"
# Name "Common3DApp - Win32 ReleaseMT"
# Name "Common3DApp - Win32 ReleaseMTDLL"
# Name "Common3DApp - Win32 DebugMTDLL"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Src\d3dapp.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\d3dfile.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\d3dfont.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\d3dutil.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ddutil.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\didcfgview.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\diutil.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\dmutil.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\dsutil.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\dxutil.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\netclient.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\netconnect.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\netvoice.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Inc\d3dapp.h
# End Source File
# Begin Source File

SOURCE=.\Inc\d3dfile.h
# End Source File
# Begin Source File

SOURCE=.\Inc\d3dfont.h
# End Source File
# Begin Source File

SOURCE=.\Inc\d3dres.h
# End Source File
# Begin Source File

SOURCE=.\Inc\d3dutil.h
# End Source File
# Begin Source File

SOURCE=.\Inc\ddutil.h
# End Source File
# Begin Source File

SOURCE=.\Inc\didcfgview.h
# End Source File
# Begin Source File

SOURCE=.\Inc\diutil.h
# End Source File
# Begin Source File

SOURCE=.\Inc\dmutil.h
# End Source File
# Begin Source File

SOURCE=.\Inc\dsutil.h
# End Source File
# Begin Source File

SOURCE=.\Inc\dxutil.h
# End Source File
# Begin Source File

SOURCE=.\Inc\netclient.h
# End Source File
# Begin Source File

SOURCE=.\Inc\netclientres.h
# End Source File
# Begin Source File

SOURCE=.\Inc\netconnect.h
# End Source File
# Begin Source File

SOURCE=.\Inc\netconnectres.h
# End Source File
# Begin Source File

SOURCE=.\Inc\netvoice.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
