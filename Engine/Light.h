#pragma once
#include "Component.h"

enum class LIGHT_TYPE : uint8
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

struct LightColor
{
	Vec4 diffuse;
	Vec4 ambient;
	Vec4 specular;							// Packing rule�� �������� 16byte�� ���������� Vec4�� ����ϴ� ��.
};

struct LightInfo
{
	LightColor		color;
	Vec4				position;				//Point Light, Spot Light���� �ǹ�����.
	Vec4				direction;				//Point���� �Ⱦ�
	int32				lightType;			//�������
	float				range;
	float				angle;
	int32				padding;				// ������� 16byte�� �ǵ��� int32�� �ϳ� ���� ����.
	
	// Data�� ����� 16����Ʈ�� ������ֱ� ���� ���� -- �⺻������ HLSL������ 16byte �ٿ������ �Ѿ�� �ȵ�
	// �ڼ��Ѱ� Packing rule�� ���� ��. �߸��ϸ� �����ͳ��� ���� �� ����. https://www.programmersought.com/article/41834061891/
};

struct LightParams							// ���� �ѹ��� ���� ������.
{
	uint32		lightCount;				// ���� ����
	Vec3			padding;
	LightInfo		lights[50];				// �ִ� 50������ �� ����� �� ����.
};

class Light : public Component
{
public:
	Light();
	virtual ~Light();

	virtual void FinalUpdate() override;

public:
	const LightInfo& GetLightInfo() { return _lightInfo; }

	void SetLightDirection(const Vec3& direction) { _lightInfo.direction = direction; }

	void SetDiffuse(const Vec3& diffuse)		{ _lightInfo.color.diffuse = diffuse; }
	void SetAmbient(const Vec3& ambient)	{ _lightInfo.color.ambient = ambient; }
	void SetSpecular(const Vec3& specular)	{ _lightInfo.color.specular = specular; }

	void SetLightType(LIGHT_TYPE type) { _lightInfo.lightType = static_cast<int32>(type); }
	void SetLightRange (float range) { _lightInfo.range = range; }
	void SetLightAngle(float angle) { _lightInfo.angle = angle; }

private:
	LightInfo _lightInfo = {};
};

