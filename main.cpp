#include <iostream>
#include <cstdlib>
#include <queue>
#include <windows.h>

POINT tl, res, mn;

namespace NCuresor
{
void move_to(int x, int y)
{
    SetCursorPos(tl.x + (x * 16), tl.y + (y * 16));
}

inline void left_click(int x, int y)
{
    move_to(x, y);
    INPUT Input = {0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &Input, sizeof(INPUT));
    ZeroMemory(&Input, sizeof(INPUT));
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &Input, sizeof(INPUT));
}

inline void right_click(int x, int y)
{
    move_to(x, y);
    INPUT Input = {0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    SendInput(1, &Input, sizeof(INPUT));
    ZeroMemory(&Input, sizeof(INPUT));
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    SendInput(1, &Input, sizeof(INPUT));
}

void res_game()
{
    SetCursorPos(res.x, res.y);
    INPUT Input = {0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &Input, sizeof(INPUT));
    ZeroMemory(&Input, sizeof(INPUT));
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &Input, sizeof(INPUT));
}
}

namespace NRead
{
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

struct SRgb
{
    int r, g, b;
};
BYTE *screen_data = 0;
int dict[256][256][256], w, h;

void init()
{
    memset(dict, -1, sizeof(dict));
    for (int i = 0; i < 12; i++)
        dict[color_code[i][0]][color_code[i][1]][color_code[i][2]] = i;
    RECT desktop;
    CONST HWND hdesktop = GetDesktopWindow();
    GetWindowRect(hdesktop, &desktop);
    w = desktop.right;
    h = desktop.bottom;
    std::cout << w << " " << h << '\n';
}

void generate_bitmap()
{
    HDC hdc = GetDC(HWND_DESKTOP);
    HDC hdc_mem = CreateCompatibleDC(hdc);
    HBITMAP hbmp = CreateCompatibleBitmap(hdc, w, h);
    HGDIOBJ bmp_hold = SelectObject(hdc_mem, hbmp);
    BitBlt(hdc_mem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
    SelectObject(hdc_mem, bmp_hold);
    BITMAPINFOHEADER bmi = {0};
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biPlanes = 1;
    bmi.biBitCount = 32;
    bmi.biWidth = w;
    bmi.biHeight = -h;
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage = 4 * w * h;
    if (screen_data)
        free(screen_data);
    screen_data = (BYTE *)malloc(4 * w * h);
    GetDIBits(hdc_mem, hbmp, 0, h, screen_data, (BITMAPINFO *)&bmi, DIB_RGB_COLORS);
    ReleaseDC(GetDesktopWindow(), hdc);
    DeleteDC(hdc_mem);
    DeleteObject(hbmp);
}

inline SRgb get_rgb(int x, int y)
{
    return {screen_data[4 * (y * w + x) + 2], screen_data[4 * (y * w + x) + 1], screen_data[4 * (y * w + x)]};
}

inline int find_color(SRgb p)
{
    return dict[p.r][p.g][p.b];
}

int scan(int x, int y)
{
    return find_color(get_rgb(x, y));
}

int scan_box(int u, int v)
{
    return find_color(get_rgb(tl.x + 16 * u, tl.y + 16 * v));
}

int scan_mine()
{
    int ret = 0;
    for (int i = 0, cur = 100; i < 3; i++, cur /= 10)
        ret += find_color(get_rgb(mn.x + 13 * i, mn.y)) * cur;
    return ret;
}
}

namespace NSolve
{
HWND win;
int w, h;

void init()
{
    std::cout << "Searching for application ...\n";
    do
        win = FindWindow(NULL, TEXT("Minesweeper X"));
    while (win == 0);
    std::cout << "Bringing the application to top ... \n";
    Sleep(500);
    SetForegroundWindow(win);

    //Setting up board sizes, mine count and restart button
    RECT tmp;
    GetWindowRect(win, &tmp);
    tl.x = tmp.left + 22;
    tl.y = tmp.top + 110;
    w = (tmp.right - 19 - tl.x) / 16 + 1;
    h = (tmp.bottom - 19 - tl.y) / 16 + 1;
    std::cout << "Board size found: " << w << " " << h << '\n';

    mn.x = tmp.left + 23;
    mn.y = tmp.top + 72;
    NRead::generate_bitmap();
    std::cout << "Mines found: " << NRead::scan_mine() << '\n';

    res.x = (tmp.left + tmp.right) / 2;
    res.y = tmp.top + 74;
}
}

int main()
{
    NRead::init();
    NSolve::init();
    //solve();
}
