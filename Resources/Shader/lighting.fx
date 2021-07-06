#ifndef _LIGHTING_FX_
#define _LIGHTING_FX_

#include "params.fx"
#include "utils.fx"

struct VS_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD;
};

struct PS_OUT
{
	float4 diffuse : SV_Target0;
	float4 specular : SV_Target1;
};

// [Directional Light]
// g_int_0 : Light index
// g_tex_0 : Position RT
// g_tex_1 : Normal RT
// Mesh : Rectangle

VS_OUT VS_DirLight(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos * 2.f, 1.f);		// rectangle이 -0.5 ~ 0.5인데, 화면크기를 -1 ~ 1로 만들어 
	output.uv = input.uv;							  // 전체화면으로 만들어주고싶음. -- 전체화면이기 때문에 -1 ~ 1까지 되어야함.

	return output;
}

PS_OUT PS_DirLight(VS_OUT input)
{
	PS_OUT output = (PS_OUT)0;

	float3 viewPos = g_tex_0.Sample(g_sam_0, input.uv).xyz;
	if (viewPos.z <= 0.f)			// 물체가 있는지 없는지 확인. viewPos의 z가 0보다 작으면 없는거랑 마찬가지. (안 찍을거)
		clip(-1);					   // 없으면 스킵 (clip에 0보다 작은수를 넘겨주면 그냥 여기서 끝내버림. return과 마찬가지.

	float3 viewNormal = g_tex_1.Sample(g_sam_0, input.uv).xyz;

	// 있으면 normal을 추출해서 LightIndex, Normal과 viewPos를 이용해 빛의 색을 계산해서 리턴
	LightColor color = CalculateLightColor(g_int_0, viewNormal, viewPos);	// tex1번에 normal
	output.diffuse = color.diffuse + color.ambient;
	output.specular = color.specular;

	return output;
}

// [Point Light]
// g_int_0 : Light index
// g_tex_0 : Position RT
// g_tex_1 : Normal RT
// g_vec2_0 : RenderTarget Resolution
// Mesh : Sphere

VS_OUT VS_PointLight(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = mul(float4(input.pos, 1.f), g_matWVP);
	output.uv = input.uv;

	return output;
}

PS_OUT PS_PointLight(VS_OUT input)
{
	PS_OUT output = (PS_OUT)0;

	// input.pos = SV_Position = Screen 좌표
	float2 uv = float2(input.pos.x / g_vec2_0.x, input.pos.y / g_vec2_0.y);
	float3 viewPos = g_tex_0.Sample(g_sam_0, uv).xyz;
	if (viewPos.z <= 0.f)
		clip(-1);

	int lightIndex = g_int_0;
	float3 viewLightPos = mul(float4(g_light[lightIndex].position.xyz, 1.f), g_matView).xyz;
	float distance = length(viewPos - viewLightPos);// 해당 물체와 광원사이의 거리
	if (distance > g_light[lightIndex].range)			 // 광원 범위보다 크면 안그려줌.
		clip(-1);

	float3 viewNormal = g_tex_1.Sample(g_sam_0, uv).xyz;	// Normal 값 추출

	LightColor color = CalculateLightColor(g_int_0, viewNormal, viewPos);	// Light값 계산

	output.diffuse = color.diffuse + color.ambient;
	output.specular = color.specular;

	return output;
}

// [Final]
// g_tex_0 : Diffuse Color Target
// g_tex_1 : Diffuse Light Target
// g_tex_2 : Specular Light Target
// Mesh : Rectangle

VS_OUT VS_Final(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos * 2.f, 1.f);
	output.uv = input.uv;

	return output;
}

float4 PS_Final(VS_OUT input) : SV_Target
{
	float4 output = (float4)0;

	float4 lightPower = g_tex_1.Sample(g_sam_0, input.uv);// diffuseLight를 뽑아서 계산
	if (lightPower.x == 0.f && lightPower.y == 0.f && lightPower.z == 0.f)
		clip(-1);				// 얘는 영향 안받음. 그러니 패스

	float4 color = g_tex_0.Sample(g_sam_0, input.uv);
	float4 specular = g_tex_2.Sample(g_sam_0, input.uv);

	output = (color * lightPower) + specular;
	return output;
}

#endif