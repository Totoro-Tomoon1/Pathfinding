#include "Agent.h"
#include <iostream>
#include "Debug.h"

#include "Tile.h"
#include "Pathfinding.h"

void Agent::AddToPriorityQueue(std::vector<Node<Tile>*>& priority_queue, Node<Tile>* nodeToAdd)
{
    if (priority_queue.empty())
    {
        priority_queue.push_back(nodeToAdd);
        return;
    }
    //si = prendre le plus loin de start
    for (int i = 0; i < priority_queue.size(); i++)
    {
        if (nodeToAdd->totalCost < priority_queue[i]->totalCost)
        {
            priority_queue.insert(priority_queue.begin() + i, nodeToAdd);
            return;
        }
        else if (nodeToAdd->totalCost == priority_queue[i]->totalCost)
        {
            if (nodeToAdd->costFromStart > priority_queue[i]->costFromStart)
            {
                priority_queue.insert(priority_queue.begin() + i, nodeToAdd);
                return;
            }
        }
    }

    priority_queue.push_back(nodeToAdd);
}

bool Agent::FindPath(WPosMPos newPos, bool addToPath)
{
    m_pScene->ResetNodes();

    sf::Vector2i endPos = sf::Vector2i(newPos.col, newPos.row);

    Node<Tile>* start;

    if (addToPath == false)
        start = m_pScene->GetNodeWithTabPos(/*m_pScene->GetPosInTab(GetPosition())*/mCurrentTabPos);
    else
        start = m_vNodesPath.back();

    if (start == nullptr)
    {
        std::cout << "start is nullptr" << std::endl;
        return false;
    }
    else
    {
        //std::cout << "start pos in tab : " << start->data->mPos.col << " col and " << start->data->mPos.row << " row" << std::endl;
    }
    
    Node<Tile>* end = m_pScene->GetNodeWithTabPos(endPos);

    Tile* tileStart = start->data;
    Tile* tileEnd = end->data;

    start->totalCost = abs(tileStart->mPos.col - tileEnd->mPos.col) + abs(tileStart->mPos.row - tileEnd->mPos.row);

    std::vector<Node<Tile>*> priority_queue;
    priority_queue.push_back(start);

    while (!priority_queue.empty())
    {
        Node<Tile>* node = priority_queue[0];
        priority_queue.erase(priority_queue.begin());

        Tile* tile = node->data;

        node->isVisited = true;

        if (sf::Vector2i(tile->mPos.row, tile->mPos.col) == sf::Vector2i(tileEnd->mPos.row, tileEnd->mPos.col))
        {
            return true;
        }

        for (Node<Tile>* neighbor : node->neighbors)
        {
            Tile* tileN = neighbor->data;

            if (tileN->mIsObstacle || tileN->mIsLock)
            {
                continue;
            }

            if (neighbor->isVisited)
            {
                //si visiter verifier si le chemin actuel serait plus interessant
                int otherCost = neighbor->totalCost;

                int newCostFromStart = node->costFromStart + 1;
                int newTotalCost = abs(tileN->mPos.col - endPos.x) + abs(tileN->mPos.row - endPos.y) + newCostFromStart;

                if (otherCost > newTotalCost)
                {
                    neighbor->comeFrom = node;
                    neighbor->costFromStart = newCostFromStart;
                    neighbor->totalCost = newTotalCost;
                }

                continue;
            }

            neighbor->isVisited = true;

            int newCostFromStart = node->costFromStart + 1;
            neighbor->comeFrom = node;
            neighbor->costFromStart = newCostFromStart;
            neighbor->totalCost = abs(tileN->mPos.col - endPos.x) + abs(tileN->mPos.row - endPos.y) + neighbor->costFromStart;

            AddToPriorityQueue(priority_queue, neighbor);
        }
        
        int i = 0;
        for (Node<Tile>* neighbor : node->diagoNeighbors)
        {
            if (neighbor == nullptr)
            {
                i++;
                continue;
            }

            Tile* tileN = neighbor->data;

            if (tileN->mIsObstacle || tileN->mIsLock)
            {
                i++;
                continue;
            }

            if (neighbor->isVisited)
            {
                //si visiter verifier si le chemin actuel serait plus interessant
                int otherCost = neighbor->totalCost;

                int newCostFromStart = node->costFromStart + 1;
                int newTotalCost = abs(tileN->mPos.col - endPos.x) + abs(tileN->mPos.row - endPos.y) + newCostFromStart;

                if (otherCost > newTotalCost)
                {
                    neighbor->comeFrom = node;
                    neighbor->costFromStart = newCostFromStart;
                    neighbor->totalCost = newTotalCost;
                }

                i++;
                continue;
            }

            sf::Vector2f posWorld = { neighbor->data->mPos.x, neighbor->data->mPos.y };

            sf::Vector2i pos1;
            sf::Vector2i pos2;

            sf::Vector2i posTab = m_pScene->GetPosInTab(posWorld);
            mCurrentTabPos;

            switch (i)
            {
            case 0:
            {
                pos1 = posTab + sf::Vector2i(1, 0);
                pos2 = posTab + sf::Vector2i(0, 1);
                break;
            }
            case 1:
            {
                pos1 = posTab + sf::Vector2i(-1, 0);
                pos2 = posTab + sf::Vector2i(0, 1);
                break;
            }
            case 2:
            {
                pos1 = posTab + sf::Vector2i(-1, 0);
                pos2 = posTab + sf::Vector2i(0, -1);
                break;
            }
            case 3:
            {
                pos1 = posTab + sf::Vector2i(1, 0);
                pos2 = posTab + sf::Vector2i(0, -1);
                break;
            }
            };

            Node<Tile>* node1 = m_pScene->GetNodeWithTabPos(pos1);
            Node<Tile>* node2 = m_pScene->GetNodeWithTabPos(pos2);

            if (node1->data->mIsObstacle || node2->data->mIsObstacle)
            {
                i++;
                continue;
            }

            if (node1->data->mIsLock || node2->data->mIsLock)
            {
                i++;
                continue;
            }
            neighbor->isVisited = true;

            int newCostFromStart = node->costFromStart + 1;
            neighbor->comeFrom = node;
            neighbor->costFromStart = newCostFromStart;
            neighbor->totalCost = abs(tileN->mPos.col - endPos.x) + abs(tileN->mPos.row - endPos.y) + neighbor->costFromStart;

            AddToPriorityQueue(priority_queue, neighbor);

            i++;
        }
    }

    return false;
}

