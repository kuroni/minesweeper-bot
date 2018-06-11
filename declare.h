#ifndef DECLARE_H
#define DECLARE_H

const int M = 105, N = 105, DX[8] = {0, 0, 1, 1, 1, -1, -1, -1}, DY[8] = {-1, 1, -1, 0, 1, -1, 0, 1};

struct SPosition
{
    int x, y;
    SPosition(int _x, int _y) : x(_x), y(_y) {}

    bool valid()
    {
    }

    bool border_valid()
    {
    }
};

int m, n, num[M][N];

#endif // DECLARE_H