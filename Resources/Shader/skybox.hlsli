#ifndef _DEFAULT_HLSLI_
#define _DEFAULT_HLSLI_

#include "params.hlsli"

struct VS_IN
{
	float3 localPos : POSITION;			// Local 좌표계 기준으로 한 pos
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

	// Translation은 적용 x 오로지 회전만 적용할 것. 그래서, 마지막 w값을 0으로 놓음(x,y,z,w) x,y,z = localpos
	float4 viewPos = mul(float4(input.localPos, 0.f), g_matView);
	// View -> projection 
	float4 clipSpacePos = mul(viewPos, g_matProjection);

	// w/w =1 이기 때문에 항상 깊이가 1로 유지된다.
	output.pos = clipSpacePos.xyww;			// z값도 w로 덮어씀. 그럴 경우 넘어갔을 때 깊이값이 무조건 1로 셋팅됨.
	output.uv = input.uv;

	return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
	float4 color = g_tex_0.Sample(g_sam_0, input.uv);
	return color;
}

#endif