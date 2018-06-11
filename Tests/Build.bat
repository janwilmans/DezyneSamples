@echo on
SETLOCAL EnableDelayedExpansion 
cd %~dp0

if [%APPVEYOR%]==[True] (
  SET MSBUILDLOGGER /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"
)

set REBUILD_X86_RELEASE=/p:Platform="x86" /p:Configuration=Release /t:Rebuild

msbuild Tests.sln %REBUILD_X86_RELEASE% %MSBUILDLOGGER%

cd %~dp0\Release

if [%APPVEYOR%]==[True] (
    Tests.exe -s -i -w NoTests -r junit -o junit-results.xml
) else (
    Tests.exe -s -i -w NoTests
)

exit /b 0