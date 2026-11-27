// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include "paths.hpp"
#include "settings.hpp"
#include "config_patch.hpp"
namespace desktop {
inline fs::path apoRoot(){wchar_t p[32768];DWORD bytes=sizeof p;LSTATUS r=RegGetValueW(HKEY_LOCAL_MACHINE,L"SOFTWARE\\EqualizerAPO",L"ConfigPath",RRF_RT_REG_SZ|RRF_SUBKEY_WOW6464KEY,nullptr,p,&bytes);if(r==ERROR_SUCCESS&&bytes>2)return fs::path(p).parent_path();PWSTR d=nullptr;if(SUCCEEDED(SHGetKnownFolderPath(FOLDERID_ProgramFiles,0,nullptr,&d))){fs::path r=fs::path(d)/L"EqualizerAPO";CoTaskMemFree(d);return r;}return L"C:\\Program Files\\EqualizerAPO";}
inline fs::path apoConfig(){wchar_t p[32768];DWORD bytes=sizeof p;if(RegGetValueW(HKEY_LOCAL_MACHINE,L"SOFTWARE\\EqualizerAPO",L"ConfigPath",RRF_RT_REG_SZ|RRF_SUBKEY_WOW6464KEY,nullptr,p,&bytes)==ERROR_SUCCESS)return p;return apoRoot()/L"config";}
inline void attach(bool remove=false){
 auto cfg=apoConfig()/L"config.txt";if(!fs::exists(cfg))throw std::runtime_error("Install Equalizer APO and select your playback device first.");
 auto txt=readText(cfg,1024*1024);auto dll=exeDir()/L"HQEffectsDSP.dll";
 if(!remove&&!fs::exists(dll))throw std::runtime_error("HQEffectsDSP.dll is missing");
 auto changed=updatedConfig(txt,utf8(dll.wstring()),remove);
 if(!fs::exists(cfg.wstring()+L".hq-backup"))fs::copy_file(cfg,cfg.wstring()+L".hq-backup");
 txt=changed;
 atomicText(cfg,txt);
 auto root=dataDir();fs::create_directories(root/L"rooms");fs::create_directories(root/L"presets");if(!fs::exists(root/L"settings.hq"))atomicText(root/L"settings.hq",signature(0).encode());
}
inline void elevated(HWND owner,const std::wstring&args){if(reinterpret_cast<intptr_t>(ShellExecuteW(owner,L"runas",(exeDir()/L"HQEffects.exe").c_str(),args.c_str(),nullptr,SW_SHOWNORMAL))<=32)throw std::runtime_error("Setup was not started");}
}
