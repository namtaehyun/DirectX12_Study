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

	TransformParams transformParams = {};
	transformParams.matWorld = _matWorld;
	transformParams.matView = Camera::S_MatView;
	transformParams.matProjection = Camera::S_MatProjection;
	transformParams.matWV = _matWorld * Camera::S_MatView;
	transformParams.matWVP = _matWorld * Camera::S_MatView * Camera::S_MatProjection;
	transformParams.matViewInv = Camera::S_MatView.Invert();

	CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushGraphicsData(&transformParams, sizeof(transformParams));
}

void Transform::LookAt(const Vec3& dir)	
{
	Vec3 front = dir;				// 쳐다보고싶은 방향을 front로 함. 이걸 z로 유도할 것.
	front.Normalize();

	Vec3 right = Vec3::Up.Cross(dir);
	if (right == Vec3::Zero)
		right = Vec3::Forward.Cross(dir);

	right.Normalize();

	Vec3 up = front.Cross(right);
	up.Normalize();

	Matrix matrix = XMMatrixIdentity();
	matrix.Right(right);
	matrix.Up(up);
	matrix.Backward(front);		// 오른손좌표계라서 look벡터가 backward임.

	_localrotation = DecomposeRotationMatrix(matrix);
}

bool Transform::CloseEnough(const float& a, const float& b, const float& epsilon)
{
	return (epsilon > std::abs(a-b));
}

Vec3 Transform::DecomposeRotationMatrix(const Matrix& rotation)		// 짐벌락까지 없앤 회전.
{
	Vec4 v[4];
	XMStoreFloat4(&v[0], rotation.Right());
	XMStoreFloat4(&v[1], rotation.Up());
	XMStoreFloat4(&v[2], rotation.Backward());
	XMStoreFloat4(&v[3], rotation.Translation());

	Vec3 ret;

	if (CloseEnough(v[0].z, -1.0f))
	{
		float x = 0;
		float y = XM_PI / 2;
		float z = x + atan2(v[1].x, v[2].x);
		ret = Vec3{ x,y,z };
	}
	else if (CloseEnough(v[0].z, 1.0f))
	{
		float x = 0;
		float y = -XM_PI / 2;
		float z = -x + atan2(v[1].x, v[2].x);
		ret = Vec3{ x,y,z };
	}
	else
	{
		float y1 = -asin(v[0].z);
		float y2 = XM_PI - y1;

		float x1 = atan2f(v[1].z / cos(y1), v[2].z / cos(y1));
		float x2 = atan2f(v[1].z / cos(y2), v[2].z / cos(y2));

		float z1 = atan2f(v[0].y / cos(y1), v[0].x / cos(y1));
		float z2 = atan2f(v[0].y / cos(y2), v[0].x / cos(y2));

		if((std::abs(x1)+std::abs(y1)+ std::abs(z1)) <= (std::abs(x2) + std::abs(y2) + std::abs(z2)))
		{
			ret = Vec3{ x1,y1,z1 };
		}
		else
		{
			ret = Vec3{ x2,y2,z2 };
		}
	}
	   
	return ret;
}
// CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&_transform, sizeof(_transform));