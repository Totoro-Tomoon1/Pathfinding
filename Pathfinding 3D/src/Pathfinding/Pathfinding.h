#pragma once

#include "Scene.h"
#include "Node.h"

class Tile;
class Agent;

class Pathfinding : public Scene
{
private:
	std::vector<std::vector<std::vector<Tile*>>> m_vTiles;
	std::vector<std::vector<std::vector<Node<Tile>>>> m_vNodes;

	std::vector<Tile*> mTileToDelete;

	std::vector<Agent*> m_vAgents;

	int mCurrentHeight;
	int mCurrentWidth;
	int mCurrentDepth;

	int mCameraType = 0;

	Agent* mCurrentSelected = nullptr;

	void SetNeighbors(Node<Tile>* node);
	void CreateAgent(gce::Vector3f32 mousePos);
	void SelectAgent(gce::Vector3f32 mousePos);
	void MoveSelectedAgent(gce::Vector3f32 mousePos, bool move = true);
	void AddPointToCurrentAgent(gce::Vector3f32 mousePos);
	Tile* GetTileWithPos(gce::Vector3f32 pos);
	void RemoveAgent(Agent* agent);
	void ChangePatrouille();
	void ChangeMap(gce::Vector3f32 mousePos, bool create);
	void DeleteMap();
	void SwitchMap(int mapNumber);
	void ChangeCamera();
	void UpdateNode(Node<Tile>* node, bool becomeObstacle);
	Tile* CreateTile();

public:
	bool mapUpdate = false;

	void OnInitialize() override;
	void OnEvent() override;
	void OnUpdate() override;
	void OnEndUpdate() override;

	void ResetNodes();

	gce::Vector3f32 GetPosInTab(gce::Vector3f32 pos);
	Node<Tile>* GetNodeWithPos(gce::Vector3f32 pos);
	Node<Tile>* GetNodeWithTabPos(gce::Vector3f32 pos);
	void DeselectAgent();

	gce::Vector3f32 GetMapSize() { return gce::Vector3f32(mCurrentWidth, mCurrentHeight, mCurrentDepth); }

	float GetHeightMousePos(gce::Vector3f32 pos) override;
};

