#pragma once
#include "InstancingBuffer.h"

class GameObject;

class InstancingManager
{
	DECLARE_SINGLE(InstancingManager);

public:
	void Render(vector<shared_ptr<GameObject>>& gameObjects);	// Render호출하면, Camera에서 분류했던 오브젝트를 다 떠넘겨줄것.

	void ClearBuffer();
	void Clear() { _buffers.clear(); }

private:
	void AddParam(uint64 instanceId, InstancingParams& data);	// 동일 Material, Mesh를 사용하는 애들을 구분하는 ID

private:
	map<uint64, shared_ptr<InstancingBuffer>> _buffers;
};