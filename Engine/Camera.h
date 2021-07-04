#pragma once
#include "Component.h"
#include "Frustum.h"

enum class PROJECTION_TYPE
{
	PERSPECTIVE,			// 원근 투영
	ORTHOGRAPHIC,		// 직교 투영 - 원근법 x 
};

class Camera : public Component
{
public:
	Camera();
	virtual	~Camera();

	virtual void				FinalUpdate() override;

	void						SetProjectionType(PROJECTION_TYPE type) { _type = type; }
	PROJECTION_TYPE		GetProJectionType() { return _type; }

	void						SortGameObject();
	void						Render_Deferred();
	void						Render_Forward();

	void						SetCullingMaskLayerOnOff(uint8 layer, bool on)
	{
		if (on)
			_cullingMask |= (1 << layer);
		else
			_cullingMask &= ~(1 << layer);
	}

	void						SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }	// 1111 1111 1111 ... Layer의 모든 비트에 대해 On해줌
	void						SetCullingMask(uint32 mask) { _cullingMask = mask; }
	bool						IsCulled(uint8 layer) { return (_cullingMask & (1 << layer)) != 0; }

private:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float			_near = 1.f;
	float			_far = 1000.f;
	float			_fov = XM_PI / 4.f;
	float			_scale = 1.f;

	Matrix		_matView = {};			// View 행렬
	Matrix		_matProjection = {};		// Projection 행렬

	Frustum		_frustum;
	uint32		_cullingMask = 0;		// bit로 두고 UI를 Culling 할지말지 정해주는 것.
public:
	vector<shared_ptr<GameObject>> _vecDeferred;
	vector<shared_ptr<GameObject>> _vecForward;

public:
	// 임시 변수
	static Matrix S_MatView;
	static Matrix S_MatProjection;
};

