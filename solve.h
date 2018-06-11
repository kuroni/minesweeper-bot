#ifndef SOLVE_H
#define SOLVE_H
#include "declare.h"
#include "move.h"
#include "read.h"
#include <vector>

namespace solve
{
struct component
{
    std::vector<position> ve;

    bool valid(int mask)
    {
    }
};

void open(const position &u)
{
    if (num[u.x][u.y] == -1)
    {
        move::left_click(u);
        if ((num[u.x][u.y] = read::read(u)) == 0)
            for (int i = 0; i < 8; i++)
                if (num[u.x + DX[i]][u.y + DY[i]] == -1)
                    open(position(u.x + DX[i], u.y + DY[i]));
    }
}

void mark(const position &u)
{
    move::right_click(u);
    num[u.x][u.y] = 10;
}

void naive()
{
    bool border = false;
    for (int x = 1; x <= m; x++)
        for (int y = 1; y <= n; y++)
            if (position(x, y).border_valid())
            {
                border = true;
                position u = position(x, y);
                bool move = false;
                component cur;
                for (int i = 0; i < 8; i++)
                    if (num[u.x + DX[i]][u.y + DY[i]] == -1)
                        cur.ve.push_back(position(u.x + DX[i], u.y + DY[i]));
                int yes = (1 << cur.ve.size()) - 1, no = 0;
                for (int mask = 0; mask < (1 << cur.ve.size()); mask++)
                    if (cur.valid(mask))
                    {
                        yes &= mask;
                        no |= mask;
                    }
                for (int i = 0; i < cur.ve.size(); i++)
                {
                    if (yes >> i & 1)
                    {
                        move = true;
                        open(cur.ve[i]);
                    }
                    if (no >> i & 1)
                    {
                        move = false;
                        mark(cur.ve[i]);
                    }
                }
                if (move)
                {
                    naive();
                    return;
                }
            }
    if (border)
        tanker();
}

void tanker();
} // namespace solve

#endif // SOLVE_H