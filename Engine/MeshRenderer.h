#pragma once
#include "Component.h"

class Mesh;
class Material;

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void						SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void						SetMaterial(shared_ptr<Material> material) { _material = material; }

	void						Render();
	shared_ptr<Material> GetMaterial() { return _material; }
private:
	shared_ptr<Mesh> _mesh;
	shared_ptr<Material> _material;
};

