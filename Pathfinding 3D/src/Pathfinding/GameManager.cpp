#include "pch.h"
#include "GameManager.h"

#include "Tile.h"
#include "Pathfinding.h"

GameManager::GameManager()
{
	mpWindow = nullptr;
	mDeltaTime = 0.0f;
	mpScene = nullptr;
	mWindowWidth = -1;
	mWindowHeight = -1;

	mLastTime = std::chrono::high_resolution_clock::now();
}

GameManager* GameManager::Get()
{
	static GameManager mInstance;

	return &mInstance;
}

GameManager::~GameManager()
{
	delete mpWindow;
	delete mpScene;

	for (Entity* entity : mEntities)
	{
		delete entity;
	}
}

void GameManager::Init()
{
	mpScene->OnInitialize();
}

void GameManager::Run()
{
	if (mpWindow == nullptr)
	{
		std::cout << "Window not created, creating default window" << std::endl;
	}
	
	_ASSERT(mpScene != nullptr);

	while (mpWindow->IsOpen())
	{
		// Deltatime
		std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

		std::chrono::duration<float> delta = currentTime - mLastTime;
		mDeltaTime = delta.count();

		mLastTime = currentTime;


		HandleInput();

		Update();

		Draw();
	}
}

void GameManager::HandleInput()
{
	mpScene->OnEvent();
}

void GameManager::Update()
{
	if (m_pCursor == nullptr)
	{
		m_pCursor = new Sphere;
		m_pCursor->SetScale({ 0.6f, 0.6f, 0.6f });
		m_pCursor->SetColor({1, 1, 0});
	}

	float x = std::round(GetWorldMousePosition().x);
	float y = std::round(GetWorldMousePosition().y);
	float z = mpScene->GetHeightMousePos({ x, y, 0 }) + 0.5f;

	gce::Vector3f32 lim = mpPathfinding->GetMapSize();

	if (x < 0)
		x = 0;
	else if (x >= lim.x)
		x = lim.x - 1;

	if (y < 0)
		y = 0;
	else if (y >= lim.y)
		y = lim.y - 1;

	gce::Vector3f32 pos = {x, y, z};

	m_pCursor->SetPosition(pos);

	mpScene->OnUpdate();

	//Update
	for (auto it = mEntities.begin(); it != mEntities.end(); )
	{
		Entity* entity = *it;


		if (entity->ToDestroy() == false)
		{
			entity->Update();
			++it;
			continue;
		}

		mEntitiesToDestroy.push_back(entity);
		it = mEntities.erase(it);
	}

	for (auto it = mTiles.begin(); it != mTiles.end();)
	{
		Tile* tile = *it;

		tile->OnUpdate();

		++it;
	}

	for (auto it = mEntitiesToDestroy.begin(); it != mEntitiesToDestroy.end(); ++it)
	{
		delete* it;
	}

	mEntitiesToDestroy.clear();

	for (auto it = mEntitiesToAdd.begin(); it != mEntitiesToAdd.end(); ++it)
	{
		mEntities.push_back(*it);
	}

	mEntitiesToAdd.clear();

	mpScene->OnEndUpdate();
}

void GameManager::Draw()
{
	mpWindow->Begin(*m_pCamera);

	for (Entity* entity : mEntities)
	{
		if (entity->IsToDraw())
			mpWindow->Draw(*entity->GetShape());
	}

	for (Tile* tile : mTiles)
	{
		if (tile->IsToDraw())
			mpWindow->Draw(*tile->GetShape());
	}

	mpWindow->Draw(*m_pCursor);

	mpWindow->End();
	mpWindow->Display();
}

gce::Vector2i32 GameManager::GetMousPosition()
{
	return GetMousePosition();
}

gce::Vector3f32 GameManager::GetWorldMousePosition()
{
	gce::Vector2i32 screenPos = GetMousPosition();

	// Position de la cam�ra
	auto camPos = m_pCamera->GetPosition();   // Vector2f
	//float zoom = camera->GetZoom();        // float

	auto screenSize = GetWindowSize();

	float worldX = (screenPos.x - screenSize.x * 0.5f) / (camPos.z * 2);
	float worldY = (screenPos.y - screenSize.y * 0.5f) / (camPos.z * 2);

	return { worldX - 4, worldY - 2.5f, 0 };
}

gce::Vector3f32 GameManager::GetCurrentMousePos()
{
	return gce::Vector3f32(m_pCursor->GetPosition().x, m_pCursor->GetPosition().y, m_pCursor->GetPosition().z);
}
