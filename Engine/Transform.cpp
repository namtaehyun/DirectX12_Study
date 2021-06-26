#include "pch.h"
#include "Transform.h"
#include "Engine.h"
#include "Camera.h"

Transform::Transform() : Component(COMPONENT_TYPE::TRANSFORM)
{

}

Transform::~Transform()
{

}

void Transform::FinalUpdate()	// 물체의 좌표와 회전 상태, Scale에 따라 world가 결정됨
{
	// SRT(Scale,Rotation,Transform)를 셋팅해놓았다는 가정하에
	Matrix matScale			= Matrix::CreateScale(_localscale);	
	Matrix matRotation		= Matrix::CreateRotationX(_localrotation.x);
	matRotation				*= Matrix::CreateRotationY(_localrotation.y);
	matRotation				*= Matrix::CreateRotationZ(_localrotation.z);			// 회전행렬 만들 때 이렇게 하는게 아니라, Quaternion으로 하는게 맞음.
	Matrix matTranslation	= Matrix::CreateTranslation(_localposition);

	_matLocal = matScale * matRotation * matTranslation;		// 부모의 Local로 가기 위한 행렬
	_matWorld = _matLocal;											// 월드로 가는 행렬

	shared_ptr<Transform> parent = GetParent().lock();
	if (parent != nullptr)
	{
		_matWorld *= parent->GetLocalToWorldMatrix();		// 부모의 Local로 가는단계
	}
}

void Transform::PushData()
{
	// 데이터를 최종적으로 건네주기 위한 일련의 작업

	// [W] VP -VP는 Camera Component 에서 관리함.
	// TODO

	Matrix matWVP = _matWorld * Camera::S_MatView * Camera::S_MatProjection;
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&_matWorld, sizeof(_matWorld));

}

// CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&_transform, sizeof(_transform));