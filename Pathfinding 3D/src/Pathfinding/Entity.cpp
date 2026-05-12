#include "pch.h"
#include "Entity.h"

#include "GameManager.h"
#include "UtilsPathfinding.h"
//#include "Debug.h"

void Entity::Initialize(float radius, const gce::Color color)
{
	mDirection = gce::Vector3f32(0.0f, 0.0f, 0.0f);

	/*mShape.setOrigin(0.f, 0.f);
	mShape.setRadius(radius);
	mShape.setFillColor(color);*/

	mShape = new Sphere;
	mShape->SetPosition(gce::Vector3f32(0.0f, 0.0f, 0.0f));
	mShape->SetScale({ radius, radius, radius });
	SetColor(color);
	
	mTarget.isSet = false;

	OnInitialize();
}

void Entity::Repulse(Entity* other) 
{
	gce::Vector3f32 distance = GetPosition() - other->GetPosition();
	
	float sqrLength = (distance.x * distance.x) + (distance.y * distance.y);
	float length = std::sqrt(sqrLength);

	float radius1 = mShape->GetScale().x;
	float radius2 = other->mShape->GetScale().x;

	float overlap = (length - (radius1 + radius2)) * 0.5f;

	gce::Vector3f32 normal = distance / length;

	gce::Vector3f32 translation = {overlap * normal.x, overlap * normal.y, overlap * normal.z};

	gce::Vector3f32 position1 = GetPosition() - translation;
	gce::Vector3f32 position2 = other->GetPosition() + translation;

	SetPosition(position1.x, position1.y, position1.z);
	other->SetPosition(position2.x, position2.y, position2.z);
}

bool Entity::IsColliding(Entity* other) const
{
	gce::Vector3f32 distance = GetPosition() - other->GetPosition();

	float sqrLength = (distance.x * distance.x) + (distance.y * distance.y);

	float radius1 = mShape->GetScale().x;
	float radius2 = other->mShape->GetScale().x;

	float sqrRadius = (radius1 + radius2) * (radius1 + radius2);

	return sqrLength < sqrRadius;
}

bool Entity::IsInside(float x, float y, float z) const
{
	gce::Vector3f32 position = GetPosition();

	float dx = x - position.x;
	float dy = y - position.y;
	float dz = z - position.z;

	float radius = mShape->GetScale().x;

	return (dx * dx + dy * dy + dz * dz) < (radius * radius);
}

void Entity::Destroy()
{
	mToDestroy = true;

	OnDestroy();
}

//void Entity::SetPosition(float x, float y)
//{
//	float size = mShape->GetScale().x * 2;
//
//	x -= size * ratioX;
//	y -= size * ratioY;
//
//	//mShape.setPosition(x, y);
//	mShape->SetPosition({x, y, 0});
//
//	//#TODO Optimise
//	if (mTarget.isSet) 
//	{
//		gce::Vector3f32 position = GetPosition(0.5f, 0.5f);
//		//mTarget.distance = Utils::GetDistance(position.x, position.y, mTarget.position.x, mTarget.position.y);
//		GoToDirection(mTarget.position.x, mTarget.position.y);
//		mTarget.isSet = true;
//	}
//}

void Entity::SetPosition(float x, float y, float z)
{
	mShape->SetPosition({x, y, z});
}

void Entity::SetColor(gce::Color color)
{
	mShape->SetColor({ (float32)(color.r / 255), (float32)(color.g / 255), (float32)(color.b / 255) });
}

gce::Vector3f32 Entity::GetPosition() const
{
	return mShape->GetPosition();
}

//gce::Vector3f32 Entity::GetPosition() const
//{
//	float size = mShape->GetScale().x * 2;
//	gce::Vector3f32 position = mShape->GetPosition();
//
//	position.x += size * ratioX;
//	position.y += size * ratioY;
//	position.z += size * ratioZ;
//
//	return position;
//}

bool Entity::GoToDirection(int x, int y, float z, float speed)
{
	gce::Vector3f32 position = GetPosition();
	gce::Vector3f32 direction = gce::Vector3f32(x - position.x, y - position.y, z - position.z);
	
	bool success = UtilsPathfinding::Normalize(direction);
	if (success == false)
		return false;

	SetDirection(direction.x, direction.y, direction.z, speed);

	return true;
}

bool Entity::GoToPosition(int x, int y, float z, float speed)
{
	if (GoToDirection(x, y, z, speed) == false)
		return false;

	gce::Vector3f32 position = GetPosition();

	mTarget.position = gce::Vector3f32(x, y, z + 0.5f);
	mTarget.distance = UtilsPathfinding::GetDistance(position.x, position.y, position.z, x, y, z + 0.5f);
	mTarget.isSet = true;

	return true;
}

void Entity::SetDirection(float x, float y, float z, float speed)
{
	if (speed > 0)
		mSpeed = speed;

	mDirection = gce::Vector3f32(x, y, z);
	mTarget.isSet = false;
}

float Entity::GetRadius() const
{
	return mShape->GetScale().x;
}

void Entity::Update()
{
	float dt = GetDeltaTime();
	float distance = dt * mSpeed;
	gce::Vector3f32 translation = {distance * mDirection.x, distance * mDirection.y, distance * mDirection.z};
	//mShape.move(translation);
	mShape->SetPosition(mShape->GetPosition() + gce::Vector3f32(translation.x, translation.y, translation.z));

	if (mTarget.isSet) 
	{
		float x1 = GetPosition().x;
		float y1 = GetPosition().y;
		float z1 = GetPosition().z;

		float x2 = x1 + mDirection.x * mTarget.distance;
		float y2 = y1 + mDirection.y * mTarget.distance;
		float z2 = z1 + mDirection.z * mTarget.distance;

		//Debug::DrawLine(x1, y1, x2, y2, sf::Color::Red);

		//Debug::DrawCircle(mTarget.position.x, mTarget.position.y, 5.f, sf::Color::Yellow);

		mTarget.distance -= distance;

		if (mTarget.distance <= 0.f)
		{
			SetPosition(mTarget.position.x, mTarget.position.y, mTarget.position.z - 0.5f);
			mDirection = gce::Vector3f32(0.f, 0.f, 0.0f);
			std::cout << "reset dir" << std::endl;
			mTarget.isSet = false;
		}
	}

	OnUpdate();
}

Scene* Entity::GetScene() const
{
	return GameManager::Get()->GetScene();
}

float Entity::GetDeltaTime() const
{
	return GameManager::Get()->GetDeltaTime();
}