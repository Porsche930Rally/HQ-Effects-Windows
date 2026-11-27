// SPDX-License-Identifier: GPL-3.0-or-later
// Minimal independently declared binary interface; no Steinberg SDK is distributed.
#pragma once
#include <cstdint>
#include <cstddef>
namespace abi {
struct Effect;
using Callback=intptr_t(*)(Effect*,int32_t,int32_t,intptr_t,void*,float);
using Process=void(*)(Effect*,float**,float**,int32_t);
struct Effect {
 int32_t magic;Callback dispatch;Process process;void(*set)(Effect*,int32_t,float);float(*get)(Effect*,int32_t);
 int32_t programs,parameters,inputs,outputs,flags;intptr_t reserved1,reserved2;int32_t latency,quality1,quality2;float ratio;
 void*object;void*user;int32_t id,version;Process replacing;void(*doubleReplacing)(Effect*,double**,double**,int32_t);char future[56];
};
static_assert(sizeof(void*)!=8||sizeof(Effect)==192);
static_assert(sizeof(void*)!=8||offsetof(Effect,replacing)==120);
}
