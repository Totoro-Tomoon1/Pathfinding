#include "WPosMPos.h"

bool WPosMPos::operator==(WPosMPos other)
{
    bool WPos = (x == other.x && y == other.y);
    bool MPos = (col == other.col && row == other.row);

    return WPos && MPos;
}
