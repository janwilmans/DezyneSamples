@echo off
cd %~dp0

::Settings
set generated_file_dirname=generated
set runtime_dir_name=runtime
set language=c++-msvc11
set version=2.2.1
::set version=development

:: install dezyne and update this path accordingly
set nodejs=%USERPROFILE%\node-v6.10.1-win-x64
if not exist %nodejs%\. (
    echo Directory does not exist: %nodejs%
    echo # Node JS not found, either Dezyne is not installed or it contains a newer Node.js version
    pause
    exit /b 1
)

::Internal Settings
set PATH=%nodejs%;%PATH%
set gendir=%~dp0\%generated_file_dirname%
set runtimedir=%~dp0\%runtime_dir_name%
set gen=dzn code --language=%language% --output=%gendir%\ -c calling_context
set verify=dzn verify
set tempfile=%~dp0\_temp.txt
call :setincludes

echo.
echo ###############################################
echo #    Generate Source Code for Dezyne Models   #
echo ###############################################
echo.

:: Tab Checking
echo Checking Dezyne files for tabs
for /f %%f in ('dir /b/s *.dzn') do (
    type %%f | find "	" && (
        echo.
        echo Found tabs in %%f!
        echo Please untabify your Dezyne models.
        echo.
        pause
        exit /b 1
    )
)
echo.
echo.    Ok. No tabs were found.
echo.

:: Generate Code from Dzn files
echo Generating source code for .dzn files ^(target Dezyne version [%version%]^):
echo.
if exist %gendir% rd /s/q %gendir%
for /f %%f in ('dir /b/s *.dzn') do (
    call :generate %%f
)
echo.

:: Generate Thread-safe Shells
echo Generating Thread Safe Shells
echo.
for /f %%f in ('dir /b/s *System.dzn') do (
    call :generateTSS %%f
)
echo.

:: Generate Exception Context Wrappers
echo Generating Exception Context Wrappers
echo.
for /f %%f in ('dir /b/s *System.dzn') do (
    call :generateECW %%f
)
echo.

:: Generate MD5Signatures
echo Generating MD5Signatures
call DznGenerateMD5Signatures.bat > DznMD5Signatures.txt
echo.

:: Add copyright headers
echo Adding copyright headers to generated files:
echo.
call :getyear
for /f %%f in ('dir /b/s %gendir%') do (
    call :addheader %%f
)
echo.

:: Download Dezyne Runtime
echo Downloading Dezyne Runtime version [%version%]
echo.
if exist %runtimedir% rd /s/q %runtimedir%
call :downloadruntime
echo.
echo Done.
echo.

pause
exit /b 0

:generate
    echo.    Generate code for: %~nx1   
    call %gen% %inc% %1 --version=%version%
    exit /b 0

:generateTSS
    echo.    Generate Thread-safe Shell [%~n1] for: %~nx1   
    call %gen% %inc% --shell=%~n1 %1 --version=%version%
    exit /b 0
    
:generateECW    
    echo.    Generate Exception Context Wrapper [%~n1]: %~n1_exception_forwarding.hh
    node.exe exception_context.js %1 > %gendir%\%~n1_exception_forwarding.hh
    exit /b 0
    
:addheader
    echo.    File: %generated_file_dirname%\%~nx1
    move /y %1 %tempfile% >nul
    echo.// Copyright (c) 1995-%YYYY% by FEI Company> %1
    echo.// All rights reserved. This file includes confidential and>> %1
    echo.// proprietary information of FEI Company.>> %1
    echo.>> %1
    :: add pragma once to prevent TICS issues about dzn_include.h being before the header-guard
    if [.hh]==[%~x1%] (
      echo.#pragma once>> %1
      echo.>> %1
    )
    echo.#include "../dzn_include.h">> %1
    echo.>> %1
    type %tempfile% >> %1
    del /f/q %tempfile%
    exit /b 0

:getyear
    :: note: cannot use %date% as it can be different depending on your regional settings
    for /f "tokens=2 delims==" %%a in ('wmic OS Get localdatetime /value') do set dt=%%a
    set YYYY=%dt:~0,4%
    exit /b 0
    
:setincludes
    set inc= -I .
    for /f %%d in ('dir /b /a:d') do call :addinclude %%d
    exit /b 0

:addinclude
    set inc=%inc% -I %1
    exit /b 0

:downloadruntime
    if not exist %runtimedir%\%language% md %runtimedir%\%language%
    call :downloadruntimefile container.hh dzn
    call :downloadruntimefile context.hh dzn
    call :downloadruntimefile coroutine.hh dzn
    call :downloadruntimefile locator.hh dzn
    call :downloadruntimefile meta.hh dzn
    call :downloadruntimefile pump.hh dzn
    call :downloadruntimefile runtime.hh dzn
    call :downloadruntimefile pump.cc
    call :downloadruntimefile runtime.cc
    exit /b 0

:downloadruntimefile
    set subdirwin=
    set subdirlin=
    if not [%2]==[] (
        set "subdirlin=%2/"
        set "subdirwin=%2\"
        if not exist %runtimedir%\%language%\%2 md %runtimedir%\%language%\%2
    )
    echo.    File: %runtime_dir_name%\%language%\%subdirwin%%1
    call dzn cat /share/runtime/%language%/%subdirlin%%1 --version=%version% > %runtimedir%\%language%\%subdirwin%%1
    exit /b 0
    
    
    
    
    