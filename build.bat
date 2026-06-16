@echo off
REM build_and_run.bat - Simple script to build and run a raylib project

echo Building raylib project...

REM Define main variables
SET MAKE_PATH=C:/raylib/w64devkit/bin/mingw32-make.exe
SET RAYLIB_PATH=C:/raylib/raylib
SET PROJECT_NAME=Game
SET SOURCE_FILES="src/main.cpp src/gameCamera.cpp"

REM Check if we want a debug build
if "%1"=="debug" (
  echo Building in DEBUG mode...
  %MAKE_PATH% RAYLIB_PATH=%RAYLIB_PATH% PROJECT_NAME=%PROJECT_NAME% OBJS=%SOURCE_FILES% BUILD_MODE=DEBUG
) else (
  echo Building in RELEASE mode...
  %MAKE_PATH% RAYLIB_PATH=%RAYLIB_PATH% PROJECT_NAME=%PROJECT_NAME% OBJS=%SOURCE_FILES%
)

REM Check if build succeeded
if %ERRORLEVEL% neq 0 (
  echo Build failed! Error code: %ERRORLEVEL%
  pause
  exit /b %ERRORLEVEL%
)

echo Build successful!
echo Running %PROJECT_NAME%.exe...
echo.

REM Run the compiled executable
%PROJECT_NAME%.exe

echo.
echo Program exited with code: %ERRORLEVEL%
pause