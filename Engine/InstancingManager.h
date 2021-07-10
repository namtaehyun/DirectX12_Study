#pragma once
#include "InstancingBuffer.h"

class GameObject;

class InstancingManager
{
	DECLARE_SINGLE(InstancingManager);

public:
	void Render(vector<shared_ptr<GameObject>>& gameObjects);	// Renderȣ���ϸ�, Camera���� �з��ߴ� ������Ʈ�� �� ���Ѱ��ٰ�.

	void ClearBuffer();
	void Clear() { _buffers.clear(); }

private:
	void AddParam(uint64 instanceId, InstancingParams& data);	// ���� Material, Mesh�� ����ϴ� �ֵ��� �����ϴ� ID

private:
	map<uint64, shared_ptr<InstancingBuffer>> _buffers;
};