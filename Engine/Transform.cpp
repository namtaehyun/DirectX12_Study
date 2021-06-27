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

void Transform::FinalUpdate()	// ��ü�� ��ǥ�� ȸ�� ����, Scale�� ���� world�� ������
{
	// SRT(Scale,Rotation,Transform)�� �����س��Ҵٴ� �����Ͽ�
	Matrix matScale			= Matrix::CreateScale(_localscale);	
	Matrix matRotation		= Matrix::CreateRotationX(_localrotation.x);
	matRotation				*= Matrix::CreateRotationY(_localrotation.y);
	matRotation				*= Matrix::CreateRotationZ(_localrotation.z);			// ȸ����� ���� �� �̷��� �ϴ°� �ƴ϶�, Quaternion���� �ϴ°� ����.
	Matrix matTranslation	= Matrix::CreateTranslation(_localposition);

	_matLocal = matScale * matRotation * matTranslation;		// �θ��� Local�� ���� ���� ���
	_matWorld = _matLocal;											// ����� ���� ���

	shared_ptr<Transform> parent = GetParent().lock();
	if (parent != nullptr)
	{
		_matWorld *= parent->GetLocalToWorldMatrix();		// �θ��� Local�� ���´ܰ�
	}
}

void Transform::PushData()
{
	// �����͸� ���������� �ǳ��ֱ� ���� �Ϸ��� �۾�

	// [W] VP -VP�� Camera Component ���� ������.
	// TODO

	TransformParams transformParams = {};
	transformParams.matWorld = _matWorld;
	transformParams.matView = Camera::S_MatView;
	transformParams.matProjection = Camera::S_MatProjection;
	transformParams.matWV = _matWorld * Camera::S_MatView;
	transformParams.matWVP = _matWorld * Camera::S_MatView * Camera::S_MatProjection;

	CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&transformParams, sizeof(transformParams));
}

// CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&_transform, sizeof(_transform));