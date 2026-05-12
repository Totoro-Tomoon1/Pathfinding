#pragma once

class WPosMPos
{
public:
	int row = 0;
	int col = 0;
	int height = 0;

	float x = 0;
	float y = 0;
	float z = 0;

	bool operator== (WPosMPos other);
};

