// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#define NOMINMAX
#include <windows.h>
#include <shlobj.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
namespace desktop {
namespace fs=std::filesystem;
inline std::wstring wide(const std::string&s){if(s.empty())return {};int n=MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,s.data(),int(s.size()),nullptr,0);if(n<=0)throw std::runtime_error("Invalid UTF-8");std::wstring w(n,0);MultiByteToWideChar(CP_UTF8,0,s.data(),int(s.size()),w.data(),n);return w;}
inline std::string utf8(const std::wstring&w){if(w.empty())return {};int n=WideCharToMultiByte(CP_UTF8,0,w.data(),int(w.size()),nullptr,0,nullptr,nullptr);std::string s(n,0);WideCharToMultiByte(CP_UTF8,0,w.data(),int(w.size()),s.data(),n,nullptr,nullptr);return s;}
inline fs::path dataDir(){PWSTR p=nullptr;if(FAILED(SHGetKnownFolderPath(FOLDERID_ProgramData,0,nullptr,&p)))throw std::runtime_error("ProgramData unavailable");fs::path r=fs::path(p)/L"HQEffects";CoTaskMemFree(p);return r;}
inline fs::path exeDir(){wchar_t p[32768];DWORD n=GetModuleFileNameW(nullptr,p,32768);if(!n||n>=32768)throw std::runtime_error("Program path unavailable");return fs::path(p).parent_path();}
inline std::string readText(const fs::path&p,size_t max=16384){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error("Cannot read file");std::string s;char b[4096];while(f){f.read(b,sizeof b);s.append(b,size_t(f.gcount()));if(s.size()>max)throw std::runtime_error("File exceeds size limit");}return s;}
inline void atomicText(const fs::path&p,const std::string&s){auto tmp=p;tmp+=L".tmp-"+std::to_wstring(GetCurrentProcessId());{std::ofstream f(tmp,std::ios::binary|std::ios::trunc);if(!f||!f.write(s.data(),s.size()))throw std::runtime_error("Cannot save file");}if(!MoveFileExW(tmp.c_str(),p.c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)){DeleteFileW(tmp.c_str());throw std::runtime_error("Cannot replace file");}}
}
