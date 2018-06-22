#pragma once
#include "declare.hpp"
#include "handle.hpp"

namespace NSolve
{
struct SComponent
{
    std::vector<SPosition> ve;

    bool valid(int mask)
    {
        bool ret = true;
        for (unsigned int i = 0; i < ve.size(); i++)
            num[ve[i].x][ve[i].y] = 9 + (mask >> i & 1);
        for (SPosition &u : ve)
            for (int i = 0; i < 8; i++)
                ret &= SPosition(u.x + DX[i], u.y + DY[i]).valid();
        for (SPosition &u : ve)
            num[u.x][u.y] = -1;
        return ret;
    }
};

// DFS to check for new cells' information
void open(SPosition u)
{
    if (num[u.x][u.y] == -1 && (num[u.x][u.y] = NHandle::read(u.pixel_pos(), NHandle::board)) == 0)
        for (int i = 0; i < 8; i++)
            if (SPosition(u.x + DX[i], u.y + DY[i]).on_board() && num[u.x + DX[i]][u.y + DY[i]] == -1)
                open(SPosition(u.x + DX[i], u.y + DY[i]));
}

#define check_status(if_return)                                                \
    st = NHandle::read(SPosition(win.left + 66, win.top + 61), NHandle::face); \
    if (st != 0)                                                               \
    {                                                                          \
        if (st == 2)                                                           \
        {                                                                      \
            if (if_return)                                                     \
                std::cout << "Fail\n";                                         \
            return;                                                            \
        }                                                                      \
        if (st == 3)                                                           \
        {                                                                      \
            if (if_return)                                                     \
                std::cout << "Finish\n";                                       \
            return;                                                            \
        }                                                                      \
    }

void naive();
void tanker();
void probability();

void solve()
{
    for (int x = 0; x < m; x++)
        for (int y = 0; y < n; y++)
            num[x][y] = -1;
    for (int x = 0; x < m; x++)
        for (int y = 0; y < n; y++)
            if (num[x][y] == -1)
            {
                NHandle::click(SPosition(x, y).pixel_pos());
                check_status(true);
                open(SPosition(x, y));
                naive();
                check_status(true);
            }
    std::cout << "Finish\n";
    return;
}

// Only solve adjacent open cells of a number cell
void naive()
{
    bool border = false;
    for (int x = 0; x < m; x++)
        for (int y = 0; y < n; y++)
            if (SPosition(x, y).border_valid())
            {
                border = true;
                SPosition u = SPosition(x, y);
                bool move = false;
                SComponent cur;
                for (int i = 0; i < 8; i++)
                    if (SPosition(u.x + DX[i], u.y + DY[i]).on_board() && num[u.x + DX[i]][u.y + DY[i]] == -1)
                        cur.ve.push_back(SPosition(u.x + DX[i], u.y + DY[i]));
                int yes = (1 << cur.ve.size()) - 1, no = 0;
                for (int mask = 0; mask < (1 << cur.ve.size()); mask++)
                    if (cur.valid(mask))
                    {
                        yes &= mask;
                        no |= mask;
                    }
                for (unsigned int i = 0; i < cur.ve.size(); i++)
                {
                    if (yes >> i & 1)
                    {
                        move = true;
                        num[cur.ve[i].x][cur.ve[i].y] = 10;
                    }
                    if (!(no >> i & 1))
                    {
                        move = true;
                        NHandle::click(cur.ve[i].pixel_pos());
                        check_status(false);
                        open(cur.ve[i]);
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

// Create disjoint sets of open border cells and solve each of them
void tanker()
{
}

// Tanker but with probability
} // namespace NSolve