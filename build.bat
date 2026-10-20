@echo off
setlocal

REM Define main variables
SET MAKE_PATH=C:/raylib/w64devkit/bin/mingw32-make.exe
SET RAYLIB_PATH=C:/raylib/raylib
SET PROJECT_NAME=Game
SET SOURCE_FILES=src/main.cpp src/player.cpp src/platforms.cpp src/functions.cpp src/gameCamera.cpp src/animator.cpp

REM Select build mode
echo.
echo Select build mode:
echo [1] Debug
echo [2] Release
echo.

set /p CHOICE=Enter choice (1 or 2): 

if "%CHOICE%"=="1" (
    SET BUILD_MODE=DEBUG
    SET PROJECT_NAME=debug
) else if "%CHOICE%"=="2" (
    SET BUILD_MODE=RELEASE
    SET PROJECT_NAME=release
) else (
    echo Invalid choice!
    pause
    exit /b 1
)

echo.
echo Building in %BUILD_MODE% mode...

REM Build project
%MAKE_PATH% RAYLIB_PATH=%RAYLIB_PATH% PROJECT_NAME=%PROJECT_NAME% OBJS="%SOURCE_FILES%" BUILD_MODE=%BUILD_MODE%

REM Check if build succeeded
if %ERRORLEVEL% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo Build successful!
echo Running %PROJECT_NAME%.exe...
echo.

REM Run executable
%PROJECT_NAME%.exe

echo.
echo Program exited with code: %ERRORLEVEL%
pause