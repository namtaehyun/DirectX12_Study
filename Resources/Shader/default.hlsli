#ifndef _DEFAULT_HLSLI_
#define _DEFAULT_HLSLI_

#include "params.hlsli"

struct VS_IN					// VertexShader
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct VS_OUT
{
	float4 pos : SV_Position;					// 얘는 무조건 SV_Position이라는 이름을 써야함. ms에서 그렇게 정했음.
	float2 uv : TEXCOORD;
	float3 viewPos : POSITION;				// View 좌표계에서의 Pos
	float3 viewNormal : NORMAL;			// View 좌표계에서의 Normal
};

VS_OUT VS_Main(VS_IN input)
{
	// 정점단위로 계산

	VS_OUT output = (VS_OUT)0;

	output.pos = mul(float4(input.pos, 1.f), g_matWVP);
	output.uv = input.uv;
	// 정점좌표를 계산하는 거

	output.viewPos = mul(float4(input.pos, 1.f), g_matWV).xyz;								// 투영 좌표계로 넘어가는게 아니라, WV까지만 곱해줌.
	output.viewNormal = normalize(mul(float4(input.normal, 0.f), g_matWV).xyz);		// 방향벡터의 경우 마지막 벡터를 0으로 설정해줘야 읻ㅇ에 적용안됨.

	return output;
}

float4 PS_Main(VS_OUT input) : SV_Target		// PixelShader
{
	// 픽셀 단위로 계산 Rasterize
	float4 color = tex_0.Sample(g_sam_0, input.uv);
	return color;
}
#endif 
