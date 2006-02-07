@echo off
:: $Id $

:: This will build Digital Paintball from source and place it in a directory
:: ready to be packaged. You must have cmake and Visual Studio installed in
:: the default locations on the machine when you run this script.

:: Parameters:	Default:
:: -c 60|70|71	71					Visual Studio version
:: -s <path>	C:\svn\dpb\trunk	Source code directory
:: -b <path>	C:\build\dpb		Build directory
:: -i <path>	C:\install\dpb		Install directory

:: Variable parameter code by fred fisher (ffisher@nvidia.com) http://www.ericphelps.com/batch/samples/fredargs.bat.txt

setlocal

:: init variables
set compiler=71
set source=C:\svn\dpb\trunk
set build=C:\build\dpb
set install=/install/dpb

goto loop
:shift3loop
shift
:shift2loop
shift
:shiftloop
shift
:loop

:: here test for expected args

if "%1"=="-c" set compiler=%2
if "%1"=="-c" goto shift2loop

if "%1"=="-s" set source=%2
if "%1"=="-s" goto shift2loop

if "%1"=="-b" set build=%2
if "%1"=="-b" goto shift2loop

if "%1"=="-i" set install=%2
if "%1"=="-i" goto shift2loop

:: For two argument parameters:
:: if "%1"=="-3" set name3=%2 %3
:: if "%1"=="-3" goto shift3loop

if "%1" == "" goto doneargs

echo Usage error
echo UNRECOGNIZED ARG '%1'
goto done

:doneargs

:: Check the compiler version to make sure it is OK to use.

if not "%compiler%"=="71" goto vs70test
call "\Program Files\Microsoft Visual Studio .NET 2003\Common7\Tools\vsvars32.bat"
goto beginbuild

:vs70test
if not "%compiler%"=="70" goto vs60test
call "\Program Files\Microsoft Visual Studio .NET\Common7\Tools\vsvars32.bat"
goto beginbuild

:vs60test
if not "%compiler%"=="60" goto badcompiler
call "\Program Files\Microsoft Visual Studio\VC98\Bin\vcvars32.bat"
goto beginbuild

:badcompiler
echo Compiler "%compiler%" is not a valid compiler.
echo Supported compilers are:
echo 60		Microsoft Visual Studio 6.0
echo 70		Microsoft Visual Studio .NET (7.0)
echo 71		Microsoft Visual Studio .NET 2003 (7.1)
goto done

:beginbuild
cd %build%

:: Execute the build process. We've assured the administrator that nothing will go wrong!

@echo on

cmake -G"NMake Makefiles" -DCMAKE_INSTALL_PREFIX:STRING=%install% -DCMAKE_BUILD_TYPE:STRING=RelWithDebInfo "%source%"
::@if not errorlevel 0 goto builderror

nmake
:: nmake returns 2 on fail.
@if errorlevel 2 goto builderror

nmake install
@if errorlevel 2 goto builderror

@echo off
echo Build completed successfully.

:: Cleanup crew! These two are generated by CMake, but unwelcome by our packager.
erase "%install%\bin\client.lib"
erase "%install%\bin\server.lib"

goto :done
:builderror
@echo off
echo Build error, aborting.

:done

endlocal