// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#include <string>
#include <stdexcept>
namespace desktop {
inline std::string updatedConfig(std::string txt,const std::string&dll,bool remove){
 if(txt.find('\0')!=std::string::npos)throw std::runtime_error("config.txt must use UTF-8 text. No changes made.");
 const std::string begin="# BEGIN HQ EFFECTS WINDOWS",end="# END HQ EFFECTS WINDOWS";auto b=txt.find(begin),e=txt.find(end);
 if((b==std::string::npos)!=(e==std::string::npos)||(b!=std::string::npos&&(e<b||txt.find(begin,b+begin.size())!=std::string::npos||txt.find(end,e+end.size())!=std::string::npos)))throw std::runtime_error("Incomplete or duplicate HQ configuration markers; no changes made.");
 if(b!=std::string::npos){e+=end.size();if(e<txt.size()&&txt[e]=='\r')e++;if(e<txt.size()&&txt[e]=='\n')e++;txt.erase(b,e-b);}
 if(!remove){if(dll.find_first_of("\"\r\n")!=std::string::npos||dll.empty())throw std::runtime_error("Unsupported installation path");if(!txt.empty()&&txt.back()!='\n')txt+="\r\n";txt+=begin+"\r\nDevice: all\r\nStage: post-mix\r\nChannel: L R\r\nVSTPlugin: Library \""+dll+"\"\r\n"+end+"\r\n";}
 return txt;
}
}
