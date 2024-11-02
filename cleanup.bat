@echo off
set "folder_path=%cd%\Car Simulation/Tests/Results"

echo Deleting all files in %folder_path%...

rem Deletes all files in the specified local folder, but not subdirectories
del /q /s "%folder_path%\*.png"

echo All files in %folder_path% have been deleted.
pause  REM Pauses to show the result before closing the window
