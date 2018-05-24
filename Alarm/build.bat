@echo on
pushd
cd %~dp0

set generated_file_dirname=generated
set runtime_dir_name=runtime
set language=c++-msvc11
set version=2.7.2

call DezyneGenerateCode.bat
msbuild vs2013project\Alarm.sln /p:Configuration=Release /t:Rebuild /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"

set version=development
call DezyneGenerateCode.bat
msbuild vs2013project\Alarm.sln /p:Configuration=Release /t:Rebuild /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"

set language=c++
set version=development
msbuild vs2017project\Alarm.sln /p:Configuration=Release /t:Rebuild /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"
