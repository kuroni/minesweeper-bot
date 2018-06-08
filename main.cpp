#include <iostream>
#include <cstdlib>
#include <queue>
#include <windows.h>

HWND win;
RECT res;
POINT origin, restart_button, mine_number, marker;
int grid_width, grid_height, mines;
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

void restart_game()
{
    SetCursorPos(restart_button.x, restart_button.y);
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
struct SRgb
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
}

void generate_bitmap()
{
    hdc = GetDC(HWND_DESKTOP);
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
    return
    {
        screen_data[4 * (y * w + x) + 2],
        screen_data[4 * (y * w + x) + 1],
        screen_data[4 * (y * w + x)]
    };
}

inline int find_color(SRgb p)
{
    return dict[p.r][p.g][p.b];
}

int scan_box(int u, int v)
{
    int x = origin.x + (16 * u),
        y = origin.y + (16 * v);
    SRgb pixel = get_rgb(x, y);
    return find_color(pixel);
}

int scan(int x, int y)
{
    SRgb pixel = get_rgb(x, y);
    std::cerr << pixel.r << " " << pixel.g << " " << pixel.b << '\n';
    return find_color(pixel);
}
}

namespace NSolve
{

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
    GetWindowRect(win, &res);
    std::cout << res.left << " " << res.right << " " << res.top << " " << res.bottom << '\n';
    origin.x = res.left + 22;
    origin.y = res.top + 110;
    w = (res.right - 19 - origin.x) / 16 + 1;
    h = (res.bottom - 19 - origin.y) / 16 + 1;
    std::cout << "Board size found: " << grid_width << " " << grid_height << '\n';
    mine_number.x = res.left + 23;
    mine_number.y = res.top + 72;
    NRead::generate_bitmap();
    mines = NRead::scan(mine_number.x, mine_number.y) * 100
          + NRead::scan(mine_number.x + 13, mine_number.y) * 10
          + NRead::scan(mine_number.x + 26, mine_number.y);
    std::cout << "Mines found: " << mines << '\n';
    restart_button.x = (res.left + res.right) / 2;
    restart_button.y = res.top + 74;
}
}

int main()
{
    NRead::init();
    NSolve::init();
    //solve();
}
