# DezyneSamples
Simple examples of Dezyne models and their generated code

# Continuous integration status

The status below indicates the status of building the DezyneSamples Alarm project that includes the following features:

## Configuration 1

- generate code for the c++-msvc11 target @ **version 2.7.2**
- generate a thread-safe shell for the AlarmSystem
- generate exception wrappers for the AlarmSystem using a combination of the -c calling_context feature and exception_wrapper.js
- compile with Visual Studio **2013** at warning level 4

## Configuration 2

- generate code for the c++-msvc11 target @ **development** (the latest un-released head version)
- generate a thread-safe shell for the AlarmSystem
- generate exception wrappers for the AlarmSystem using a combination of the -c calling_context feature and exception_wrapper.js
- compile with Visual Studio **2013** at warning level 4

## Configuration 3

- generate code for the **c++ (c++14)** target @ **development** (the latest un-released head version)
- generate a thread-safe shell for the AlarmSystem
- generate exception wrappers for the AlarmSystem using a combination of the -c calling_context feature and exception_wrapper.js
- compile with Visual Studio **2017** at warning level 4

## Overall build status

------------------
[![Build status](https://ci.appveyor.com/api/projects/status/0s2ncnqubg2ksesb/branch/master?svg=true)](https://ci.appveyor.com/project/janwilmans/dezynesamples)
------------------
There should be a badge between the two lines.






