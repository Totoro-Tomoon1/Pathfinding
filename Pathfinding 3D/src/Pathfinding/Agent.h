#pragma once

#include "Entity.h"
#include "Node.h"
#include "WPosMPos.h"

class Tile;
class Pathfinding;
class WPosMPos;

class Agent : public Entity
{
private:
	std::vector<gce::Vector3f32> m_vPath;
	std::vector<Node<Tile>*> m_vNodesPath;
	bool mIsMoving = false;
	bool mShowPath = false;

	Pathfinding* m_pScene;

	bool mIsPatrouille = false;
	std::vector<WPosMPos> m_vPatrouillePoint;
	int mCurrentPos = 0;
	WPosMPos mNextPos;

	bool mIsWaiting = false;
	float mWaitTiming = 0.5f;
	float mWaitProgress = 0.5f;

	bool mToStop = false;

	float mSpeed = 1.0f;

	gce::Vector3f32 mCurrentTabPos;

	//Node<Tile>* mCurrentNode = nullptr;
	//Node<Tile>* mNextNode = nullptr;

	void AddToPriorityQueue(std::vector<Node<Tile>*>& priority_queue, Node<Tile>* nodeToAdd);
	bool FindPath(WPosMPos newPos, bool addToPath = false);
	void GetPath(WPosMPos newPos, bool addToPath = false);
	void DrawPath();
	//void ClearPath(); temporairement retirer
	void CheckNextNode();

public:
	void Init();
	void OnUpdate() override;
	void OnDestroy() override;

	void Move(WPosMPos newPos, bool move = true);
	void AddPointToPath(WPosMPos newPos);

	void SetScene(Pathfinding* pScene) { m_pScene = pScene; }
	void ChangePatrouille(WPosMPos currentPos);
};

