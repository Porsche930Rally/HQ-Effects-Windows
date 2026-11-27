#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
: "${HQ_MINGW_PREFIX:=x86_64-w64-mingw32-}"
: "${HQ_CXX:=${HQ_MINGW_PREFIX}g++}"
: "${HQ_WINDRES:=${HQ_MINGW_PREFIX}windres}"
mkdir -p build dist
rcflags=(); if [[ -n "${HQ_RC_PREPROCESSOR:-}" ]]; then rcflags=(--preprocessor="$HQ_RC_PREPROCESSOR" --preprocessor-arg=-E --preprocessor-arg=-xc --preprocessor-arg=-DRC_INVOKED); fi
"$HQ_WINDRES" "${rcflags[@]}" -Isrc src/app.rc -o build/app-res.o
flags=(-std=c++20 -O2 -s -static -pthread -Icore -Isrc -Wall -Wextra -Wno-misleading-indentation)
"$HQ_CXX" "${flags[@]}" -shared src/plugin.cpp -o dist/HQEffectsDSP.dll -lole32 -lshell32 -luuid
"$HQ_CXX" "${flags[@]}" -municode -mwindows src/gui.cpp build/app-res.o -o dist/HQEffects.exe -lcomctl32 -lcomdlg32 -lpsapi -lole32 -lshell32 -luuid -ladvapi32
"$HQ_CXX" "${flags[@]}" -municode tests/windows_smoke.cpp -o build/windows-smoke.exe -lpsapi -lole32 -lshell32 -luuid
"${HQ_CXX}" "${flags[@]}" -municode tests/setup_tests.cpp -o build/setup-tests.exe -lole32 -lshell32 -luuid -ladvapi32
if command -v makensis >/dev/null; then makensis installer.nsi; fi
