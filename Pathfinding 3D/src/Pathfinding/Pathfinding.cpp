#include "pch.h"
#include "Pathfinding.h"

#include "GridConfiguration.h"
#include "Tile.h"
#include "Agent.h"

#include "UtilsPathfinding.h"

#include <iostream>

int TYPE = 4;

void Pathfinding::SetNeighbors(Node<Tile>* node)
{
    //sf::Vector2i sizeWindow = GameManager::Get()->GetWindowSize();

    int x = node->data->mPos.col;
    int y = node->data->mPos.row;
    int z = node->data->mPos.height;

    std::vector<Node<Tile>*> verifie;
    std::vector<Node<Tile>*> diagoVerifie;

    std::vector<int> ligneVerifie;

    if (z - 1 >= 0)
        ligneVerifie.push_back(-1);

    ligneVerifie.push_back(0);

    if (z + 1 < mCurrentDepth)
        ligneVerifie.push_back(1);

    for (int offsetZ : ligneVerifie)
    {
        if (y + 1 < mCurrentHeight)
            verifie.push_back(&m_vNodes[z + offsetZ][y + 1][x]);
        if (y - 1 >= 0)
            verifie.push_back(&m_vNodes[z + offsetZ][y - 1][x]);
        if (x + 1 < mCurrentWidth)
            verifie.push_back(&m_vNodes[z + offsetZ][y][x + 1]);
        if (x - 1 >= 0)
            verifie.push_back(&m_vNodes[z + offsetZ][y][x - 1]);
    }

    // DIAGONAAAAAAAAAAAAAAAAAAALE
    if (y - 1 >= 0 && x - 1 >= 0)
        diagoVerifie.push_back(&m_vNodes[z][y - 1][x - 1]);
    else
        diagoVerifie.push_back(nullptr);
    if (y - 1 >= 0 && x + 1 < mCurrentWidth)
        diagoVerifie.push_back(&m_vNodes[z][y - 1][x + 1]);
    else
        diagoVerifie.push_back(nullptr);
    if (y + 1 < mCurrentHeight && x + 1 < mCurrentWidth)
        diagoVerifie.push_back(&m_vNodes[z][y + 1][x + 1]);
    else
        diagoVerifie.push_back(nullptr);
    if (y + 1 < mCurrentHeight && x - 1 >= 0)
        diagoVerifie.push_back(&m_vNodes[z][y + 1][x - 1]);
    else
        diagoVerifie.push_back(nullptr);


    for (Node<Tile>* neighbor : verifie)
    {
        Tile* tile = neighbor->data;

        if (tile->mIsObstacle)
        {
            continue;
        }

        //verifier si il n'y a rien aus dessus
        if (tile->mPos.z + 1 < mCurrentDepth)
        {
            if (m_vNodes[tile->mPos.z + 1][tile->mPos.y][tile->mPos.x].data->mIsObstacle == false)
                continue;
        }

        node->neighbors.push_back(neighbor);
    }

    for (Node<Tile>* diagoNeighbor : diagoVerifie)
    {
        if (diagoNeighbor != nullptr)
        {
            Tile* tile = diagoNeighbor->data;
            //verifier si il n'y a rien aus dessus
            if (tile->mPos.z + 1 < mCurrentDepth)
            {
                if (m_vNodes[tile->mPos.z + 1][tile->mPos.y][tile->mPos.x].data->mIsObstacle == false)
                    diagoNeighbor = nullptr;
            }
        }

        node->diagoNeighbors.push_back(diagoNeighbor);
    }
}

