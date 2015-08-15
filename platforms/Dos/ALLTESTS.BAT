REM Run all tests on Windows systems
@echo off
set log=%~n0.LOG & shift & set args=%*
if exist %log% del %log%
echo.>exit
for %%a in (*.EXE) do call :run %%a
echo. & type %log%
echo. & echo Done.
exit/b

:run
 echo Running %1 inside DOSBox...
 echo *** %1 (%~z1 bytes) *********************>>%log%
 DOSBox.exe -conf ..\platforms\Dos\dosbox-0.74.conf exit ^
 -c "%1 %args%>>%log%" -noconsole -exit
exit/b
