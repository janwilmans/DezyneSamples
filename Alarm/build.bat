@echo on
cd %~dp0
msbuild vs2013project\Alarm.sln /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"
msbuild vs2017project\Alarm.sln /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"