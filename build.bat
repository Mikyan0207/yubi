@echo off

set CLCompilerPath= "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"

set opts=-FC -GR- -EHsc- -Od -Oi -WX -W4 -nologo -Zi /MDd /LD /INCREMENTAL:no /std:c++17 -Fm"yubi.map"
set code=..\Sources\Screen.cpp
set libs=user32.lib kernel32.lib

IF NOT EXIST build mkdir build

pushd build
call %CLCompilerPath% x64
cl %opts% %code% %libs% -Feyubi /link /DLL
popd
