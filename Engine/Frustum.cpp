#include "pch.h"
#include "Frustum.h"
#include "Camera.h"

void Frustum::FinalUpdate()
{
	// Projection -> View -> World로 역으로 가야하기 때문에, 역행렬을 두번 곱해서 Inv행렬을 만들어준다.

	Matrix matViewInv = Camera::S_MatView.Invert();
	Matrix matProjectionInv = Camera::S_MatProjection.Invert();
	Matrix matInv = matProjectionInv * matViewInv;

	vector<Vec3> worldPos =
	{
		::XMVector3TransformCoord(Vec3(-1.f, 1.f, 0.f), matInv),			// 이 함수는 벡터에 행렬을 곱해주는 함수 (Z를 1로 셋팅하기 위해)
		::XMVector3TransformCoord(Vec3(1.f, 1.f, 0.f), matInv),
		::XMVector3TransformCoord(Vec3(1.f, -1.f, 0.f), matInv),
		::XMVector3TransformCoord(Vec3(-1.f, -1.f, 0.f), matInv),
		::XMVector3TransformCoord(Vec3(-1.f, 1.f, 1.f), matInv),
		::XMVector3TransformCoord(Vec3(1.f, 1.f, 1.f), matInv),
		::XMVector3TransformCoord(Vec3(1.f, -1.f, 1.f), matInv),
		::XMVector3TransformCoord(Vec3(-1.f, -1.f, 1.f), matInv),
	};

	// 각각 ViewFrustum의 평면에 대해 NormalVector를 제대로 구해주기 위해 다음과 같이 CW / CCW를 정해준다.

	// XMPlaneFromPoints - 점 세 개 중 두 점을 빼서 2개의 벡터를 만들고, 그 두 벡터를 외적하여 NormalVector를 구하고
	// 그 NormalVector와 평면 위의 다른 한 점을 내적해 거리를 구해준다.
	// 그래서 이걸 Vec4(NormalVector{x,y,z}, d)로 넘겨준다.

	_planes[PLANE_FRONT] = ::XMPlaneFromPoints(worldPos[0], worldPos[1], worldPos[2]);		// CW
	_planes[PLANE_BACK] = ::XMPlaneFromPoints(worldPos[4], worldPos[7], worldPos[5]);		// CCW
	_planes[PLANE_UP] = ::XMPlaneFromPoints(worldPos[4], worldPos[5], worldPos[1]);			// CW
	_planes[PLANE_DOWN] = ::XMPlaneFromPoints(worldPos[7], worldPos[3], worldPos[6]);		// CCW
	_planes[PLANE_LEFT] = ::XMPlaneFromPoints(worldPos[4], worldPos[0], worldPos[7]);		// CW
	_planes[PLANE_RIGHT] = ::XMPlaneFromPoints(worldPos[5], worldPos[6], worldPos[1]);		// CCW

}

bool Frustum::ContainSphere(const Vec3& pos, float radius)
{
	for (const Vec4& plane : _planes)
	{
		// normal Vector= a, b, c
		Vec3 normal = Vec3(plane.x, plane.y, plane.z);

		// ax + by + cz + d > radius
		if (normal.Dot(pos) + plane.w > radius)
			return false;
	}

	return true;

}
