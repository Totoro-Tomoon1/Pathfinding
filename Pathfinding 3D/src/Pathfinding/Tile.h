#pragma once
#include "GameManager.h"
#include "WPosMPos.h"

class Agent;

class Tile
{
private:
	bool mIsTempLock = false;
	Geometry* mShape = nullptr;

public:
	WPosMPos mPos;

	bool mIsObstacle = false;
	bool mIsLock = false;
	

	float mTimer = 4.f;
	float mTimerProgress = 4.f;

	Agent* mAgent = nullptr;

	bool mToDraw = true;

	void OnInitialize();
	void OnUpdate();
	void SetTempLock(bool isTempLock, Agent* newAgent);
	bool IsTempLock() { return mIsTempLock; }
	void SetColor(gce::Color color);
	gce::Vector3f32 GetPosition() const;
	Geometry* GetShape() { return mShape; }
	void SetPosition(float x, float y, float z);
	void SetToDraw(bool toDraw) { mToDraw = toDraw; }
	bool IsToDraw() { return mToDraw; }
};

