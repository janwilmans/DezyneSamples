@echo off
cd %~dp0

if [%APPVEYOR%]==[True] (
  SETLOCAL MSBUILDLOGGER=/logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"
)

msbuild Tests\Tests.sln /p:Configuration=Release /t:Rebuild %MSBUILDLOGGER%

if NOT [%APPVEYOR%]==[True] (
    Tests\Release\Tests.exe -s -i -w NoTests
)
