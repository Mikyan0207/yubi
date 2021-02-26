@echo off

set CLCompilerPath= "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"

set opts=-FC -GR- -EHa- -Od -Oi -WX -W4 -nologo -Zi /std:c++17 -Fm"yubi.map"
set code=..\Sources\main.cpp

IF NOT EXIST build mkdir build

pushd build
call %CLCompilerPath% x64
cl %opts% %code% -Feyubi
popd
