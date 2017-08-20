#ifndef GLOBAL_VAR_H_INCLUDED
#define GLOBAL_VAR_H_INCLUDED

#define _WIN32_WINNT 0x0500
#include <ole2.h>
#include <olectl.h>
#include <queue>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <time.h>
#include <stdio.h>

using namespace std;

/// GET_MSX_INFO

HWND hwnd;
RECT resolution;
POINT origin, restart_button, mine_number, marker;
int grid_width, grid_height, mines;

/// COLOR_READING

struct rgb_t
{
    int r, g, b;
};

struct ind
{
    int u, v;
};

const int color_code[12][3] =
{
    {255, 255, 255}, // blank
    {0, 0, 255}, // 1
    {0, 128, 0}, // 2
    {255, 0, 0}, // 3
    {0, 0, 128}, // 4
    {128, 0, 0}, // 5
    {0, 128, 128}, // 6
    {0, 0, 0}, // 7
    {96, 96, 128}, // 8
    {192, 192, 192}, // unknown
    {128, 0, 64}, // flag
    {224, 96, 0}, // end game
};

int color_dict[256][256][256], cap_x, cap_y, cap_w, cap_h;

HDC hdc = GetDC(HWND_DESKTOP);
BYTE* screen_data = 0;

/// SOLVE

const int dx[4] = {0, 1, 0, -1},
          dy[4] = {1, 0, -1, 0},
          mx[8] = {0, 1, 0, -1, 1, 1, -1, -1},
          my[8] = {1, 0, -1, 0, 1, -1, 1, -1};
int last_val, delay = 0, suc = 0, grid[100][100];
bool wut_nomine[8], wut_mine[8];

vector < vector<int> > current_combination;
vector <int> rv[256], gv[256], bv[256], curx, cury;
vector <double> cdx, cdy;
queue <ind> q, s, lfb;


#endif // GLOBAL_VAR_H_INCLUDED
