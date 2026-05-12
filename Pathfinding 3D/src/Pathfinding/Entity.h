#pragma once
#include "Geometry.h"

class Scene;
class Sphere;

class Entity
{
    struct Target 
    {
		gce::Vector3f32 position;
        float distance;
		bool isSet;
    };

protected:
	//sf::RectangleShape mShape;
    //sf::CircleShape mShape;
	Geometry* mShape = nullptr;
	gce::Vector3f32 mDirection;
	Target mTarget;
    float mSpeed = 0.f;
    bool mToDestroy = false;
    int mTag = -1;
	bool mRigidBody = false;
	bool mToDraw = true;

public:
	bool GoToDirection(int x, int y, float z, float speed = -1.f);
    bool GoToPosition(int x, int y, float z, float speed = -1.f);
    //void SetPosition(float x, float y, float ratioX = 0.5f, float ratioY = 0.5f);
	void SetPosition(float x, float y, float z);
	void SetDirection(float x, float y, float z, float speed = -1.f);
	void SetSpeed(float speed) { mSpeed = speed; }
	void SetTag(int tag) { mTag = tag; }
	float GetRadius() const;
	void SetRigidBody(bool isRigitBody) { mRigidBody = isRigitBody; }
	bool IsRigidBody() const { return mRigidBody; }
	void SetColor(gce::Color color);

    //gce::Vector3f32 GetPosition(float ratioX = 0.5f, float ratioY = 0.5f, float ratioZ = 0.5f) const;
	gce::Vector3f32 GetPosition() const;
	Geometry* GetShape() { return mShape; }

	bool IsTag(int tag) const { return mTag == tag; }
    bool IsColliding(Entity* other) const;
	bool IsInside(float x, float y, float z) const;
	bool IsToDraw() { return mToDraw; }
	void SetToDraw(bool toDraw) { mToDraw = toDraw; }

    void Destroy();
	bool ToDestroy() const { return mToDestroy; }
	
	template<typename T>
	T* GetScene() const;

    Scene* GetScene() const;
	float GetDeltaTime() const;

    template<typename T>
    T* CreateEntity(float radius, const gce::Vector3f32 color);

protected:
    Entity() = default;
    ~Entity() = default;

    virtual void OnUpdate() {};
    virtual void OnCollision(Entity* collidedWith) {};
	virtual void OnInitialize() {};
	virtual void OnDestroy() {};
	
private:
    void Update();
	void Initialize(float radius, const gce::Color color);
	void Repulse(Entity* other);

    friend class GameManager;
    friend Scene;
};

#include "Entity.inl"