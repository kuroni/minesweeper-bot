#ifndef DECLARE_H
#define DECLARE_H
#include <vector>
#include <algorithm>
#include <windows.h>

const int M = 105, N = 105, DX[8] = {0, 0, 1, 1, 1, -1, -1, -1}, DY[8] = {-1, 1, -1, 0, 1, -1, 0, 1};

struct SPosition
{
    int x, y;
    SPosition() : x(0), y(0) {}
    SPosition(int _x, int _y) : x(_x), y(_y) {}

    bool valid()
    {
    }

    bool border_valid()
    {
    }

    bool operator<(const SPosition &b)
    {
        return x < b.x || (x == b.x && y < b.y);
    }
};

int m, n, num[M][N];

#endif // DECLARE_H