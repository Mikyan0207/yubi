@echo off

set CLCompilerPath= "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"


set includes=/I"..\Headers"
set opts=/EHsc -Od -Oi -WX -W4 -wd4100 -wd4251 -wd4201 /std:c++17 /Feyubi
set code=..\Source\main.cpp ..\Source\WinEvents.cpp
set libs=user32.lib kernel32.lib

IF NOT EXIST build mkdir build

pushd build
call %CLCompilerPath% x64
cl %opts% %includes% %code% %libs%
popd
