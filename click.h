#ifndef CLICK_H_INCLUDED
#define CLICK_H_INCLUDED
#include "global_var.h"

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
    ::SendInput(1,&Input,sizeof(INPUT));
    ::ZeroMemory(&Input,sizeof(INPUT));
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    ::SendInput(1,&Input,sizeof(INPUT));
}

inline void right_click(int x, int y)
{
    move_to(x, y);
    INPUT Input = {0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    ::SendInput(1,&Input,sizeof(INPUT));
    ::ZeroMemory(&Input,sizeof(INPUT));
    Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    ::SendInput(1,&Input,sizeof(INPUT));
}

void restart_game()
{
    SetCursorPos(restart_button.x, restart_button.y);
    INPUT Input = {0};
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    ::SendInput(1,&Input,sizeof(INPUT));
    ::ZeroMemory(&Input,sizeof(INPUT));
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    ::SendInput(1,&Input,sizeof(INPUT));
}

#endif // CLICK_H_INCLUDED
