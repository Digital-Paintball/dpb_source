:: buildmap.bat [mapname] [arguments]
:: Hit F9 in Worldcraft first, so that it generates a .vmx for this script to compile.

:: Variable parameter code by fred fisher (ffisher@nvidia.com) http://www.ericphelps.com/batch/samples/fredargs.bat.txt

@echo off

setlocal

:: init variables
set mapname=%1
set bspname=%mapname%.bsp

:: First argument is always the map name.
shift

goto loop
:shift3loop
shift
:shift2loop
shift
:shiftloop
shift
:loop

:: here test for expected args

:: if "%1"=="-a" set argument=%2
:: if "%1"=="-a" goto shift2loop

:: For two argument parameters:
:: if "%1"=="-3" set name3=%2 %3
:: if "%1"=="-3" goto shift3loop

if "%1" == "" goto doneargs

echo Usage error
echo UNRECOGNIZED ARG '%1'
goto done

:doneargs

%sourcesdk%\bin\vbsp.exe "%mapname%"

%sourcesdk%\bin\vvis.exe "%mapname%"

%sourcesdk%\bin\vrad.exe "%mapname%"

copy "%bspname%" "%vproject%\maps\%bspname%"

erase "..\..\production\maps\%bspname%"

move "%bspname%" "..\..\production\maps\%bspname%"

echo Don't forget to build cube maps!

endlocal