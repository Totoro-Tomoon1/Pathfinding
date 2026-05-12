#pragma once
#include <vector>

template<typename T>
struct Node
{
    T* data = nullptr;

    bool isVisited = false;
    Node* comeFrom = nullptr;

    int totalCost = 0;
    int costFromStart = 0;

    std::vector<Node<T>*> neighbors;
    std::vector<Node<T>*> diagoNeighbors;
};

