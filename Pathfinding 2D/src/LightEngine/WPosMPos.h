#pragma once

class WPosMPos
{
public:
	int row = 0;
	int col = 0;

	float x = 0;
	float y = 0;

	bool operator== (WPosMPos other);
};

