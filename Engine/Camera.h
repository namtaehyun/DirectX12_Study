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

	virtual void	FinalUpdate() override;
	void			Render();

private:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float _near = 1.f;
	float _far = 1000.f;
	float _fov = XM_PI / 4.f;
	float _scale = 1.f;

	Matrix _matView = {};			// View 행렬
	Matrix _matProjection = {};		// Projection 행렬

	Frustum _frustum;
public:
	// 임시 변수
	static Matrix S_MatView;
	static Matrix S_MatProjection;
};

