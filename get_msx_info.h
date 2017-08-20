#ifndef GET_MSX_INFO_H_INCLUDED
#define GET_MSX_INFO_H_INCLUDED

#include "global_var.h"
#include "color_reading.h"

void init_grid()
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
}

#endif // GET_MSX_INFO_H_INCLUDED
