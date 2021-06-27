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
	Vec4 specular;							// Packing rule이 귀찮으니 16byte로 떨어지도록 Vec4를 사용하는 것.
};

struct LightInfo
{
	LightColor		color;
	Vec4				position;				//Point Light, Spot Light에서 의미있음.
	Vec4				direction;				//Point에서 안씀
	int32				lightType;			//여기부터
	float				range;
	float				angle;
	int32				padding;				// 여기까지 16byte가 되도록 int32를 하나 낑겨 넣음.
	
	// Data의 사이즈를 16바이트로 만들어주기 위해 존재 -- 기본적으로 HLSL에서는 16byte 바운더리를 넘어가면 안됨
	// 자세한건 Packing rule을 보면 됨. 잘못하면 데이터낭비가 심할 수 있음. https://www.programmersought.com/article/41834061891/
};

struct LightParams							// 보통 한번에 전부 셋팅함.
{
	uint32		lightCount;				// 빛의 갯수
	Vec3			padding;
	LightInfo		lights[50];				// 최대 50개까지 빛 사용할 수 있음.
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

