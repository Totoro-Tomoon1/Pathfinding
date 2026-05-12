#include "pch.h"
#include "WPosMPos.h"

bool WPosMPos::operator==(WPosMPos other)
{
    bool WPos = (x == other.x && y == other.y && z == other.z);
    bool MPos = (col == other.col && row == other.row && height == other.height);

    return WPos && MPos;
}

