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
	float4 pos : SV_Position;					// ��� ������ SV_Position�̶�� �̸��� �����. ms���� �׷��� ������.
	float2 uv : TEXCOORD;
	float3 viewPos : POSITION;				// View ��ǥ�迡���� Pos
	float3 viewNormal : NORMAL;			// View ��ǥ�迡���� Normal
};

VS_OUT VS_Main(VS_IN input)
{
	// ���������� ���

	VS_OUT output = (VS_OUT)0;

	output.pos = mul(float4(input.pos, 1.f), g_matWVP);
	output.uv = input.uv;
	// ������ǥ�� ����ϴ� ��

	output.viewPos = mul(float4(input.pos, 1.f), g_matWV).xyz;								// ���� ��ǥ��� �Ѿ�°� �ƴ϶�, WV������ ������.
	output.viewNormal = normalize(mul(float4(input.normal, 0.f), g_matWV).xyz);		// ���⺤���� ��� ������ ���͸� 0���� ��������� �ޤ��� ����ȵ�.

	return output;
}

float4 PS_Main(VS_OUT input) : SV_Target		// PixelShader
{
	// �ȼ� ������ ��� Rasterize
	float4 color = tex_0.Sample(g_sam_0, input.uv);
	return color;
}
#endif 
