#pragma once

#include "Scene.h"
#include "Node.h"

class Tile;
class Agent;

class Pathfinding : public Scene
{
private:
	std::vector<std::vector<Tile*>> m_vTiles;
	std::vector<std::vector<Node<Tile>>> m_vNodes;

	std::vector<Agent*> m_vAgents;

	int mCurrentHeight;
	int mCurrentWidth;

	Agent* mCurrentSelected = nullptr;

	bool mousePress = false;

	void SetNeighbors(Node<Tile>* node);
	void CreateAgent(sf::Vector2i mousePos);
	void SelectAgent(sf::Vector2i mousePos);
	void MoveSelectedAgent(std::vector<std::vector<Node<Tile>>>& vNodes, sf::Vector2i mousePos, bool move = true);
	void AddPointToCurrentAgent(sf::Vector2i mousePos);
	Tile* GetTileWithPos(sf::Vector2i pos);
	void RemoveAgent(Agent* agent);
	void ChangePatrouille();
	void ChangeMap(sf::Vector2i mousePos);
	void DeleteMap();
	void SwitchMap(int mapNumber);
	void UpdateNode(Node<Tile>* node, bool becomeObstacle);

public:
	bool mapUpdate = false;

	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;
	void OnEndUpdate() override;

	void ResetNodes();
	void DeselectAgent();

	sf::Vector2i GetPosInTab(sf::Vector2f pos);
	Node<Tile>* GetNodeWithPos(sf::Vector2f pos);
	Node<Tile>* GetNodeWithTabPos(sf::Vector2i pos);
};

