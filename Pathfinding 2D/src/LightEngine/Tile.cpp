#include "Tile.h"

#include <iostream>
#include "Agent.h"

void Tile::OnUpdate()
{
	if (mIsTempLock == false)
		return;

	mTimerProgress -= GameManager::Get()->GetDeltaTime();

	if (mTimerProgress <= 0)
	{
		SetTempLock(false, nullptr);
		//std::cout << "desactivation auto" << std::endl;
	}
}

void Tile::SetTempLock(bool isTempLock, Agent* newAgent)
{
	mIsTempLock = isTempLock;

	mAgent = newAgent;

	if (isTempLock)
	{
		SetColor(sf::Color::Magenta);
		mTimerProgress = mTimer;
	}
	else
	{
		SetColor(sf::Color::White);
		//std::cout << "yo" << std::endl;
	}
}
