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

HWND hwnd;
RECT resolution;
POINT origin, restart_button, mine_number, marker;
int grid_width, grid_height, mines;

/// COLOR_READING

struct ind
{
    int u, v;
};

HDC hdc = GetDC(HWND_DESKTOP);
BYTE *screen_data = 0;

namespace NCursor
{
void move_to(int x, int y)
{
    SetCursorPos(origin.x + (x * 16), origin.y + (y * 16));
}

inline void left_click(int x, int y)
{
    move_to(x, y);
    INPUT Input = {0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    ::SendInput(1, &Input, sizeof(INPUT));
    ::ZeroMemory(&Input, sizeof(INPUT));
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    ::SendInput(1, &Input, sizeof(INPUT));
}

inline void right_click(int x, int y)
{
    move_to(x, y);
    INPUT Input = {0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    ::SendInput(1, &Input, sizeof(INPUT));
    ::ZeroMemory(&Input, sizeof(INPUT));
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    ::SendInput(1, &Input, sizeof(INPUT));
}

void restart_game()
{
    SetCursorPos(restart_button.x, restart_button.y);
    INPUT Input = {0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    ::SendInput(1, &Input, sizeof(INPUT));
    ::ZeroMemory(&Input, sizeof(INPUT));
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    ::SendInput(1, &Input, sizeof(INPUT));
}
}

namespace NRead
{
struct rgb_t
{
    int r, g, b;
};

const int color_code[12][3] =
    {
        {255, 255, 255}, // blank
        {0, 0, 255},     // 1
        {0, 128, 0},     // 2
        {255, 0, 0},     // 3
        {0, 0, 128},     // 4
        {128, 0, 0},     // 5
        {0, 128, 128},   // 6
        {0, 0, 0},       // 7
        {96, 96, 128},   // 8
        {192, 192, 192}, // unknown
        {128, 0, 64},    // flag
        {224, 96, 0},    // end game
};

int color_dict[256][256][256], cap_x, cap_y, cap_w, cap_h;

void init()
{
    memset(color_dict, -1, sizeof(color_dict));
    for (int i = 0; i < 12; i++)
        color_dict[color_code[i][0]][color_code[i][1]][color_code[i][2]] = i;
    RECT desktop;
    CONST HWND hdesktop = GetDesktopWindow();
    GetWindowRect(hdesktop, &desktop);
    cap_x = 0;
    cap_y = 0;
    cap_w = desktop.right;
    cap_h = desktop.bottom;
}

/// BITMAP HANDLE

void generate_bitmap()
{
    hdc = GetDC(HWND_DESKTOP);
    HDC hdc_mem = CreateCompatibleDC(hdc);
    HBITMAP hbmp = CreateCompatibleBitmap(hdc, cap_w, cap_h);
    HGDIOBJ bmp_hold = SelectObject(hdc_mem, hbmp);
    BitBlt(hdc_mem, 0, 0, cap_w, cap_h, hdc, 0, 0, SRCCOPY);
    SelectObject(hdc_mem, bmp_hold);
    BITMAPINFOHEADER bmi = {0};
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biPlanes = 1;
    bmi.biBitCount = 32;
    bmi.biWidth = cap_w;
    bmi.biHeight = -cap_h;
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage = 4 * cap_w * cap_h;
    if (screen_data)
        free(screen_data);
    screen_data = (BYTE *)malloc(4 * cap_w * cap_h);
    GetDIBits(hdc_mem, hbmp, 0, cap_h, screen_data, (BITMAPINFO *)&bmi, DIB_RGB_COLORS);
    ReleaseDC(GetDesktopWindow(), hdc);
    DeleteDC(hdc_mem);
    DeleteObject(hbmp);
}

int scan_box(int u, int v)
{
    int x = origin.x + (16 * u), y = origin.y + (16 * v);
    rgb_t p = {screen_data[4 * (y * cap_w + x) + 2], screen_data[4 * (y * cap_w + x) + 1], screen_data[4 * (y * cap_w + x)]};
    return color_dict[p.r][p.g][p.b];
}

bool update_new_boxes(int u, int v)
{
    generate_bitmap();
    q.push({u, v});
    while (!q.empty())
    {
        ind cur = q.front();
        if (grid[cur.u][cur.v] == 9)
        {
            grid[cur.u][cur.v] = scan_box(cur.u, cur.v);
            if (grid[cur.u][cur.v] != 9)
            {
                if ((grid[cur.u][cur.v] < 9) && (grid[cur.u][cur.v] > 0))
                    s.push(cur);
                if (grid[cur.u][cur.v] == 11)
                    return 0;
                cout << grid[cur.u][cur.v] << ' ';
                for (int i = 0; i < 4; i++)
                {
                    ind tar = {cur.u + dx[i], cur.v + dy[i]};
                    if (tar.u < 0 || tar.u >= grid_width || tar.v < 0 || tar.v >= grid_height)
                        continue;
                    else if (grid[tar.u][tar.v] == 9)
                        q.push(tar);
                }
            }
        }
        q.pop();
    }
    return 1;
}
}

int main()
{   
    do
    {
        hwnd = FindWindow(NULL, TEXT("Minesweeper X"));
        if (hwnd != 0)
            cout << "Application found, please do not move the window throughout the game\n";
        else
        {
            cout << "Application not found, please open ...";
            cin.get();
        }
    } while (hwnd == 0);
    cout << "Bringing the application to top ... \n";
    ::SetForegroundWindow(hwnd);
    Sleep(500);
    GetWindowRect(hwnd, &resolution);
    origin.x = resolution.left + 22;
    origin.y = resolution.top + 110;
    grid_width = (resolution.right - 19 - origin.x) / 16 + 1;
    grid_height = (resolution.bottom - 19 - origin.y) / 16 + 1;
    cout << "Board size found: " << grid_width << " " << grid_height << '\n';
    mine_number.x = resolution.left + 23;
    mine_number.y = resolution.top + 72;
    generate_bitmap();
    mines = custom_scan(mine_number.x, mine_number.y) * 100;
    mines += custom_scan(mine_number.x + 13, mine_number.y) * 10;
    mines += custom_scan(mine_number.x + 26, mine_number.y);
    cout << "Mines found: " << mines << '\n';
    restart_button.x = (resolution.left + resolution.right) / 2;
    restart_button.y = resolution.top + 74;
    for (int i = 0; i < grid_height; i++)
        fill(grid[i], grid[i] + grid_width, 9);
    NRead::init();
    init_grid();
    generate_bitmap();
    //solve();
}
