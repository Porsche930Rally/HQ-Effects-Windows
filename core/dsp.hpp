#pragma once
#include "fft.hpp"
#include "partitioned_convolution.hpp"
#include <vector>
#include <algorithm>
namespace hq {
struct Biquad {
 float b0=1,b1=0,b2=0,a1=0,a2=0,z1=0,z2=0;
 float process(float x){float y=b0*x+z1;z1=b1*x-a1*y+z2;z2=b2*x-a2*y;return y;}
 void set(float B0,float B1,float B2,float A0,float A1,float A2){b0=B0/A0;b1=B1/A0;b2=B2/A0;a1=A1/A0;a2=A2/A0;}
 void peak(float hz,float db,float rate,float q=4.318f){float w=2*std::numbers::pi_v<float>*std::min(hz,rate*.45f)/rate,c=std::cos(w),alpha=std::sin(w)/(2*q),A=std::pow(10.f,db/40);set(1+alpha*A,-2*c,1-alpha*A,1+alpha/A,-2*c,1-alpha/A);}
 void pass(float hz,bool high,float rate){float w=2*std::numbers::pi_v<float>*hz/rate,c=std::cos(w),alpha=std::sin(w)/std::sqrt(2.f),k=high?(1+c)/2:(1-c)/2;set(k,high?-2*k:2*k,k,1+alpha,-2*c,1-alpha);}
};
struct Delay {std::array<float,128> data{};size_t pos=0;float process(float x,float delay){data[pos]=x;float p=float(pos)+128-std::clamp(delay,0.f,126.f);size_t k=size_t(p);float f=p-k;float y=data[k%128]*(1-f)+data[(k+1)%128]*f;pos=(pos+1)%128;return y;}};
}