void Agent::GetPath(WPosMPos newPos, bool addToPath)
{
    int offset = 0;
    int offsetNode = 0;

    if (!addToPath)
    {
        m_vPath.clear();
        m_vNodesPath.clear();
    }
    else
    {
        offset = m_vPath.size();
        offsetNode = m_vNodesPath.size();
    }

    Node<Tile>* node = m_pScene->GetNodeWithTabPos({ newPos.col, newPos.row });

    while (node != nullptr)
    {
        // Ajoute la position
        m_vPath.insert(m_vPath.begin() + offset, node->data->GetPosition());

        // Ajoute aussi la node (obligatoire pour CheckNextNode)
        m_vNodesPath.insert(m_vNodesPath.begin() + offsetNode, node);

        // Stop : si on est à la startNode
        if (node->comeFrom == nullptr)
        {
            return;
        }

        // Suivant
        node = node->comeFrom;
    }
}

void Agent::DrawPath()
{
    if (m_vPath.size() == 0)
        return;

    Node<Tile>* node = m_pScene->GetNodeWithPos(m_vPath[0]);

    for (int i = 1; i < m_vPath.size(); i++)
    {
        Node<Tile>* nextNode = m_pScene->GetNodeWithPos(m_vPath[i]);

        Debug::DrawLine(node->data->GetPosition().x, node->data->GetPosition().y, nextNode->data->GetPosition().x, nextNode->data->GetPosition().y, sf::Color::Black);

        node = nextNode;
    }
}

//void Agent::ClearPath()
//{
//    for (sf::Vector2f pos : m_vPath)
//    {
//        Node<Tile>* node = m_pScene->GetNodeWithPos(pos);
//        m_vNodesPath.push_back(node);
//    }
//
//    //si la direction des tiles est la meme faut les merges
//    for (int i = 1; i < m_vPath.size() - 1; i++)
//    {
//        Node<Tile>* lastNode = m_pScene->GetNodeWithPos(m_vPath[i - 1]);
//        Node<Tile>* nextNode = m_pScene->GetNodeWithPos(m_vPath[i + 1]);
//
//        if (lastNode->data->mPos.col == nextNode->data->mPos.col || lastNode->data->mPos.row == nextNode->data->mPos.row)
//        {
//            m_vPath.erase(m_vPath.begin() + i);
//
//            i--;
//        }
//    }
//}

