#include "Pathfinding.h"

#include "GridConfiguration.h"
#include "Tile.h"
#include "Agent.h"

#include <iostream>

int TYPE = 3;

void Pathfinding::SetNeighbors(Node<Tile>* node)
{
    sf::Vector2i sizeWindow = GameManager::Get()->GetWindowSize();

    int x = node->data->mPos.col;
    int y = node->data->mPos.row;

    std::vector<Node<Tile>*> verifie;
    std::vector<Node<Tile>*> diagoVerifie;

    if (y + 1 < mCurrentHeight)
        verifie.push_back(&m_vNodes[y + 1][x]);
    if (y - 1 >= 0)
        verifie.push_back(&m_vNodes[y - 1][x]);
    if (x + 1 < mCurrentWidth)
        verifie.push_back(&m_vNodes[y][x + 1]);
    if (x - 1 >= 0)
        verifie.push_back(&m_vNodes[y][x - 1]);

    // DIAGONAAAAAAAAAAAAAAAAAAALE
    if (y - 1 >= 0 && x - 1 >= 0)
        diagoVerifie.push_back(&m_vNodes[y - 1][x - 1]);
    else
        diagoVerifie.push_back(nullptr);
    if (y - 1 >= 0 && x + 1 < mCurrentWidth)
        diagoVerifie.push_back(&m_vNodes[y - 1][x + 1]);
    else
        diagoVerifie.push_back(nullptr);
    if (y + 1 < mCurrentHeight && x + 1 < mCurrentWidth)
        diagoVerifie.push_back(&m_vNodes[y + 1][x + 1]);
    else
        diagoVerifie.push_back(nullptr);
    if (y + 1 < mCurrentHeight && x - 1 >= 0)
        diagoVerifie.push_back(&m_vNodes[y + 1][x - 1]);
    else
        diagoVerifie.push_back(nullptr);


    for (Node<Tile>* neighbor : verifie)
    {
        Tile* tile = neighbor->data;

        if (tile->mIsObstacle)
        {
            continue;
        }

        node->neighbors.push_back(neighbor);
    }

    for (Node<Tile>* diagoNeighbor : diagoVerifie)
    {

        node->diagoNeighbors.push_back(diagoNeighbor);
    }
}

void Pathfinding::CreateAgent(sf::Vector2i mousePos)
{
    for (std::vector<Node<Tile>>& ligne : m_vNodes)
    {
        for (Node<Tile>& node : ligne)
        {
            Tile* tile = node.data;
            if (tile->GetShape()->getGlobalBounds().contains({(float)mousePos.x, (float)mousePos.y}) && tile->mIsObstacle == false)
            {
                //si dans une shape verifier si il y en a deja un pour le detruire
                for (Agent* agent : m_vAgents)
                {
                    if (agent->GetPosition() == tile->GetPosition())
                    {
                        if (agent == mCurrentSelected)
                            mCurrentSelected = nullptr;

                        RemoveAgent(agent);
                        agent->Destroy();
                        return;
                    }
                }

                Agent* newAgent = CreateEntity<Agent>(15, sf::Color::Green);
                newAgent->SetPosition(tile->GetPosition().x, tile->GetPosition().y);
                newAgent->SetScene(this);
                newAgent->Init();
                m_vAgents.push_back(newAgent);
                return;
            }
        }
    }
}

void Pathfinding::SelectAgent(sf::Vector2i mousePos)
{
    for (Agent* agent : m_vAgents)
    {
        if (agent->GetShape()->getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y }))
        {
            if (mCurrentSelected != agent)
            {
                if (mCurrentSelected != nullptr)
                    mCurrentSelected->SetColor(sf::Color::Green);

                mCurrentSelected = agent;
                agent->SetColor(sf::Color::Red);
                return;
            }
        }
    }

    MoveSelectedAgent(m_vNodes, GameManager::Get()->GetMousePosition());
}

void Pathfinding::DeselectAgent()
{
    if (mCurrentSelected != nullptr)
    {
        mCurrentSelected->SetColor(sf::Color::Green);
        mCurrentSelected = nullptr;
    }
}

void Pathfinding::MoveSelectedAgent(std::vector<std::vector<Node<Tile>>>& vNodes, sf::Vector2i mousePos, bool move)
{
    if (mCurrentSelected == nullptr)
        return;

    Tile* tile = GetTileWithPos(mousePos);

    if (tile != nullptr)
    {
        ResetNodes();
        mCurrentSelected->Move(tile->mPos, move);
    }
}

void Pathfinding::AddPointToCurrentAgent(sf::Vector2i mousePos)
{
    if (mCurrentSelected == nullptr)
        return;

    Tile* tile = GetTileWithPos(mousePos);

    if (tile != nullptr)
    {
        ResetNodes();
        mCurrentSelected->AddPointToPath(tile->mPos);
    }
}

