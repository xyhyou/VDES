@echo off
set HEADER_PATH=%cd%\Headers
set LIBRARY_PATH=%cd%\out\build
set DEST_PATH=E:\ECDIS(VDES)\ExternalLib
set DEST_PATH_QT=E:\GitProject\AISStudio

REM 删除原文件夹下的相关头文件
del /q /s %DEST_PATH%\Include\vdes\*.*
del /q /s %DEST_PATH%\Lib\Windows\VDES.lib
del /q /s %DEST_PATH%\Lib\Linux\libVDES.a

del /q /s %DEST_PATH_QT%\Headers\VDES\*.*
del /q /s %DEST_PATH_QT%\Libs\Win64\Debug\VDES.lib


REM Copy headers
xcopy /S /I %HEADER_PATH%\eventpp %DEST_PATH%\Include\vdes\eventpp\
xcopy  %HEADER_PATH%\VDES.h %DEST_PATH%\Include\vdes\
xcopy  %HEADER_PATH%\VDESDTD.h %DEST_PATH%\Include\vdes\
xcopy  %HEADER_PATH%\VDESConfigure.h %DEST_PATH%\Include\vdes\
xcopy  %HEADER_PATH%\BoundingBox.h %DEST_PATH%\Include\vdes\
xcopy  %HEADER_PATH%\Coordinate.h %DEST_PATH%\Include\vdes\


xcopy /S /I %HEADER_PATH%\eventpp %DEST_PATH_QT%\Headers\VDES\eventpp\
xcopy  %HEADER_PATH%\VDES.h %DEST_PATH_QT%\Headers\VDES\
xcopy  %HEADER_PATH%\VDESDTD.h %DEST_PATH_QT%\Headers\VDES\
xcopy  %HEADER_PATH%\VDESConfigure.h %DEST_PATH_QT%\Headers\VDES\
xcopy  %HEADER_PATH%\BoundingBox.h %DEST_PATH_QT%\Headers\VDES\
xcopy  %HEADER_PATH%\Coordinate.h %DEST_PATH_QT%\Headers\VDES\


REM Copy library
xcopy  %LIBRARY_PATH%\x64-debug-vs2017\Debug\VDES.lib %DEST_PATH%\Lib\Windows\
xcopy  %LIBRARY_PATH%\linux-x64-debug\libVDES.a %DEST_PATH%\Lib\Linux\libVDESD.a
xcopy  %LIBRARY_PATH%\linux-x64-release\libVDES.a %DEST_PATH%\Lib\Linux\libVDES.a


xcopy  %LIBRARY_PATH%\x64-debug\VDES.lib %DEST_PATH_QT%\Libs\Win64\Debug\

pause