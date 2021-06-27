#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Engine.h"
#include "ConstantBuffer.h"
#include "Light.h"

void Scene::Awake()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)			// const�� ���� ������, shared_ptr�� ��� 
																								// const�� ���ϸ�, refCount�� �������ѹ�����. 
																								// ���� ���� ����
	{
		gameObject->Awake();
	}
}

void Scene::Start()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Start();
	}
}

void Scene::Update()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Update();
	}
}

void Scene::LateUpdate()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->LateUpdate();
	}
}

void Scene::FinalUpdate()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->FinalUpdate();
	}
}

void Scene::Render()
{
	PushLightData();					// �� �����ӿ� �� �ѹ��� �Ѵ�.

	for (auto& gameObject : _gameObjects)
	{
		if (gameObject->GetCamera() == nullptr)
			continue;

		gameObject->GetCamera()->Render();
	}

}

void Scene::PushLightData()
{
	LightParams lightParams = {};

	for (auto& gameObject : _gameObjects)		// ��� ���ӿ�����Ʈ�� ����
	{
		if (gameObject->GetLight() == nullptr)	// Light�� ������ ��ŵ
			continue;

		const LightInfo& lightInfo = gameObject->GetLight()->GetLightInfo();	// ������ Lightinfo�� ������

		lightParams.lights[lightParams.lightCount] = lightInfo;					// ���� ��
		lightParams.lightCount++;														// ī��Ʈ ����
	}
	// ������ Global ConstantBuffer�� �־� GPU�� �о����.
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::GLOBAL)->SetGlobalData(&lightParams, sizeof(lightParams));
}

void Scene::AddGameObject(shared_ptr<GameObject> gameObject)
{
	_gameObjects.emplace_back(gameObject);
}

void Scene::RemoveGameObject(shared_ptr<GameObject> gameObject)
{
	auto findIt = std::find(_gameObjects.begin(), _gameObjects.end(), gameObject);

	if (findIt != _gameObjects.end())
	{
		_gameObjects.erase(findIt);
	}
}
