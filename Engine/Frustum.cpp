#include "pch.h"
#include "Frustum.h"
#include "Camera.h"

void Frustum::FinalUpdate()
{
	// Projection -> View -> World�� ������ �����ϱ� ������, ������� �ι� ���ؼ� Inv����� ������ش�.

	Matrix matViewInv = Camera::S_MatView.Invert();
	Matrix matProjectionInv = Camera::S_MatProjection.Invert();
	Matrix matInv = matProjectionInv * matViewInv;

	vector<Vec3> worldPos =
	{
		::XMVector3TransformCoord(Vec3(-1.f, 1.f, 0.f), matInv),			// �� �Լ��� ���Ϳ� ����� �����ִ� �Լ� (Z�� 1�� �����ϱ� ����)
		::XMVector3TransformCoord(Vec3(1.f, 1.f, 0.f), matInv),
		::XMVector3TransformCoord(Vec3(1.f, -1.f, 0.f), matInv),
		::XMVector3TransformCoord(Vec3(-1.f, -1.f, 0.f), matInv),
		::XMVector3TransformCoord(Vec3(-1.f, 1.f, 1.f), matInv),
		::XMVector3TransformCoord(Vec3(1.f, 1.f, 1.f), matInv),
		::XMVector3TransformCoord(Vec3(1.f, -1.f, 1.f), matInv),
		::XMVector3TransformCoord(Vec3(-1.f, -1.f, 1.f), matInv),
	};

	// ���� ViewFrustum�� ��鿡 ���� NormalVector�� ����� �����ֱ� ���� ������ ���� CW / CCW�� �����ش�.

	// XMPlaneFromPoints - �� �� �� �� �� ���� ���� 2���� ���͸� �����, �� �� ���͸� �����Ͽ� NormalVector�� ���ϰ�
	// �� NormalVector�� ��� ���� �ٸ� �� ���� ������ �Ÿ��� �����ش�.
	// �׷��� �̰� Vec4(NormalVector{x,y,z}, d)�� �Ѱ��ش�.

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
