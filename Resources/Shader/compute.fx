#ifndef _COMPUTE_FX_
#define _COMPUTE_FX_

#include "params.fx"

RWTexture2D<float4> g_rwtex0 : register(u0);		// uav(Unordered Access View) - ComputeShader �������� ����ϴ� ��������
//RW�� ��� Read/Write��� ����. RW�� �ƴѾֵ��� �б����� - RWTexture2D

// ������ �׷�� ������ ����
// max : 1024 (CS_5.0)
// �ϳ��� ������ �׷��� �ϳ��� ����ó���⿡�� ����
[numthreads(1024, 1, 1)]		// numthreads[x, y, z] ���� x * y * z�� 1024�϶� �ִ���.
void CS_Main(int3 threadIndex : SV_DispatchThreadID)
{
	if (threadIndex.y % 2 == 0)
		g_rwtex0[threadIndex.xy] = float4(1.f, 0.f, 0.f, 1.f);
	else
		g_rwtex0[threadIndex.xy] = float4(0.f, 1.f, 0.f, 1.f);
}

#endif