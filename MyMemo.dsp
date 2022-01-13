# Microsoft Developer Studio Project File - Name="MyMemo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MyMemo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MyMemo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MyMemo.mak" CFG="MyMemo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MyMemo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MyMemo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "MyMemo - Win32 Release_NoMFC" (based on "Win32 (x86) Application")
!MESSAGE "MyMemo - Win32 DRelease_NoMFC" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MyMemo - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../mysource/include" /I "D:\dev\mytest\InetTest\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "BOOST_REGEX_STATIC_LINK" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 imm32.lib wininet.lib /nologo /subsystem:windows /machine:I386
# Begin Custom Build
TargetPath=.\Release\MyMemo.exe
TargetName=MyMemo
InputPath=.\Release\MyMemo.exe
SOURCE="$(InputPath)"

"..\MyMemoInstall\source\$(TargetName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo on 
	copy $(TargetPath) ..\MyMemoInstall\source 
	echo off 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MyMemo - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../mysource/include" /I "D:\dev\mytest\InetTest\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL" /d "VER_ENG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 imm32.lib wininet.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "MyMemo - Win32 Release_NoMFC"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MyMemo___Win32_Release_NoMFC"
# PROP BASE Intermediate_Dir "MyMemo___Win32_Release_NoMFC"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_NoMFC"
# PROP Intermediate_Dir "Release_NoMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../../mysource/include" /I "D:\dev\mytest\InetTest\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../mysource/include" /I "D:\dev\mytest\InetTest\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 imm32.lib wininet.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 imm32.lib wininet.lib /nologo /subsystem:windows /machine:I386
# Begin Custom Build
TargetPath=.\Release_NoMFC\MyMemo.exe
TargetName=MyMemo
InputPath=.\Release_NoMFC\MyMemo.exe
SOURCE="$(InputPath)"

"..\MyMemoInstall\source\$(TargetName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo on 
	copy $(TargetPath) ..\MyMemoInstall\source 
	echo off 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MyMemo - Win32 DRelease_NoMFC"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MyMemo___Win32_DRelease_NoMFC"
# PROP BASE Intermediate_Dir "MyMemo___Win32_DRelease_NoMFC"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MyMemo___Win32_DRelease_NoMFC"
# PROP Intermediate_Dir "MyMemo___Win32_DRelease_NoMFC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "../../mysource/include" /I "D:\dev\mytest\InetTest\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /Od /I "../../mysource/include" /I "D:\dev\mytest\InetTest\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 imm32.lib wininet.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 imm32.lib wininet.lib /nologo /subsystem:windows /debug /machine:I386
# Begin Custom Build
TargetPath=.\MyMemo___Win32_DRelease_NoMFC\MyMemo.exe
TargetName=MyMemo
InputPath=.\MyMemo___Win32_DRelease_NoMFC\MyMemo.exe
SOURCE="$(InputPath)"

"..\MyMemoInstall\source\$(TargetName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo on 
	copy $(TargetPath) ..\MyMemoInstall\source 
	echo off 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "MyMemo - Win32 Release"
# Name "MyMemo - Win32 Debug"
# Name "MyMemo - Win32 Release_NoMFC"
# Name "MyMemo - Win32 DRelease_NoMFC"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DirNFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVirtualFolder.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVirtualFolder2.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\InetFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\LeftView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MyMemo.cpp
# End Source File
# Begin Source File

SOURCE=.\MyMemo.rc
# End Source File
# Begin Source File

SOURCE=.\MyMemoDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MyMemoView.cpp
# End Source File
# Begin Source File

SOURCE=.\ParseKi.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfilePrivate.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeOutSocket1.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeViewExt.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DirNFile.h
# End Source File
# Begin Source File

SOURCE=.\DlgVirtualFolder.h
# End Source File
# Begin Source File

SOURCE=.\DlgVirtualFolder2.h
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\InetFile.h
# End Source File
# Begin Source File

SOURCE=.\Label.h
# End Source File
# Begin Source File

SOURCE=.\LeftView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MemoTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MyMemo.h
# End Source File
# Begin Source File

SOURCE=.\MyMemoDoc.h
# End Source File
# Begin Source File

SOURCE=.\MyMemoView.h
# End Source File
# Begin Source File

SOURCE=.\ParseKi.h
# End Source File
# Begin Source File

SOURCE=.\ProfilePrivate.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TimeOutSocket1.h
# End Source File
# Begin Source File

SOURCE=.\TreeViewExt.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\MyMemo.ico
# End Source File
# Begin Source File

SOURCE=.\res\MyMemo.rc2
# End Source File
# Begin Source File

SOURCE=.\res\MyMemoDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\MyMemo.txt
# End Source File
# End Target
# End Project