void Agent::CheckNextNode()
{
    if (m_vNodesPath.size() <= 1)
        return;

    Node<Tile>* current = m_vNodesPath[0];
    Node<Tile>* next = m_vNodesPath[1];

    if (next->data->IsTempLock() && next->data->mAgent != this)
    {
        mIsWaiting = true;
        mToStop = true;
        mWaitProgress = mWaitTiming;

        //next->data->mIsLock = true;
        return;
    }

    // lock uniquement la vraie nextNode
    next->data->SetTempLock(true, this);
    current->data->SetTempLock(true, this);

    if (next->data->GetShape()->getGlobalBounds().contains(GetPosition()))
    {
        current->data->SetTempLock(false, nullptr);

        mCurrentTabPos = { next->data->mPos.col, next->data->mPos.row };

        // avancer dans le chemin
        m_vNodesPath.erase(m_vNodesPath.begin());

        // lock suivante si encore
        if (m_vNodesPath.size() > 1)
        {
            if (m_vNodesPath[1]->data->mIsLock == true)
            {
                WPosMPos nextPos;

                if (mIsPatrouille)
                {
                    nextPos = m_vPatrouillePoint[mCurrentPos];
                }
                else
                {
                    nextPos = mNextPos;
                }

                if (m_vNodesPath[1]->data->mPos == nextPos)//si dernier bloquer
                {
                    mIsMoving = false;
                    m_vPath.clear();
                    next->data->mIsLock = true;
                    mToStop = true;

                    return;
                }
                else if (FindPath(nextPos))
                {
                    GetPath(nextPos);
                }
            }
            else if (m_vNodesPath[1]->data->mIsObstacle == true)
            {
                mIsMoving = false;
                m_vPath.clear();
                next->data->mIsLock = true;
                mToStop = true;

                return;
            }
            else if (m_vNodesPath[1]->data->IsTempLock()) //si deja temp lock alors attendre
            {
                mIsWaiting = true;

                next->data->mIsLock = true;

                mToStop = true;
                mWaitProgress = mWaitTiming;
            }
            else
            {
                m_vNodesPath[1]->data->SetTempLock(true, this);
            }
        }
    }

    /*if (mIsWaiting)
        return;

    if (m_vNodesPath.size() <= 1)
        return;

    Node<Tile>* currentNode = m_vNodesPath[0];
    //currentNode->isTempLock = true;
    
    Node<Tile>* nextNode = m_vNodesPath[1];

    if (/*nextNode->isLock == false && nextNode->isTempLock == falsenextNode->data->GetShape()->getGlobalBounds().contains(GetPosition()))
    {
        m_vNodesPath.erase(m_vNodesPath.begin());
        //currentNode->isTempLock = false;
        //nextNode->isTempLock = true;
        //mCurrentNode = nextNode;
        return;
    }

    if (nextNode->data->mIsLock)
    {
        WPosMPos nextPos;

    //    GoToPosition(currentNode->data->GetPosition().x, currentNode->data->GetPosition().y, 100.0f); //� v�rifier mais � priori inutile
    //    std::cout << "2" << std::endl;

    //    mToStop = true;

        if (nextPos == nextNode->data->mPos)//si la case final est bloquer
        {
            //GoToPosition(currentNode->data->GetPosition().x, currentNode->data->GetPosition().y, 100.0f); //� v�rifier mais � priori inutile
            std::cout << "1" << std::endl;
            mIsMoving = false;
            m_vPath.clear();
            currentNode->data->mIsLock = true;
            mToStop = true;

            return;
        }

        if (FindPath(nextPos))
        {
            GetPath(nextPos);
            ClearPath();
        }
    }

    //if (nextNode->isTempLock)
    //{
    //    mIsWaiting = true;
    //    currentNode->isLock = true;

    //    //GoToPosition(currentNode->data->GetPosition().x, currentNode->data->GetPosition().y, 100.0f); //� v�rifier mais � priori inutile
    //    std::cout << "2" << std::endl;

    //    mToStop = true;

    //    mWaitProgress = mWaitTiming;
    //}

    return;*/
}

void Agent::Init()
{
    Node<Tile>* node = m_pScene->GetNodeWithPos(GetPosition());

    node->data->mIsLock = true;

    mCurrentTabPos = {node->data->mPos.col, node->data->mPos.row};
}

