#ifndef COLOR_READING_H_INCLUDED
#define COLOR_READING_H_INCLUDED

#include "global_var.h"

/// PREP FUNCTIONS

void init_color_dict()
{
    memset(color_dict, -1, sizeof(color_dict));
    for (int i = 0; i < 12; i++)
        color_dict[color_code[i][0]][color_code[i][1]][color_code[i][2]] = i;
}

void init_desktop_cap()
{
    RECT desktop;
    CONST HWND hdesktop = GetDesktopWindow();
    GetWindowRect(hdesktop, &desktop);
    cap_x = 0;
    cap_y = 0;
    cap_w = 2806; //desktop.right;
    cap_h = 900; //desktop.bottom;
}

void color_reading_init_master()
{
    init_color_dict();
    init_desktop_cap();
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
    if(screen_data) free(screen_data);
    screen_data = (BYTE*)malloc(4 * cap_w * cap_h);
    GetDIBits(hdc_mem, hbmp, 0, cap_h, screen_data, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
    ReleaseDC(GetDesktopWindow(),hdc);
    DeleteDC(hdc_mem);
    DeleteObject(hbmp);
}

inline rgb_t get_rgb(int x, int y)
{
    return
    {
        screen_data[4 * (y * cap_w + x) + 2],
        screen_data[4 * (y * cap_w + x) + 1],
        screen_data[4 * (y * cap_w + x)]
    };
}

/// PROCESSING FUNCTIONS

inline int find_color(rgb_t p)
{
    return color_dict[p.r][p.g][p.b];
}

int scan_box(int u, int v) // scan_grid(0, 0) gives the color code for the first box of the grid
{
    int x = origin.x + (16 * u),
        y = origin.y + (16 * v);
    rgb_t pixel = get_rgb(x, y);
    return find_color(pixel);
}

int custom_scan(int x, int y)
{
    rgb_t pixel = get_rgb(x, y);
    return find_color(pixel);
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
                if (grid[cur.u][cur.v] == 11) return 0;
                cout << grid[cur.u][cur.v] << ' ';
                for (int i = 0; i < 4; i++)
                {
                    ind tar = {cur.u + dx[i], cur.v + dy[i]};
                    if (tar.u < 0 || tar.u >= grid_width || tar.v < 0 || tar.v >= grid_height) continue;
                    else if (grid[tar.u][tar.v] == 9)
                        q.push(tar);
                }
            }
        }
        q.pop();
    }
    return 1;
}

#endif // COLOR_READING_H_INCLUDED
