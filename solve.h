#ifndef SOLVE_H_INCLUDED
#define SOLVE_H_INCLUDED

#include "global_var.h"
#include "click.h"
#include "color_reading.h"

bool check_box(int x, int y)
{
    int cnt = 0, m = 0, u, v, i;
    if (x < 0 || x >= grid_width || y < 0 || y >= grid_height)
        return 1;
    else if (grid[x][y] == 9 || grid[x][y] == 10 || grid[x][y] == 12)
        return 1;
    for (i = 0; i < 8; i++)
    {
        u = x + mx[i];
        v = y + my[i];
        if ((u >= 0) && (u < grid_width) && (v >= 0) && (v < grid_height))
        {
            if (grid[u][v] == 9)
                m++;
            else if (grid[u][v] == 10 || grid[u][v] == 12)
                cnt++;
        }
    }
    if ((grid[x][y] >= cnt) && (grid[x][y] - cnt <= m))
        return 1;
    else return 0;
}

bool check_around(int x, int y)
{
    for (int i = 0; i < 8; i++)
        if (!check_box(x + mx[i], y + my[i]))
        return 0;
    return 1;
}
#endif // SOLVE_H_INCLUDED
