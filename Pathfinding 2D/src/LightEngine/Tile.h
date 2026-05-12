#pragma once

#include "Entity.h"
#include "WPosMPos.h"

class Agent;

class Tile : public Entity
{
private:
	bool mIsTempLock = false;

public:
	WPosMPos mPos;

	bool mIsObstacle = false;
	bool mIsLock = false;
	

	float mTimer = 0.5f;
	float mTimerProgress = 0.5f;

	Agent* mAgent = nullptr;

	void OnUpdate() override;
	void SetTempLock(bool isTempLock, Agent* newAgent);
	bool IsTempLock() { return mIsTempLock; }
};

