@echo off
SETLOCAL EnableDelayedExpansion 
cd %~dp0

if [%APPVEYOR%]==[True] (
  SETLOCAL MSBUILDLOGGER /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"
)

set REBUILD_X86_RELEASE=/p:Platform="x86" /p:Configuration=Release /t:Rebuild

msbuild Tests.sln %REBUILD_X86_RELEASE% %MSBUILDLOGGER%

if NOT [%APPVEYOR%]==[True] (
    Release\Tests.exe -s -i -w NoTests
)
