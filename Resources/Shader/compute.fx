#ifndef _COMPUTE_FX_
#define _COMPUTE_FX_

#include "params.fx"

RWTexture2D<float4> g_rwtex0 : register(u0);		// uav(Unordered Access View) - ComputeShader 전용으로 사용하는 레지스터
//RW의 경우 Read/Write모두 가능. RW가 아닌애들은 읽기전용 - RWTexture2D

// 쓰레드 그룹당 쓰레드 개수
// max : 1024 (CS_5.0)
// 하나의 쓰레드 그룹은 하나의 다중처리기에서 실행
[numthreads(1024, 1, 1)]		// numthreads[x, y, z] 에서 x * y * z가 1024일때 최대임.
void CS_Main(int3 threadIndex : SV_DispatchThreadID)
{
	if (threadIndex.y % 2 == 0)
		g_rwtex0[threadIndex.xy] = float4(1.f, 0.f, 0.f, 1.f);
	else
		g_rwtex0[threadIndex.xy] = float4(0.f, 1.f, 0.f, 1.f);
}

#endif