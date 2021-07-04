#pragma once
#include "Component.h"
#include "Frustum.h"

enum class PROJECTION_TYPE
{
	PERSPECTIVE,			// ���� ����
	ORTHOGRAPHIC,		// ���� ���� - ���ٹ� x 
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

	void						SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }	// 1111 1111 1111 ... Layer�� ��� ��Ʈ�� ���� On����
	void						SetCullingMask(uint32 mask) { _cullingMask = mask; }
	bool						IsCulled(uint8 layer) { return (_cullingMask & (1 << layer)) != 0; }

private:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float			_near = 1.f;
	float			_far = 1000.f;
	float			_fov = XM_PI / 4.f;
	float			_scale = 1.f;

	Matrix		_matView = {};			// View ���
	Matrix		_matProjection = {};		// Projection ���

	Frustum		_frustum;
	uint32		_cullingMask = 0;		// bit�� �ΰ� UI�� Culling �������� �����ִ� ��.
public:
	vector<shared_ptr<GameObject>> _vecDeferred;
	vector<shared_ptr<GameObject>> _vecForward;

public:
	// �ӽ� ����
	static Matrix S_MatView;
	static Matrix S_MatProjection;
};