void Pathfinding::CreateAgent(gce::Vector3f32 mousePos)
{
    //for (std::vector<Node<Tile>>& ligne : m_vNodes)
    //{
    //    for (Node<Tile>& node : ligne)
    //    {
    //        Tile* tile = node.data;
    //        if (UtilsPathfinding::IsInside(tile->GetShape(), mousePos) && tile->mIsObstacle == false)//a verifier
    //        {
    //            for (Agent* agent : m_vAgents)
    //            {
    //                if (agent->GetPosition() == tile->GetPosition() + gce::Vector3f32(0, 0, 0.5f))
    //                {
    //                    if (agent == mCurrentSelected)
    //                        mCurrentSelected = nullptr;

    //                    RemoveAgent(agent);
    //                    agent->Destroy();
    //                    return;
    //                }
    //            }

    //            Agent* newAgent = CreateEntity<Agent>(1, gce::Color::Green);
    //            newAgent->SetPosition(mousePos.x, mousePos.y, 0.5f);
    //            newAgent->SetScene(this);
    //            newAgent->Init();
    //            m_vAgents.push_back(newAgent);
    //            return;
    //        }
    //    }
    //}

    //voir a qu'elle hauteur creer l'agent
    for (int i = mCurrentDepth - 1; i >= 0; i--)
    {
        if (m_vNodes[i][mousePos.y][mousePos.x].data->mIsObstacle == false)
        {
            //On creer l'agent ici
            Agent* newAgent = CreateEntity<Agent>(1, gce::Color::Green);
            newAgent->SetPosition(mousePos.x, mousePos.y, m_vNodes[i][mousePos.y][mousePos.x].data->mPos.z + 0.5f);
            newAgent->SetScene(this);
            newAgent->Init();
            m_vAgents.push_back(newAgent);
            return;
        }
    }
}

void Pathfinding::SelectAgent(gce::Vector3f32 mousePos)
{
    for (Agent* agent : m_vAgents)
    {
        if (UtilsPathfinding::IsInside(agent->GetShape(), mousePos))
        {
            if (mCurrentSelected != agent)
            {
                if (mCurrentSelected != nullptr)
                    mCurrentSelected->SetColor(gce::Color::Green);

                mCurrentSelected = agent;
                agent->SetColor(gce::Color::Red);
                return;
            }
        }
    }

    MoveSelectedAgent(mousePos);
}

void Pathfinding::DeselectAgent()
{
    if (mCurrentSelected != nullptr)
    {
        mCurrentSelected->SetColor(gce::Color::Green);
        mCurrentSelected = nullptr;
    }
}

float Pathfinding::GetHeightMousePos(gce::Vector3f32 pos)
{
    for (int i = mCurrentDepth - 1; i >= 0; i--)
    {
        if (pos.y < 0 || pos.y >= mCurrentHeight)
            continue;

        if (pos.x < 0 || pos.x >= mCurrentWidth)
            continue;

        if (m_vNodes[i][pos.y][pos.x].data->mIsObstacle == false)
        {
            return i;
        }
    }

    return 0;
}

void Pathfinding::MoveSelectedAgent(gce::Vector3f32 mousePos, bool move)
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

void Pathfinding::AddPointToCurrentAgent(gce::Vector3f32 mousePos)
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

Tile* Pathfinding::GetTileWithPos(gce::Vector3f32 pos)
{
    for (std::vector<std::vector<Node<Tile>>>& plan : m_vNodes)
    {
        for (std::vector<Node<Tile>>& ligne : plan)
        {
            for (Node<Tile>& node : ligne)
            {
                Tile* tile = node.data;

                // V�rifie si la position 3D est � l'int�rieur de la tile
                if (UtilsPathfinding::IsInside(tile->GetShape(), pos))
                {
                    std::cout << "Detected tile" << std::endl;
                    return tile;
                }
            }
        }
    }

    return nullptr;
}

gce::Vector3f32 Pathfinding::GetPosInTab(gce::Vector3f32 pos)
{
    Tile* tile = GetTileWithPos({ pos.x, pos.y, pos.z });

    if (tile != nullptr)
        return gce::Vector3f32(tile->mPos.col, tile->mPos.row, tile->mPos.height);

    return { 0, 0, 0 };
}

