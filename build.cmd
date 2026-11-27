@echo off
setlocal
cd /d "%~dp0"
where g++.exe >nul 2>&1 || (echo Install MinGW-w64 x64 GCC 14 or newer and add it to PATH. & exit /b 1)
if not exist build mkdir build
if not exist dist mkdir dist
windres -Isrc src\app.rc -o build\app-res.o || exit /b 1
g++ -std=c++20 -O2 -s -static -pthread -Icore -Isrc -shared src\plugin.cpp -o dist\HQEffectsDSP.dll -lole32 -lshell32 -luuid || exit /b 1
g++ -std=c++20 -O2 -s -static -pthread -Icore -Isrc -municode -mwindows src\gui.cpp build\app-res.o -o dist\HQEffects.exe -lcomctl32 -lcomdlg32 -lpsapi -lole32 -lshell32 -luuid -ladvapi32 || exit /b 1
where makensis.exe >nul 2>&1 || (echo Add NSIS 3 to PATH to build the installer. & exit /b 1)
makensis installer.nsi
