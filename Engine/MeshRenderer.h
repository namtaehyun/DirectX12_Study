#pragma once
#include "Component.h"

class Mesh;
class Material;

// [32][32] -- Mesh[32], Material[32]
union InstanceID
{
	struct
	{
		uint32 meshID;
		uint32 materialID;
	};
	uint64 id;	// 이 ID라는게 uint32로 되어있는 mesh, material ID를 모두 가지고 있는 데이터이다.
};

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void						SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void						SetMaterial(shared_ptr<Material> material) { _material = material; }

	void						Render();
	void						Render(shared_ptr<class InstancingBuffer>& buffer);

	shared_ptr<Material> GetMaterial() { return _material; }
	uint64					GetInstanceID();

private:
	shared_ptr<Mesh> _mesh;
	shared_ptr<Material> _material;
};

