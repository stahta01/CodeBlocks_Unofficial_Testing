@echo off

@REM SETLOCAL assures environment variables created in a batch file are not exported to its calling environment
setlocal
SET CurrentDir="%CD%"

set BUILD_BITS=%1
if "%BUILD_BITS%" == "32" goto BuildBits_Okay
if "%BUILD_BITS%" == "64" goto BuildBits_Okay
if exist "..\..\src\devel31_32\codeblocks.exe" set BUILD_BITS=32
if exist "..\..\src\devel31_64\codeblocks.exe" set BUILD_BITS=64
if "%BUILD_BITS%" == "32" goto BuildBits_Okay
if "%BUILD_BITS%" == "64" goto BuildBits_Okay
goto BuildBitError

:BuildBits_Okay
@rem change to the CB source root directory
cd ..\..\src

@rem GCC_ROOT is the root directory of the compiler you are going to use to build C::B with
set GCC_ROOT=C:\msys64\mingw%BUILD_BITS%
@rem set GCC_ROOT=C:\mingw64-winlib
@rem set GCC_ROOT=C:\TDM-GCC-64

@rem WXWIN is the wxWidgets root directory that you have all ready built from source 
CALL :NORMALIZEPATH "..\..\..\Libraries\wxWidgets-3.1.5_win%BUILD_BITS%"
SET WXWIN=%RETVAL%
if not exist "%WXWIN%" goto ErrNowxWidget

@rem ----------------------------------------------------------------------------
@rem Check if build worked
@rem ----------------------------------------------------------------------------
if not exist "devel31_%BUILD_BITS%\codeblocks.exe"                                                                  goto CompileError
if not exist "devel31_%BUILD_BITS%\libcodeblocks.la"  if not exist "devel31_%BUILD_BITS%\codeblocks.dll"            goto CompileError
if not exist "devel31_%BUILD_BITS%\libcodeblocks.la"  if not exist "devel31_%BUILD_BITS%\share\codeblocks\todo.zip" goto CompileError

@rem ----------------------------------------------------------------------------
@rem Copy the compiler DLL and wxWidget DLL's into the devel31_%BUILD_BITS% directory
@rem ----------------------------------------------------------------------------
@echo Copying compiler and wxWidget DLL's into the devel31_%BUILD_BITS% directory.
if "%BUILD_BITS%" == "32" if exist "%GCC_ROOT%\bin\libgcc_s_dw2-1.dll"    copy /Y "%GCC_ROOT%\bin\libgcc_s_dw2-1.dll"        devel31_%BUILD_BITS% > nul
if "%BUILD_BITS%" == "64" if exist "%GCC_ROOT%\bin\libgcc_s_seh-1.dll"    copy /Y "%GCC_ROOT%\bin\libgcc_s_seh-1.dll"        devel31_%BUILD_BITS% > nul
@rem The next DLL is required for some GCC compilers, but not for others. Either way copy it is if exists.
if "%BUILD_BITS%" == "64" if exist "%GCC_ROOT%\bin\libgcc_s_seh_64-1.dll" copy /Y "%GCC_ROOT%\bin\libgcc_s_seh_64-1.dll"     devel31_%BUILD_BITS% > nul
if exist "%GCC_ROOT%\bin\libwinpthread-1.dll"       copy /Y "%GCC_ROOT%\bin\libwinpthread-1.dll"       devel31_%BUILD_BITS% > nul
if exist "%GCC_ROOT%\bin\libstdc++-6.dll"           copy /Y "%GCC_ROOT%\bin\libstdc++-6.dll"           devel31_%BUILD_BITS% > nul

if exist "%WXWIN%\lib\gcc_dll\wxmsw*_gcc_cb.dll"    copy /Y "%WXWIN%\lib\gcc_dll\wxmsw*_gcc_cb.dll"    devel31_%BUILD_BITS% > nul
if exist "%WXWIN%\lib\gcc_dll\wxmsw*_gl_gcc_cb.dll" copy /Y "%WXWIN%\lib\gcc_dll\wxmsw*_gl_gcc_cb.dll" devel31_%BUILD_BITS% > nul

if not exist "devel31_%BUILD_BITS%\exchndl.dll"     copy /Y "exchndl\win%BUILD_BITS%\bin\*.*" devel31_%BUILD_BITS% > nul

@rem ------------------------------------------------------------------------------------------
@rem Rename DLL files if built with MSYS 2 using bootstrap/configure/make/make install process
@rem ------------------------------------------------------------------------------------------
if not exist "devel31_%BUILD_BITS%\share\codeblocks\plugins\*.dll" goto NoPluginDLLFilesFound

SET PrevDirectory="%CD%"
cd "devel31_%BUILD_BITS%\share\codeblocks\plugins"
for /f "usebackq delims=^=^" %%a in (`"dir "lib*.dll" /b" 2^>nul`) do (set fnameDLL=%%a) & call :renameDLL
cd /d %PrevDirectory%

@rem ---------------------------------------------------------------------------------------------------------
@rem Check if MSYS 2 using bootstrap/configure/make/make install process was used and if it was cleanup files
@rem ---------------------------------------------------------------------------------------------------------
if not exist "devel31_%BUILD_BITS%\*.la" goto RunUpdateBatchFile

SET PrevDirectory="%CD%"
cd "devel31_%BUILD_BITS%"
for /f "usebackq delims=^=^" %%a in (`"dir "*.la" /b/s" 2^>nul`) do del /Q %%a
cd /d %PrevDirectory%
@goto Finish

@rem ---------------------------------------------------------------------------------------------------------
@rem Check if MSYS 2 using bootstrap/configure/make/make install process was used and if it was cleanup files
@rem ---------------------------------------------------------------------------------------------------------
:RunUpdateBatchFile
@echo Start the update
@call update31_"%BUILD_BITS%".bat C:\msys64\mingw"%BUILD_BITS%"
@echo Finished the update

@goto Finish

@rem ------------------------------------------------------------------------------------------

:renameDLL
::Cuts off 1st three chars
:: Delete renamed file if it exists as the new file could have changes you want!!!
if exist "%fnameDLL:~3%" del "%fnameDLL:~3%"
ren "%fnameDLL%" "%fnameDLL:~3%"
goto :eof

:BuildBitError
@echo.
@echo ^+------------------------------------------------------^+
@echo ^| Error: NO Windows '32' or '64' parameter specified.  ^|
@echo ^|        Please run again with a parameter             ^|
@echo ^+------------------------------------------------------^+
@echo.
@echo.
@goto Finish

:CompileError
@echo.
@echo.
@echo ^+----------------------------------------------------^+
@echo ^| Error: Code::Blocks compile error was detected.    ^|
@echo ^|        Please fix the error and try again.         ^|
@echo ^+----------------------------------------------------^+
@echo.
@echo.
goto Finish

:NoPluginDLLFilesFound
@echo.
@echo.
@echo ^+------------------------------------------------------^+
@echo ^| Error: Code::Blocks Plugin DLL files not found in:   ^|
@echo ^|          "devel31_%BUILD_BITS%\share\codeblocks\plugins"       ^|
@echo ^+------------------------------------------------------^+
@echo.
@echo.
goto Finish

:ErrNowxWidget
@echo.
@echo.
@echo ^+------------------------------------------------------------------------------------------------------------^+
@echo ^|     Error: NO "%WXWIN%" sub directory found  ^|
@echo ^+------------------------------------------------------------------------------------------------------------^+
@echo. 
@echo.
goto Finish

:NORMALIZEPATH
    SET RETVAL=%~f1
    EXIT /B

:Finish
cd /d %CurrentDir%
@endlocal
