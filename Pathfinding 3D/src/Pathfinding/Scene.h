#pragma once

class GameManager;

class Scene
{
private:
	void SetGameManager(GameManager* pGameManager) { mpGameManager = pGameManager; }
	
protected:
	Scene() = default;

	virtual void OnInitialize() = 0;
	virtual void OnEvent() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnEndUpdate() {};

	GameManager* mpGameManager;

public:
	template<typename T>
	T* CreateEntity(float radius, gce::Color color);

	float GetDeltaTime() const;

	int GetWindowWidth() const;
	int GetWindowHeight() const;

	virtual float GetHeightMousePos(gce::Vector3f32 pos) = 0;

	friend GameManager;
};

#include "Scene.inl"