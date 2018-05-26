@echo on
cd %~dp0

:: SETLOCAL will start localisation of environment variables
setlocal enableDelayedExpansion

set "OUTPUT=>build.txt 2>&1"
set "OUTPUT_APPEND=>>build.txt 2>&1"

if [%APPVEYOR%]==[True] (
  set MSBUILDLOGGER /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"
  set "OUTPUT=>build.txt 2>&1"
  set "OUTPUT_APPEND=>>build.txt 2>&1"
)

set REBUILD_X86_RELEASE=/p:Platform="x86" /p:Configuration=Release /t:Rebuild

set generated_file_dirname=generated
set runtime_dir_name=runtime
set language=c++-msvc11
set version=2.7.2

call DezyneGenerateCode.bat
msbuild vs2013project\Alarm.sln %REBUILD_X86_RELEASE% %MSBUILDLOGGER% %OUTPUT% 

set version=development
call DezyneGenerateCode.bat
msbuild vs2013project\Alarm.sln %REBUILD_X86_RELEASE% %MSBUILDLOGGER% %OUTPUT_APPEND%

set language=c++
set version=development
msbuild vs2017project\Alarm2017.sln %REBUILD_X86_RELEASE% %MSBUILDLOGGER% %OUTPUT_APPEND%

