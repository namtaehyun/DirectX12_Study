#pragma once
#include "Component.h"


class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	// TODO : �°� Parent/Child ����

	virtual void					FinalUpdate() override;
	void							PushData();

	const Vec3&					GetLocalPosition() { return _localposition; }				// Parent�� �������� �� Position
	const Vec3&					GetLocalRotation() { return _localrotation; }				// Parent�� �������� �� Rotation
	const Vec3&					GetLocalScale() { return _localscale; }						// Parent�� �������� �� Scale

	const Matrix&				GetLocalToWorldMatrix() { return _matWorld; }			// ��¥ ��ü ��ü�� World�� �پ�ѱ� ���� ���
	const Vec3&					GetWorldPosition() { return _matWorld.Translation(); }

	Vec3							GetRight() { return _matWorld.Right(); }
	Vec3							GetUp() { return _matWorld.Up(); }
	Vec3							GetLook() { return _matWorld.Backward(); }		// ������ ���� ��������? - ������� �Լ��� ����������.

	void							SetLocalPosition(const Vec3& position) { _localposition = position; }
	void							SetLocalRotation(const Vec3& rotation) { _localrotation = rotation; }
	void							SetLocalScale(const Vec3& scale) { _localscale = scale; }

public:
	void							SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	weak_ptr<Transform>		GetParent() { return _parent; }
private:
	// TODO : World ��ġ ����

	//Parent ���� Local ��ǥ
	Vec3 _localposition		= {}; 
	Vec3 _localrotation		= {};
	Vec3 _localscale			= {1.f, 1.f, 1.f};

	Matrix _matLocal		= {};		// �θ��� Local�� �������� �� ���
	Matrix _matWorld		= {};		// world�� �������� �� ���

	weak_ptr<Transform> _parent;
};

