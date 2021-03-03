@echo off

set CLCompilerPath= "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"

set dll_opts=/DYUBI_EXPORTS /LD /INCREMENTAL:no
set dll_code=..\Sources\dll_main.cpp ..\Sources\Screen.cpp ..\Sources\Monitor.cpp
set dll_libs=user32.lib kernel32.lib

set opts=-FC -GR- -EHsc- -Od -Oi -WX -W4 -wd4100 -nologo -Zi /std:c++17 -Fm"yubi.map"
set code=..\Sources\main.cpp
set libs=user32.lib kernel32.lib yubi.lib

IF NOT EXIST build mkdir build

pushd build
call %CLCompilerPath% x64
cl %opts% %dll_opts% %dll_code% %dll_libs% -Feyubi /link /DLL
cl %opts% %code% %libs% -Feyubic
popd
