@echo off

if [%1] == [] goto help
if "%1" == "help" goto help

set GETDEP_ARG=%1
if "%1" == "Visual" if "%2" == "Studio" if "%3" == "2013" set GETDEP_ARG=v120
if "%1" == "Visual" if "%2" == "Studio" if "%3" == "2015" set GETDEP_ARG=v130

git clone https://github.com/snek-game/snek-deps-%GETDEP_ARG%/
goto noop

:help
set MSG="Usage: snekdep {v120,v140,Visual Studio 2013,Visual Studio 2015}"
echo %MSG:"=%
echo.

:noop