void Agent::OnUpdate()
{
    mToStop = false;

    if (mIsMoving && m_vPath.empty() == false)
    {
        if (m_pScene->mapUpdate)
        {
            WPosMPos nextPos;

            if (mIsPatrouille)
            {
                nextPos = m_vPatrouillePoint[mCurrentPos];
            }
            else
            {
                nextPos = mNextPos;
            }

            if (FindPath(nextPos))
            {
                GetPath(nextPos);
                //ClearPath();
            }
        }

        sf::Vector2f goTo = m_vPath[0];

        DrawPath();

        CheckNextNode();

        if (mIsWaiting)
        {
            mWaitProgress -= GetDeltaTime();

            if (mWaitProgress <= 0)
            {
                mIsWaiting = false;

                Node<Tile>* node = m_pScene->GetNodeWithTabPos(mCurrentTabPos);

                node->data->mIsLock = false;
            }
            else
                return;
        }

        if (mToStop == false)
        {
            GoToPosition(goTo.x, goTo.y, mSpeed);
            //std::cout << "3" << std::endl;
        }

        if (GetPosition() == goTo)
        {
            m_vPath.erase(m_vPath.begin());

            if (m_vPath.empty())
            {
                if (mIsPatrouille)
                {
                    if (m_vPatrouillePoint.size() > mCurrentPos + 1)
                        mCurrentPos++;
                    else
                        mCurrentPos = 0;

                    WPosMPos nextPos = m_vPatrouillePoint[mCurrentPos];

                    if (FindPath(nextPos))
                    {
                        GetPath(nextPos);
                        //ClearPath();
                    }
                    else
                    {
                        Node<Tile>* node = m_pScene->GetNodeWithTabPos(mCurrentTabPos);

                        node->data->mIsLock = true;
                        m_vPatrouillePoint.clear();
                        mIsPatrouille = false;
                        SetColor(sf::Color::Green);
                        mIsMoving = false;
                        m_pScene->DeselectAgent();
                    }
                }
                else
                {
                    mIsMoving = false;

                    Node<Tile>* node = m_pScene->GetNodeWithTabPos(mCurrentTabPos);

                    node->data->mIsLock = true;
                    //node->isTempLock = false;
                }
            }
            
        }
    }
    else if (mShowPath)
    {
        DrawPath();
    }
}

void Agent::OnDestroy()
{
    Node<Tile>* node = m_pScene->GetNodeWithTabPos(mCurrentTabPos);

    node->data->mIsLock = false;
}

void Agent::Move(WPosMPos newPos, bool move)
{
    if (mIsMoving)
        return;

	if (FindPath(newPos))
	{
        //Node<Tile>* node = m_pScene->GetNodeWithPos(GetPosition());

        if (move)
        {
            /*if (mCurrentNode != nullptr)
                mCurrentNode->data->mIsTempLock = false;

            if (mNextNode != nullptr)
                mNextNode->data->mIsTempLock = false;*/

            mIsMoving = true;
            mShowPath = false;

            /*if (node != nullptr)
            {
                node->data->mIsLock = false;
                node->data->mIsTempLock = true;
            }*/

            Node<Tile>* currentNode = m_pScene->GetNodeWithTabPos(mCurrentTabPos);
            currentNode->data->mIsLock = false;
            currentNode->data->SetTempLock(true, this);
        }
        else
        {
            mShowPath = true;
        }

        //a changer avec un deplacement progressif
        GetPath(newPos);
        //ClearPath();

        if (mIsPatrouille)
        {
            if (move)
            {
                m_vPatrouillePoint.insert(m_vPatrouillePoint.begin() + 1, newPos);
            }
        }
        else
        {
            mNextPos = newPos;
        }
	}
}

void Agent::AddPointToPath(WPosMPos newPos)
{
    if (mIsMoving == false)
        return;

    mNextPos = newPos;

    if (mIsPatrouille)
    {
        m_vPatrouillePoint.push_back(newPos);
    }
    else if (FindPath(newPos, true))
    {
        GetPath(newPos, true);
        //ClearPath();
    }    
}

void Agent::ChangePatrouille(WPosMPos currentPos)
{
    bool temp = !mIsPatrouille;

    if (temp == false)
    {
        m_vPatrouillePoint.clear();
        mShape.setFillColor(sf::Color::Red);

        mIsPatrouille = false;
    }
    else
    {
        if (mIsMoving)
            return;

        mIsPatrouille = true;
        mShape.setFillColor(sf::Color::Blue);
        m_vPatrouillePoint.push_back(currentPos);
    }
}

