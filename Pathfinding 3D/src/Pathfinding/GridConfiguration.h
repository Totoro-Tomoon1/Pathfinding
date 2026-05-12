#pragma once
#include <vector>

#include "Node.h"

class Tile;

class GridConfiguration
{
public:
	static std::vector<std::vector<std::vector<char>>> Type1();
	static std::vector<std::vector<std::vector<char>>> Type2();
	static std::vector<std::vector<std::vector<char>>> Type3();
	static std::vector<std::vector<std::vector<char>>> Type4();
};

