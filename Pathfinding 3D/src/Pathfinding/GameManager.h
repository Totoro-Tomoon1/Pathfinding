#pragma once
#include "pch.h"
#include <chrono>

class Entity;
class Scene;
class Debug;

class Tile;
class Pathfinding;

class GameManager
{
	std::list<Entity*> mEntities;
	std::list<Entity*> mEntitiesToDestroy;
	std::list<Entity*> mEntitiesToAdd;

	Window* mpWindow = nullptr;
	Camera* m_pCamera = nullptr;
	gce::Font mFont;

	Scene* mpScene = nullptr;
	Pathfinding* mpPathfinding = nullptr;

	int mWindowWidth;
	int mWindowHeight;

	std::chrono::steady_clock::time_point mLastTime;
	float mDeltaTime;

	gce::Color mClearColor;

	Sphere* m_pCursor = nullptr;

private:
	GameManager();

	void Init();

	void Run();

	void HandleInput();
	void Update();
	void Draw();

	Window* GetWindow() const { return mpWindow; }

	gce::Vector2i32 GetMousPosition();
	gce::Vector3f32 GetWorldMousePosition();

public:
	std::list<Tile*> mTiles;

	~GameManager();
	static GameManager* Get();

	template<typename T>
	void LaunchScene();

	float GetDeltaTime() const { return mDeltaTime; }
	Camera* GetCamera() const { return m_pCamera; }
	Scene* GetScene() const { return mpScene; }
	gce::Vector2i32 GetWindowSize() { return { mWindowHeight, mWindowWidth }; }
	gce::Vector3f32 GetCurrentMousePos();
	void SetCamera(Camera* camera) { m_pCamera = camera; }
	void SetWindow(Window* window) { mpWindow = window; }

	friend Debug;
	friend Scene;
};

#include "GameManager.inl"