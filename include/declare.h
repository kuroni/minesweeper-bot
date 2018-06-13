#ifndef DECLARE_H
#define DECLARE_H
#include <vector>
#include <algorithm>
#include <iostream>
#include <windows.h>


const int DX[8] = {0, 0, 1, 1, 1, -1, -1, -1}, DY[8] = {-1, 1, -1, 0, 1, -1, 0, 1};
const int SZ = 512;

int m, n, min, num[SZ][SZ];

struct SPosition
{
    int x, y;
    SPosition() : x(0), y(0) {}
    SPosition(int _x, int _y) : x(_x), y(_y) {}

    bool on_board()
    {
        return x >= 0 && x < m && y >= 0 && y < n;
    }

    bool valid()
    {
    }

    bool border_valid()
    {
    }
};

inline bool operator<(const SPosition &a, const SPosition &b)
{
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

#endif // DECLARE_H