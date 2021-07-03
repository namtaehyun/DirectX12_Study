#ifndef _UTILS_FX_
#define _UTILS_FX_

#include "params.fx"

LightColor CalculateLightColor(int lightIndex, float3 viewNormal, float3 viewPos)
{
	LightColor color = (LightColor)0.f;

	float3 viewLightDir = (float3)0.f;

	float diffuseRatio = 0.f;
	float specularRatio = 0.f;
	float distanceRatio = 1.f;			//������ ��� ���(spot, point light)

	if (g_light[lightIndex].lightType == 0)
	{
		// Directional Light
		viewLightDir = normalize(mul(float4(g_light[lightIndex].direction.xyz, 0.f), g_matView).xyz);
		// world���� Direction�� view�������� �ٲ��ְ� �������ͷ� ��ȯ.
		diffuseRatio = saturate(dot(-viewLightDir, viewNormal));
		// cos(theta)�� ���ϴ� ����. ���� �� saturate���ָ� 0~1���� ���ڷ� �о���. (�����ϰ�� 0)
	}
	else if (g_light[lightIndex].lightType == 1)
	{
		// Point Light
		float3 viewLightPos = mul(float4(g_light[lightIndex].position.xyz, 1.f), g_matView).xyz;		// �ش� ��ü�� ��ǥ(������ ��ġ) * View���
		viewLightDir = normalize(viewPos - viewLightPos);													// �������ͷ� ����. �߽�->�� �����ڸ�
		diffuseRatio = saturate(dot(-viewLightDir, viewNormal));

		float dist = distance(viewPos, viewLightPos);			// ������ ���������� �ȼ������� �Ÿ�
		// ������ ����� �Ⱥ������ϰ�, �־������� ����������.
		if (g_light[lightIndex].range == 0.f)						// range�� 0�̸� ������ 0
			distanceRatio = 0.f;
		else
			distanceRatio = saturate(1.f - pow(dist / g_light[lightIndex].range, 2));		// �־������� ���� ���� ������.
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
			float halfAngle = g_light[lightIndex].angle / 2;											// �þ߰� ����. -- �ִ밢��/2

			float3 viewLightVec = viewPos - viewLightPos;																	//�̹� �������ֱ��ѵ� �ѹ��� ����.
			float3 viewCenterLightDir = normalize(mul(float4(g_light[lightIndex].direction.xyz, 0.f), g_matView).xyz);		// �߾ӿ� �ִ� �� * view

			float centerDist = dot(viewLightVec, viewCenterLightDir);					 // �ִ� ������ ������� �ȹ������ üũ�ϱ� ����.
			distanceRatio = saturate(1.f - centerDist / g_light[lightIndex].range);

			float lightAngle = acos(dot(normalize(viewLightVec), viewCenterLightDir));

			if (centerDist < 0.f || centerDist > g_light[lightIndex].range) // �ִ� �Ÿ��� ������� - �ȱ׸�
				distanceRatio = 0.f;
			else if (lightAngle > halfAngle) // �ִ� �þ߰��� ������� - �ȱ׸�
				distanceRatio = 0.f;
			else // �Ÿ��� ���� ������ ���⸦ ����
				distanceRatio = saturate(1.f - pow(centerDist / g_light[lightIndex].range, 2));
		}
	}

	float3 reflectionDir = normalize(viewLightDir + 2 * (saturate(dot(-viewLightDir, viewNormal)) * viewNormal));
	// ���� ������ ������ ���̳ʽ��� normal���͸� �������ذſ� �ٽ� ���̳ʽ��� ���� �� 2������ �ݻ纤�Ͱ� ����.
	float3 eyeDir = normalize(viewPos);
	// camera ����
	specularRatio = saturate(dot(-eyeDir, reflectionDir));		// ���� �� ���͸� �����ؼ� 0~1���������� ��
	specularRatio = pow(specularRatio, 2);						// ratio�� ���� -> ���� ���ĸ��� �ϰ� �; ������ ��.

	color.diffuse = g_light[lightIndex].color.diffuse * diffuseRatio * distanceRatio;				// ���� ������ �ִ� ���� ���� cos(theta)�� ������.
	color.ambient = g_light[lightIndex].color.ambient * distanceRatio;
	color.specular = g_light[lightIndex].color.specular * specularRatio * distanceRatio;

	return color;
}
#endif