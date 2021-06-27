#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Engine.h"
#include "ConstantBuffer.h"
#include "Light.h"

void Scene::Awake()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)			// const로 놓은 이유는, shared_ptr의 경우 
																								// const를 안하면, refCount를 증가시켜버린다. 
																								// 성능 저하 방지
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
	PushLightData();					// 한 프레임에 딱 한번만 한다.

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

	for (auto& gameObject : _gameObjects)		// 모든 게임오브젝트에 대해
	{
		if (gameObject->GetLight() == nullptr)	// Light가 없으면 스킵
			continue;

		const LightInfo& lightInfo = gameObject->GetLight()->GetLightInfo();	// 있으면 Lightinfo를 꺼내서

		lightParams.lights[lightParams.lightCount] = lightInfo;					// 복사 후
		lightParams.lightCount++;														// 카운트 증가
	}
	// 끝나면 Global ConstantBuffer에 넣어 GPU에 밀어넣음.
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
