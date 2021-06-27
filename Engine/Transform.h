#pragma once
#include "Component.h"


class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	// TODO : 온갖 Parent/Child 관계

	virtual void					FinalUpdate() override;
	void							PushData();

	const Vec3&					GetLocalPosition() { return _localposition; }				// Parent를 기준으로 한 Position
	const Vec3&					GetLocalRotation() { return _localrotation; }				// Parent를 기준으로 한 Rotation
	const Vec3&					GetLocalScale() { return _localscale; }						// Parent를 기준으로 한 Scale

	const Matrix&				GetLocalToWorldMatrix() { return _matWorld; }			// 진짜 물체 자체의 World로 뛰어넘기 위한 행렬
	const Vec3&					GetWorldPosition() { return _matWorld.Translation(); }

	Vec3							GetRight() { return _matWorld.Right(); }
	Vec3							GetUp() { return _matWorld.Up(); }
	Vec3							GetLook() { return _matWorld.Backward(); }		// 참조를 하지 않은이유? - 사용중인 함수도 참조안했음.

	void							SetLocalPosition(const Vec3& position) { _localposition = position; }
	void							SetLocalRotation(const Vec3& rotation) { _localrotation = rotation; }
	void							SetLocalScale(const Vec3& scale) { _localscale = scale; }

public:
	void							SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	weak_ptr<Transform>		GetParent() { return _parent; }
private:
	// TODO : World 위치 관련

	//Parent 기준 Local 좌표
	Vec3 _localposition		= {}; 
	Vec3 _localrotation		= {};
	Vec3 _localscale			= {1.f, 1.f, 1.f};

	Matrix _matLocal		= {};		// 부모의 Local을 기준으로 한 행렬
	Matrix _matWorld		= {};		// world를 기준으로 한 행렬

	weak_ptr<Transform> _parent;
};

