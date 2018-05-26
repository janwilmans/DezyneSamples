@echo on
cd %~dp0

if [%APPVEYOR%]==[True] (
  SETLOCAL MSBUILDLOGGER /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"
  SETLOCAL OUTPUT >%APPVEYOR_BUILD_FOLDER%\build.txt 2>&1
  SETLOCAL OUTPUT_APPEND >>%APPVEYOR_BUILD_FOLDER%\build.txt 2>&1
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
