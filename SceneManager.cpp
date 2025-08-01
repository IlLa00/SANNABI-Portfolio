﻿#include "pch.h"
#include "SceneManager.h"
#include "GameScene.h"

void SceneManager::Init()
{
	GameScene* gameScene = new GameScene;
	Scenes.push_back(gameScene);

	currentScene = gameScene;

	currentScene->Init();
}

void SceneManager::Update(float deltaTime)
{
	if (!currentScene) return;

	currentScene->Update(deltaTime);
}

void SceneManager::Render(HDC _hdcBack)
{
	if (!currentScene) return;

	currentScene->Render(_hdcBack);
}

void SceneManager::Destroy()
{
	for (auto& scene : Scenes)
		scene->Destroy();

}