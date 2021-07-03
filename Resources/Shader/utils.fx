#ifndef _UTILS_FX_
#define _UTILS_FX_

#include "params.fx"

LightColor CalculateLightColor(int lightIndex, float3 viewNormal, float3 viewPos)
{
	LightColor color = (LightColor)0.f;

	float3 viewLightDir = (float3)0.f;

	float diffuseRatio = 0.f;
	float specularRatio = 0.f;
	float distanceRatio = 1.f;			//범위를 벗어날 경우(spot, point light)

	if (g_light[lightIndex].lightType == 0)
	{
		// Directional Light
		viewLightDir = normalize(mul(float4(g_light[lightIndex].direction.xyz, 0.f), g_matView).xyz);
		// world기준 Direction을 view공간으로 바꿔주고 단위벡터로 변환.
		diffuseRatio = saturate(dot(-viewLightDir, viewNormal));
		// cos(theta)를 구하는 것임. 내적 후 saturate해주면 0~1까지 숫자로 밀어줌. (음수일경우 0)
	}
	else if (g_light[lightIndex].lightType == 1)
	{
		// Point Light
		float3 viewLightPos = mul(float4(g_light[lightIndex].position.xyz, 1.f), g_matView).xyz;		// 해당 물체의 좌표(광원의 위치) * View행렬
		viewLightDir = normalize(viewPos - viewLightPos);													// 단위벡터로 만듬. 중심->원 가장자리
		diffuseRatio = saturate(dot(-viewLightDir, viewNormal));

		float dist = distance(viewPos, viewLightPos);			// 광원의 시작점부터 픽셀까지의 거리
		// 범위를 벗어나면 안보여야하고, 멀어질수록 약해져야함.
		if (g_light[lightIndex].range == 0.f)						// range가 0이면 무조건 0
			distanceRatio = 0.f;
		else
			distanceRatio = saturate(1.f - pow(dist / g_light[lightIndex].range, 2));		// 멀어질수록 빛이 점점 약해짐.
	}
	else
	{
		// Spot Light
		float3 viewLightPos = mul(float4(g_light[lightIndex].position.xyz, 1.f), g_matView).xyz;
		viewLightDir = normalize(viewPos - viewLightPos);
		diffuseRatio = saturate(dot(-viewLightDir, viewNormal));

		if (g_light[lightIndex].range == 0.f)
			distanceRatio = 0.f;
		else
		{
			float halfAngle = g_light[lightIndex].angle / 2;											// 시야각 구함. -- 최대각도/2

			float3 viewLightVec = viewPos - viewLightPos;																	//이미 구해져있긴한데 한번더 구함.
			float3 viewCenterLightDir = normalize(mul(float4(g_light[lightIndex].direction.xyz, 0.f), g_matView).xyz);		// 중앙에 있는 빛 * view

			float centerDist = dot(viewLightVec, viewCenterLightDir);					 // 최대 범위를 벗어났는지 안벗어났는지 체크하기 위함.
			distanceRatio = saturate(1.f - centerDist / g_light[lightIndex].range);

			float lightAngle = acos(dot(normalize(viewLightVec), viewCenterLightDir));

			if (centerDist < 0.f || centerDist > g_light[lightIndex].range) // 최대 거리를 벗어났는지 - 안그림
				distanceRatio = 0.f;
			else if (lightAngle > halfAngle) // 최대 시야각을 벗어났는지 - 안그림
				distanceRatio = 0.f;
			else // 거리에 따라 적절히 세기를 조절
				distanceRatio = saturate(1.f - pow(centerDist / g_light[lightIndex].range, 2));
		}
	}

	float3 reflectionDir = normalize(viewLightDir + 2 * (saturate(dot(-viewLightDir, viewNormal)) * viewNormal));
	// 빛이 들어오는 방향의 마이너스와 normal벡터를 내적해준거에 다시 마이너스를 곱한 후 2번가면 반사벡터가 나옴.
	float3 eyeDir = normalize(viewPos);
	// camera 벡터
	specularRatio = saturate(dot(-eyeDir, reflectionDir));		// 위의 두 벡터를 내적해서 0~1까지값으로 댕강
	specularRatio = pow(specularRatio, 2);						// ratio에 제곱 -> 빛을 가파르게 하고 싶어서 제곱한 것.

	color.diffuse = g_light[lightIndex].color.diffuse * diffuseRatio * distanceRatio;				// 원래 가지고 있던 빛에 대해 cos(theta)를 곱해줌.
	color.ambient = g_light[lightIndex].color.ambient * distanceRatio;
	color.specular = g_light[lightIndex].color.specular * specularRatio * distanceRatio;

	return color;
}
#endif