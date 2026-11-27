#pragma once
#include <array>
#include <complex>
#include <numbers>
#include <cmath>
#include <vector>
#include <stdexcept>
// Header-only radix-2 FFT. All scratch storage belongs to the caller.
namespace hq {
template<class Buffer> void fftBuffer(Buffer& a, bool inverse=false) {
 const size_t N=a.size();
 for(size_t i=1,j=0;i<N;i++) { size_t b=N>>1; for(;j&b;b>>=1) j^=b; j^=b; if(i<j) std::swap(a[i],a[j]); }
 for(size_t len=2;len<=N;len<<=1) { auto wlen=std::polar(1.f,(inverse?2.f:-2.f)*std::numbers::pi_v<float>/float(len));
  for(size_t i=0;i<N;i+=len) { std::complex<float> w=1; for(size_t j=0;j<len/2;j++) { auto u=a[i+j],v=a[i+j+len/2]*w; a[i+j]=u+v; a[i+j+len/2]=u-v; w*=wlen; } }
 } if(inverse) for(auto& v:a) v/=float(N);
}
template<size_t N> void fft(std::array<std::complex<float>,N>& a,bool inverse=false){
 static_assert(N>0 && (N&(N-1))==0);fftBuffer(a,inverse);
}
inline void fft(std::vector<std::complex<float>>& a,bool inverse=false){
 if(a.empty() || (a.size()&(a.size()-1)))throw std::invalid_argument("FFT size must be a power of two");
 fftBuffer(a,inverse);
}
}