Tile* Pathfinding::GetTileWithPos(sf::Vector2i pos)
{
    for (std::vector<Node<Tile>>& ligne : m_vNodes)
    {
        for (Node<Tile>& node : ligne)
        {
            Tile* tile = node.data;
            if (tile->GetShape()->getGlobalBounds().contains({ (float)pos.x, (float)pos.y }))
            {
                return tile;
            }
        }
    }

    return nullptr;
}

sf::Vector2i Pathfinding::GetPosInTab(sf::Vector2f pos)
{
    Tile* tile = GetTileWithPos((sf::Vector2i)pos);

    if (tile != nullptr)
        return sf::Vector2i(tile->mPos.col, tile->mPos.row);

    return { 0, 0 };
}

Node<Tile>* Pathfinding::GetNodeWithPos(sf::Vector2f pos)
{
    for (std::vector<Node<Tile>>& ligne : m_vNodes)
    {
        for (Node<Tile>& node : ligne)
        {
            Tile* tile = node.data;

            if (tile->GetShape()->getGlobalBounds().contains(pos))
                return &node;
        }
    }

    return nullptr;
}

Node<Tile>* Pathfinding::GetNodeWithTabPos(sf::Vector2i pos)
{
    return &m_vNodes[pos.y][pos.x];
}

void Pathfinding::ResetNodes()
{
    for (std::vector<Node<Tile>>& ligne : m_vNodes)
    {
        for (Node<Tile>& node : ligne)
        {
            node.isVisited = false;
            node.comeFrom = nullptr;
            node.totalCost = 0;
            node.costFromStart = 0;
        }
    }
}

void Pathfinding::RemoveAgent(Agent* agentToRemove)
{
    int i = 0;
    for (Agent* agent : m_vAgents)
    {
        if (agentToRemove == agent)
        {
            m_vAgents.erase(m_vAgents.begin() + i);
            return;
        }
        i++;
    }
}

void Pathfinding::ChangePatrouille()
{
    if (mCurrentSelected == nullptr)
        return;

    Tile* tile = GetTileWithPos((sf::Vector2i)mCurrentSelected->GetPosition());

    if (tile == nullptr)
        return;
    
    mCurrentSelected->ChangePatrouille(tile->mPos);
}

void Pathfinding::ChangeMap(sf::Vector2i mousePos)
{
    if (mCurrentSelected != nullptr)
    {
        mCurrentSelected->SetColor(sf::Color::Green);
    }

    mCurrentSelected = nullptr;

    Node<Tile>* node = GetNodeWithPos((sf::Vector2f)mousePos);
    Tile* tile = GetTileWithPos(mousePos);

    if (tile == nullptr)
        return;

    if (tile->mIsLock)
        return;        

    if (tile->mIsObstacle)
    {
        tile->mIsObstacle = false;
        tile->GetShape()->setFillColor(sf::Color::White);
    }
    else
    {
        tile->mIsObstacle = true;
        tile->GetShape()->setFillColor(sf::Color::Black);
    }

    mapUpdate = true;
}

void Pathfinding::DeleteMap()
{
    for (Agent* agent : m_vAgents)
    {
        agent->Destroy();
    }
    m_vAgents.clear();
    for (std::vector<Tile*> vTile : m_vTiles)
    {
        for (Tile* tile : vTile)
        {
            tile->Destroy();
        }
    }
    m_vTiles.clear();
    for (std::vector<Node<Tile>> vTile : m_vNodes)
    {
        for (Node<Tile> tile : vTile)
        {
            tile.data->Destroy();
        }
    }
    m_vNodes.clear();

    mCurrentSelected = nullptr;
}

void Pathfinding::SwitchMap(int mapNumber)
{
    TYPE = mapNumber;

    DeleteMap();

    OnInitialize();
}

void Pathfinding::UpdateNode(Node<Tile>* node, bool becomeObstacle)
{
    Tile* tile = node->data;

    if (becomeObstacle)
    {
        tile->mIsObstacle = true;

        for (Node<Tile>* neighbor : node->neighbors)
        {
            for (int i = 0; i + 1 < neighbor->neighbors.size(); i++)
            {
                if (neighbor->neighbors[i] == node)
                {
                    neighbor->neighbors.erase(neighbor->neighbors.begin() + i);
                    continue;
                }
            }
        }
    }
    else
    {
        tile->mIsObstacle = false;
        node->neighbors.clear();

        SetNeighbors(node);

        for (Node<Tile>* neighbor : node->neighbors)
        {
            neighbor->neighbors.push_back(node);
        }
    }
}

