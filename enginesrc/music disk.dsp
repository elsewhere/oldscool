# Microsoft Developer Studio Project File - Name="music disk" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=music disk - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "music disk.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "music disk.mak" CFG="music disk - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "music disk - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "music disk - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "music disk - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40b /d "NDEBUG"
# ADD RSC /l 0x40b /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib bass.lib opengl32.lib glu32.lib jpeg.lib winmm.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "music disk - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40b /d "_DEBUG"
# ADD RSC /l 0x40b /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib glaux.lib gdi32.lib user32.lib advapi32.lib bass.lib winmm.lib jpeg.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "music disk - Win32 Release"
# Name "music disk - Win32 Debug"
# Begin Group "script"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\script.txt
# End Source File
# End Group
# Begin Group "Events"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Events\kuutio.cpp
# End Source File
# Begin Source File

SOURCE=.\Events\kuutio.hpp
# End Source File
# Begin Source File

SOURCE=.\Events\levy.cpp
# End Source File
# Begin Source File

SOURCE=.\Events\levy.hpp
# End Source File
# Begin Source File

SOURCE=.\Events\munkki.cpp
# End Source File
# Begin Source File

SOURCE=.\Events\munkki.hpp
# End Source File
# Begin Source File

SOURCE=.\Events\munkki2.cpp
# End Source File
# Begin Source File

SOURCE=.\Events\munkki2.hpp
# End Source File
# Begin Source File

SOURCE=.\Events\munkki3.cpp
# End Source File
# Begin Source File

SOURCE=.\Events\munkki3.hpp
# End Source File
# Begin Source File

SOURCE=.\Events\pallo.cpp
# End Source File
# Begin Source File

SOURCE=.\Events\pallo.hpp
# End Source File
# Begin Source File

SOURCE=.\Events\picfadein.cpp
# End Source File
# Begin Source File

SOURCE=.\Events\picfadein.hpp
# End Source File
# Begin Source File

SOURCE=.\Events\picfadeout.cpp
# End Source File
# Begin Source File

SOURCE=.\Events\picfadeout.hpp
# End Source File
# Begin Source File

SOURCE=.\Events\picpulse.cpp
# End Source File
# Begin Source File

SOURCE=.\Events\picpulse.hpp
# End Source File
# Begin Source File

SOURCE=.\Events\picshow.cpp
# End Source File
# Begin Source File

SOURCE=.\Events\picshow.hpp
# End Source File
# Begin Source File

SOURCE=.\Events\tunneli.cpp
# End Source File
# Begin Source File

SOURCE=.\Events\tunneli.hpp
# End Source File
# Begin Source File

SOURCE=.\Events\tunneli2.cpp
# End Source File
# Begin Source File

SOURCE=.\Events\tunneli2.hpp
# End Source File
# Begin Source File

SOURCE=.\Events\twister.cpp
# End Source File
# Begin Source File

SOURCE=.\Events\twister.hpp
# End Source File
# Begin Source File

SOURCE=.\Events\vilkku.cpp
# End Source File
# Begin Source File

SOURCE=.\Events\vilkku.hpp
# End Source File
# Begin Source File

SOURCE=.\Events\wobble.cpp
# End Source File
# Begin Source File

SOURCE=.\Events\wobble.hpp
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\beziercurve.cpp
# End Source File
# Begin Source File

SOURCE=.\bpm.cpp
# End Source File
# Begin Source File

SOURCE=.\console.cpp
# End Source File
# Begin Source File

SOURCE=.\event.cpp
# End Source File
# Begin Source File

SOURCE=.\hermitecurve.cpp
# End Source File
# Begin Source File

SOURCE=.\image.cpp
# End Source File
# Begin Source File

SOURCE=.\lista.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\matikka.cpp
# End Source File
# Begin Source File

SOURCE=.\matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\mesh.cpp
# End Source File
# Begin Source File

SOURCE=.\primitives.cpp
# End Source File
# Begin Source File

SOURCE=.\song.cpp
# End Source File
# Begin Source File

SOURCE=.\spline.cpp
# End Source File
# Begin Source File

SOURCE=.\stuff.cpp
# End Source File
# Begin Source File

SOURCE=.\texture.cpp
# End Source File
# Begin Source File

SOURCE=.\timer.cpp
# End Source File
# Begin Source File

SOURCE=.\vector.cpp
# End Source File
# Begin Source File

SOURCE=.\vertexbuffer.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\beziercurve.hpp
# End Source File
# Begin Source File

SOURCE=.\bpm.hpp
# End Source File
# Begin Source File

SOURCE=.\console.hpp
# End Source File
# Begin Source File

SOURCE=.\event.hpp
# End Source File
# Begin Source File

SOURCE=.\font.hpp
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\hermitecurve.hpp
# End Source File
# Begin Source File

SOURCE=.\image.hpp
# End Source File
# Begin Source File

SOURCE=.\lista.hpp
# End Source File
# Begin Source File

SOURCE=.\matikka.h
# End Source File
# Begin Source File

SOURCE=.\matrix.hpp
# End Source File
# Begin Source File

SOURCE=.\mesh.hpp
# End Source File
# Begin Source File

SOURCE=.\primitives.hpp
# End Source File
# Begin Source File

SOURCE=.\song.hpp
# End Source File
# Begin Source File

SOURCE=.\spline.hpp
# End Source File
# Begin Source File

SOURCE=.\stuff.hpp
# End Source File
# Begin Source File

SOURCE=.\texture.hpp
# End Source File
# Begin Source File

SOURCE=.\timer.hpp
# End Source File
# Begin Source File

SOURCE=.\vector.hpp
# End Source File
# Begin Source File

SOURCE=.\vertexbuffer.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
