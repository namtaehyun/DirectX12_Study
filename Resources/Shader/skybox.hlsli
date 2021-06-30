#ifndef _DEFAULT_HLSLI_
#define _DEFAULT_HLSLI_

#include "params.hlsli"

struct VS_IN
{
	float3 localPos : POSITION;			// Local ��ǥ�� �������� �� pos
	float2 uv : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	// Translation�� ���� x ������ ȸ���� ������ ��. �׷���, ������ w���� 0���� ����(x,y,z,w) x,y,z = localpos
	float4 viewPos = mul(float4(input.localPos, 0.f), g_matView);
	// View -> projection 
	float4 clipSpacePos = mul(viewPos, g_matProjection);

	// w/w =1 �̱� ������ �׻� ���̰� 1�� �����ȴ�.
	output.pos = clipSpacePos.xyww;			// z���� w�� ���. �׷� ��� �Ѿ�� �� ���̰��� ������ 1�� ���õ�.
	output.uv = input.uv;

	return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
	float4 color = g_tex_0.Sample(g_sam_0, input.uv);
	return color;
}

#endif