void Pathfinding::OnInitialize()
{
    std::vector<std::vector<char>> vMaps;

    switch (TYPE)
    {
    case 1:
        vMaps = GridConfiguration::Type1();
        mCurrentHeight = 10;
        mCurrentWidth = 10;
        break;
    case 2:
        vMaps = GridConfiguration::Type2();
        mCurrentHeight = 18;
        mCurrentWidth = 20;
        break;
    case 3:
        vMaps = GridConfiguration::Type3();
        mCurrentHeight = 18;
        mCurrentWidth = 32;
        break;
    case 4:
        vMaps = GridConfiguration::Type4();
        mCurrentHeight = 5;
        mCurrentWidth = 5;
        break;
    default:
        vMaps = GridConfiguration::Type1();
        mCurrentHeight = 10;
        mCurrentWidth = 10;
        break;
    };

    int size = 20;

    for (int i = 0; i < mCurrentHeight; i++)
    {
        std::vector<Tile*> temp;

        for (int j = 0; j < mCurrentWidth; j++)
        {
            Tile* newTile = CreateEntity<Tile>(size, sf::Color::White);
            newTile->SetPosition(j * size * 2 + size, i * size * 2 + size);
            WPosMPos pos = {i, j, j * size * 2 + size , i * size * 2 + size };
            newTile->mPos = pos;

            if (vMaps[i][j] == '#')
            {
                newTile->mIsObstacle = true;
                newTile->SetColor(sf::Color::Black);
            }

            temp.push_back(newTile);
        }

        m_vTiles.push_back(temp);
    }

    // Initialisation des nodes
    for (std::vector<Tile*> ligne : m_vTiles)
    {
        std::vector<Node<Tile>> temp;
        for (Tile* tile : ligne)
        {
            Node<Tile> node;
            node.data = tile;
            temp.push_back(node);
        }
        m_vNodes.push_back(temp);
    }

    // Initialisation des voisins
    for (int y = 0; y < mCurrentHeight; y++)
    {
        for (int x = 0; x < mCurrentWidth; x++)
        {
            Node<Tile>* currentNode = &m_vNodes[y][x];

            SetNeighbors(currentNode);
        }
    }
}

void Pathfinding::OnEvent(const sf::Event& event)
{
    if (event.type == sf::Event::EventType::KeyPressed)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && event.key.code == sf::Keyboard::S && mousePress == false)
        {
            ChangeMap(GameManager::Get()->GetMousePosition());
            //std::cout << "changemap" << std::endl;
            mousePress = true;
        }
        else if (event.key.code == sf::Keyboard::P)
        {
            ChangePatrouille();
        }
        else if (event.key.code == sf::Keyboard::T)
        {
            DeselectAgent();
        }
        else if (event.key.code == sf::Keyboard::F1)
        {
            SwitchMap(1);
        }
        else if (event.key.code == sf::Keyboard::F2)
        {
            SwitchMap(2);
        }
        else if (event.key.code == sf::Keyboard::F3)
        {
            SwitchMap(3);
        }
        else if (event.key.code == sf::Keyboard::F4)
        {
            SwitchMap(4);
        }
    }
    else if (event.type == sf::Event::EventType::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Button::Left && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            CreateAgent(GameManager::Get()->GetMousePosition());
        }
        else if (event.mouseButton.button == sf::Mouse::Button::Left && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        {
            AddPointToCurrentAgent(GameManager::Get()->GetMousePosition());
            //std::cout << "addpoint" << std::endl;
        }
        else if (event.mouseButton.button == sf::Mouse::Button::Left && sf::Keyboard::isKeyPressed(sf::Keyboard::S) == false)
        {
            SelectAgent(GameManager::Get()->GetMousePosition());
            //std::cout << "selectagent" << std::endl;
        }
        else if (event.mouseButton.button == sf::Mouse::Button::Right)
        {
            MoveSelectedAgent(m_vNodes, GameManager::Get()->GetMousePosition(), false);
        }
    }
    else if (event.type == sf::Event::EventType::MouseButtonReleased)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            mousePress = false;
        }
    }
}

void Pathfinding::OnUpdate()
{
    for (std::vector<Node<Tile>>& ligne : m_vNodes)
    {
        for (Node<Tile>& node : ligne)
        {
            sf::Color newColor = sf::Color::White;

            if (node.data->mIsObstacle)
                newColor = sf::Color::Black;
            else if (node.data->mIsLock)
                newColor = sf::Color::Blue;
            else if (node.data->IsTempLock())
            {
                newColor = sf::Color::Magenta;
                //debug
                /*for (Agent* agent : m_vAgents)
                {
                    if (node.data->GetShape()->getGlobalBounds().contains(agent->GetPosition()))
                    {
                        newColor = sf::Color::Magenta;
                    }
                }  */              
            }

            node.data->GetShape()->setFillColor(newColor);
        }
    }
}

void Pathfinding::OnEndUpdate()
{
    mapUpdate = false;
}