Node<Tile>* Pathfinding::GetNodeWithPos(gce::Vector3f32 pos)
{
    for (std::vector<std::vector<Node<Tile>>>& plan : m_vNodes)
    {
        for (std::vector<Node<Tile>>& ligne : plan)
        {
            for (Node<Tile>& node : ligne)
            {
                Tile* tile = node.data;

                if (UtilsPathfinding::IsInside(tile->GetShape(), { pos.x, pos.y, pos.z }))
                    return &node;
            }
        }
    }

    return nullptr;
}

Node<Tile>* Pathfinding::GetNodeWithTabPos(gce::Vector3f32 pos)
{
    return &m_vNodes[pos.z][pos.y][pos.x];
    return nullptr;
}

void Pathfinding::ResetNodes()
{
    for (std::vector<std::vector<Node<Tile>>>& plan : m_vNodes)
    {
        for (std::vector<Node<Tile>>& ligne : plan)
        {
            for (Node<Tile>& node : ligne)
            {
                node.isVisited = false;
                node.comeFrom = nullptr;
                node.totalCost = 0;
                node.costFromStart = 0;

                std::cout << "Detected reset" << std::endl;
            }
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

    Tile* tile = GetTileWithPos(mCurrentSelected->GetPosition());

    if (tile == nullptr)
        return;
    
    mCurrentSelected->ChangePatrouille(tile->mPos);
}

void Pathfinding::ChangeMap(gce::Vector3f32 mousePos, bool create)
{
    if (mCurrentSelected != nullptr)
    {
        mCurrentSelected->SetColor(gce::Color::Green);
        mCurrentSelected = nullptr;
    }

    for (int i = mCurrentDepth - 1; i >= 0; i--)
    {
        Tile* tile = m_vNodes[i][mousePos.y][mousePos.x].data;

        if (create)
        {
            if (i == 0 && tile->mIsObstacle)
            {
                Tile* changeTile = m_vNodes[i][mousePos.y][mousePos.x].data;
                changeTile->mIsObstacle = false;
                changeTile->SetToDraw(true);
                changeTile->SetColor(gce::Color::White);
                mapUpdate = true;
                UpdateNode(&m_vNodes[i][mousePos.y][mousePos.x], false);
                return;
            }
            else if (tile->mIsObstacle == false && i != mCurrentDepth - 1)
            {
                Tile* changeTile = m_vNodes[i + 1][mousePos.y][mousePos.x].data;
                changeTile->mIsObstacle = false;
                changeTile->SetToDraw(true);
                changeTile->SetColor(gce::Color::White);
                mapUpdate = true;
                UpdateNode(&m_vNodes[i + 1][mousePos.y][mousePos.x], false);
                return;
            }
        }
        else
        {
            if (tile->mIsObstacle == false)
            {
                tile->mIsObstacle = true;
                tile->SetToDraw(false);
                mapUpdate = true;
                UpdateNode(&m_vNodes[i][mousePos.y][mousePos.x], true);
                return;
            }
        }
    }
}

void Pathfinding::DeleteMap()
{
    for (Agent* agent : m_vAgents)
    {
        agent->Destroy();
    }
    m_vAgents.clear();
    for(std::vector<std::vector<Tile*>> mvTile : m_vTiles)
    {
        for (std::vector<Tile*> vTile : mvTile)
        {
            for (Tile* tile : vTile)
            {
                mTileToDelete.push_back(tile);
            }
        }
    }
    m_vTiles.clear();
    mpGameManager->mTiles.clear();

    m_vNodes.clear();

    mCurrentSelected = nullptr;
}

void Pathfinding::SwitchMap(int mapNumber)
{
    TYPE = mapNumber;

    DeleteMap();

    OnInitialize();
}

void Pathfinding::ChangeCamera()
{
    Camera* camera = GameManager::Get()->GetCamera();

    if (mCameraType == 1)
    {
        camera->SetPosition({ 15.0f, 8.0f, 25.0f });
        camera->SetRotation({ 180.0f, 0.0f, 0.0f });

        mCameraType = 0;
    }
    else
    {
        camera->SetPosition({ 15.0f, 32.5f, 25.0f });
        camera->SetRotation({ 135.0f, 0.0f, 0.0f });

        mCameraType = 1;
    }
}

void Pathfinding::UpdateNode(Node<Tile>* node, bool becomeObstacle)
{
    Tile* tile = node->data;

    if (becomeObstacle)
    {
        tile->mIsObstacle = true;

        for (Node<Tile>* neighbor : node->neighbors)
        {
            for (int i = 0; i < neighbor->neighbors.size(); i++)
            {
                if (neighbor->neighbors[i] == node)
                {
                    neighbor->neighbors.erase(neighbor->neighbors.begin() + i);
                    continue;
                }
            }
        }

        //mettre a jour celui en dessous si il existe
        if (node->data->mPos.z - 1 >= 0)
        {
            Node<Tile>* currentNode = &m_vNodes[tile->mPos.z - 1][tile->mPos.y][tile->mPos.x];

            SetNeighbors(currentNode);

            for (Node<Tile>* neighbor : currentNode->neighbors)
            {
                neighbor->neighbors.push_back(currentNode);
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

        //mettre a jour celui en dessous si il existe
        if (node->data->mPos.z - 1 >= 0)
        {
            Node<Tile>* currentNode = &m_vNodes[tile->mPos.z - 1][tile->mPos.y][tile->mPos.x];

            for (Node<Tile>* neighbor : currentNode->neighbors)
            {
                for (int i = 0; i < neighbor->neighbors.size(); i++)
                {
                    if (neighbor->neighbors[i] == currentNode)
                    {
                        neighbor->neighbors.erase(neighbor->neighbors.begin() + i);
                        continue;
                    }
                }
            }
        }
    }
}

Tile* Pathfinding::CreateTile()
{
    Tile* newTile = new Tile();
    newTile->OnInitialize();

    mpGameManager->mTiles.push_back(newTile);

    return newTile;
}



void Pathfinding::OnInitialize()
{
    std::vector<std::vector<std::vector<char>>> vMaps;

    switch (TYPE)
    {
    case 1:
        vMaps = GridConfiguration::Type1();
        mCurrentHeight = 10;
        mCurrentWidth = 10;
        mCurrentDepth = 1;
        break;
    case 2:
        vMaps = GridConfiguration::Type2();
        mCurrentHeight = 18;
        mCurrentWidth = 20;
        mCurrentDepth = 1;
        break;
    case 3:
        vMaps = GridConfiguration::Type3();
        mCurrentHeight = 18;
        mCurrentWidth = 32;
        mCurrentDepth = 1;
        break;
    case 4:
        vMaps = GridConfiguration::Type4();
        mCurrentHeight = 5;
        mCurrentWidth = 5;
        mCurrentDepth = 2;
        break;
    default:
        vMaps = GridConfiguration::Type4();
        mCurrentHeight = 10;
        mCurrentWidth = 10;
        break;
    };

    int size = 1;

    int z = 0;
    for (std::vector<std::vector<char>> twoD : vMaps)
    {
        std::vector<std::vector<Tile*>> vectorTemp;
        int y = 0;
        for (std::vector<char> oneD : twoD)
        {
            std::vector<Tile*> temp;
            int x = 0;
            for (char box : oneD)
            {
                Tile* newTile = CreateTile();
                newTile->SetPosition(x, y, z);
                WPosMPos pos = {y, x, z, x, y, z  };
                newTile->mPos = pos;

                if (box == '#')
                {
                    newTile->mIsObstacle = true;
                    newTile->SetColor(gce::Color::Black);
                    newTile->SetToDraw(false);
                }

                temp.push_back(newTile);

                x++;
            }

            vectorTemp.push_back(temp);
            y++;
        }

        m_vTiles.push_back(vectorTemp);
        z++;
    }

    // Initialisation des nodes
    for (const std::vector<std::vector<Tile*>>& plan : m_vTiles)
    {
        std::vector<std::vector<Node<Tile>>> temp2D;

        for (const std::vector<Tile*>& ligne : plan)
        {
            std::vector<Node<Tile>> temp1D;

            for (Tile* tile : ligne)
            {
                Node<Tile> node;
                node.data = tile;
                temp1D.push_back(node);
            }

            temp2D.push_back(temp1D);
        }

        m_vNodes.push_back(temp2D);
    }

    // Initialisation des voisins
    for (int z = 0; z < mCurrentDepth; z++)
    {
        for (int y = 0; y < mCurrentHeight; y++)
        {
            for (int x = 0; x < mCurrentWidth; x++)
            {
                Node<Tile>* currentNode = &m_vNodes[z][y][x];

                SetNeighbors(currentNode);
            }
        }
    }
}

void Pathfinding::OnEvent()
{
    if (GetKey(Keyboard::S) && GetButtonDown(Mouse::LEFT))
    {
        ChangeMap(GameManager::Get()->GetCurrentMousePos(), false);
    }
    else if (GetKey(Keyboard::C) && GetButtonDown(Mouse::LEFT))
    {
        ChangeMap(GameManager::Get()->GetCurrentMousePos(), true);
    }
    else if (GetKeyDown(Keyboard::P))
    {
        ChangePatrouille();
    }
    else if (GetKeyDown(Keyboard::T))
    {
        DeselectAgent();
    }
    else if (GetKeyDown(Keyboard::F1))
    {
        SwitchMap(1);
    }
    else if (GetKeyDown(Keyboard::F2))
    {
        SwitchMap(2);
    }
    else if (GetKeyDown(Keyboard::F3))
    {
        SwitchMap(3);
    }
    else if (GetKeyDown(Keyboard::F4))
    {
        SwitchMap(4);
    }
    else if (GetKeyDown(Keyboard::F5))
    {
        ChangeCamera();
    }

    if (GetButtonDown(Mouse::LEFT) && GetKey(Keyboard::A))
    {
        CreateAgent(GameManager::Get()->GetCurrentMousePos());
    }
    else if (GetButtonDown(Mouse::LEFT) && GetKey(Keyboard::LCONTROL))
    {
        AddPointToCurrentAgent(GameManager::Get()->GetCurrentMousePos());
    }
    else if (GetButtonDown(Mouse::LEFT) && GetKey(Keyboard::S) == false)
    {
        SelectAgent(GameManager::Get()->GetCurrentMousePos());
    }
}

void Pathfinding::OnUpdate()
{
    //for (std::vector<Node<Tile>>& ligne : m_vNodes)
    //{
    //    for (Node<Tile>& node : ligne)
    //    {
    //        gce::Color newColor = gce::Color::White;

    //        if (node.data->mIsObstacle)
    //            newColor = gce::Color::Black;
    //        else if (node.data->mIsLock)
    //            newColor = gce::Color::Blue;
    //        else if (node.data->IsTempLock())
    //        {
    //            newColor = gce::Color::Magenta;
    //            //debug
    //            /*for (Agent* agent : m_vAgents)
    //            {
    //                if (node.data->GetShape()->getGlobalBounds().contains(agent->GetPosition()))
    //                {
    //                    newColor = sf::Color::Magenta;
    //                }
    //            }  */              
    //        }

    //        node.data->SetColor(newColor);
    //    }
    //}
}

void Pathfinding::OnEndUpdate()
{
    mapUpdate = false;

    for (Tile* tile : mTileToDelete)
    {
        delete tile;
    }

    mTileToDelete.clear();
}
