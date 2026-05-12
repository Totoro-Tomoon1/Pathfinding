#include "pch.h"
#include "Tile.h"

#include <iostream>

void Tile::OnInitialize()
{
	mShape = new Cube;
	mShape->SetScale({1.f, 1.f, 1.f});
	SetColor(gce::Color::White);
}

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
		SetColor(gce::Color::Magenta);
		mTimerProgress = mTimer;
	}
	else
	{
		SetColor(gce::Color::White);
		//std::cout << "yo" << std::endl;
	}
}

void Tile::SetColor(gce::Color color)
{
	mShape->SetColor({ (float32)(color.r / 255), (float32)(color.g / 255), (float32)(color.b / 255) });
}

gce::Vector3f32 Tile::GetPosition() const
{
	return mShape->GetPosition();
}

void Tile::SetPosition(float x, float y, float z)
{
	mShape->SetPosition({ x, y, z });
}