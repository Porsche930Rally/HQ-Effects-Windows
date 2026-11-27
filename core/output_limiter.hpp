#pragma once
#include <array>
#include <algorithm>
#include <cmath>
#include <cstdint>

namespace hq {
// Stereo-linked peak limiter. A 128-frame lookahead avoids waveform saturation;
// the monotonic peak queue needs constant amortized work and no allocations.
class OutputLimiter {
 static constexpr size_t Lookahead=128, QueueSize=Lookahead+2;
 struct Peak {uint64_t frame=0;float value=0;};
 std::array<std::array<float,2>,Lookahead> audio{};
 std::array<Peak,QueueSize> peaks{};
 size_t position=0,head=0,count=0;
 uint64_t frame=0;
 float gain=1,release=0;
public:
 void reset(int rate){audio={};peaks={};position=head=count=0;frame=0;gain=1;release=1-std::exp(-1.f/(rate*.08f));}
 float reduction()const{return gain;}
 void process(float* data,int frames,float ceiling){
  ceiling=std::clamp(ceiling,.01f,.95f);
  for(int i=0;i<frames;i++){
   float l=data[2*i],r=data[2*i+1];if(!std::isfinite(l))l=0;if(!std::isfinite(r))r=0;
   while(count&&frame>Lookahead&&peaks[head].frame<frame-Lookahead){head=(head+1)%QueueSize;--count;}
   float peak=std::max(std::abs(l),std::abs(r));
   while(count&&peaks[(head+count-1)%QueueSize].value<=peak)--count;
   peaks[(head+count)%QueueSize]={frame,peak};++count;
   float largest=peaks[head].value;
   float needed=largest>ceiling?ceiling/largest:1.f;
   gain=std::min(needed,gain+release*(1-gain));
   auto delayed=audio[position];audio[position]={l,r};position=(position+1)%Lookahead;
   data[2*i]=delayed[0]*gain;data[2*i+1]=delayed[1]*gain;++frame;
  }
 }
};
}
