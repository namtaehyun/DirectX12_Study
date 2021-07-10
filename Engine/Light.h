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

	virtual void								FinalUpdate() override;
	void										Render();
	void										RenderShadow();		// ���� �ٶ󺸰� �ִ� �������� ī�޶� ��°�
public:
	LIGHT_TYPE								GetLightType() { return static_cast<LIGHT_TYPE>(_lightInfo.lightType); }
	const LightInfo&						GetLightInfo() { return _lightInfo; }

	void										SetLightDirection(const Vec3 direction);

	void										SetDiffuse(const Vec3& diffuse)		{ _lightInfo.color.diffuse = diffuse; }
	void										SetAmbient(const Vec3& ambient)	{ _lightInfo.color.ambient = ambient; }
	void										SetSpecular(const Vec3& specular)	{ _lightInfo.color.specular = specular; }

	void										SetLightType(LIGHT_TYPE type);
	void										SetLightRange(float range) { _lightInfo.range = range; }
	void										SetLightAngle(float angle) { _lightInfo.angle = angle; }

	void										SetLightIndex(int8 index) { _lightIndex = index; }

private:
	LightInfo									_lightInfo = {};

	int8										_lightIndex = -1;
	shared_ptr<class Mesh>				_volumeMesh;
	shared_ptr<class Material>			_lightMaterial;

	shared_ptr<GameObject>			_shadowCamera;		// Shadow�� ���� Cam
};

