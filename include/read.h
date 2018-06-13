#ifndef READ_H
#define READ_H
#include "declare.h"
#include "EasyBMP.h"

namespace NRead
{
struct STrie
{
    int val;
    SPosition pos;
    std::vector<std::pair<int, STrie *>> ve;

    STrie(int _val, SPosition _pos) : val(_val), pos(_pos) {}
} * board, *mine, *face, *boardxp, *minexp, *facexp, *board98, *mine98, *face98;

HWND hwnd;
RECT win;

int hash(RGBApixel *px)
{
    return ((int)px->Red << 16) | ((int)px->Green << 8) | ((int)px->Blue);
}

int read(const SPosition &u)
{
    return 0;
}

void init_xp()
{
}

void init_98()
{
}

STrie *construct(std::vector<int> ve, BMP &bmp, SPosition st, int h, int w, int dis)
{
    if (ve.size() == 1)
        return new STrie(ve.back(), SPosition());
    std::pair<unsigned int, SPosition> ans = std::make_pair(0, SPosition());
    std::vector<int> hsh;
    for (int x = 0; x < h; x++)
        for (int y = 0; y < w; y++)
        {
            hsh.clear();
            for (int &v : ve)
                hsh.push_back(hash(bmp(st.x + dis * v + x, st.y + y)));
            sort(hsh.begin(), hsh.end());
            hsh.resize(std::distance(hsh.begin(), std::unique(hsh.begin(), hsh.end())));
            ans = std::max(ans, std::make_pair(hsh.size(), SPosition(x, y)));
        }
    if (ans.first == 0)
        return nullptr;
    STrie *board = new STrie(-1, ans.second);
    hsh.clear();
    for (int &v : ve)
        hsh.push_back(hash(bmp(st.x + dis * v + board->pos.x, st.y + board->pos.y)));
    sort(hsh.begin(), hsh.end());
    hsh.resize(std::distance(hsh.begin(), std::unique(hsh.begin(), hsh.end())));
    for (int &u : hsh)
    {
        std::vector<int> ne;
        for (int &v : ve)
            if (hash(bmp(st.x + dis * v + board->pos.x, st.y + board->pos.y)) == u)
                ne.push_back(v);
        board->ve.push_back(std::make_pair(u, construct(ne, bmp, st, h, w, dis)));
        if (board->ve.back().second == nullptr)
            return nullptr;
    }
    return board;
}

bool init()
{
    init_xp();
    init_98();
    do
        hwnd = FindWindow(NULL, TEXT("Minesweeper X"));
    while (hwnd == 0);
    SetForegroundWindow(hwnd);
    RECT tmp;
    GetWindowRect(hwnd, &win);
    HKEY reg;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Brightsoft\\Minesweeper X"), 0, KEY_QUERY_VALUE, &reg) != ERROR_SUCCESS)
    {
        board = boardxp;
        mine = minexp;
        face = facexp;
        RegCloseKey(reg);
        return true;
    }
    char val[SZ];
    DWORD len = SZ;
    RegQueryValueEx(reg, "Skin", NULL, NULL, (LPBYTE)&val, &len);
    if (val[0] == 0)
    {
        board = boardxp;
        mine = minexp;
        face = facexp;
        RegCloseKey(reg);
        return true;
    }
    if (val[0] == 1)
    {
        board = board98;
        mine = mine98;
        face = face98;
        RegCloseKey(reg);
        return true;
    }
    std::fill(val, val + SZ, 0);
    len = SZ;
    RegQueryValueEx(reg, "SkinPath", NULL, NULL, (LPBYTE)&val, &len);
    BMP bmp;
    bmp.ReadFromFile(val);
    RegCloseKey(reg);
    if ((board = construct({0, 1, 2, 3, 4, 5, 6, 7, 8}, bmp, SPosition(0, 0), 16, 16, 16)) == nullptr)
        return false;
    if ((mine = construct({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, bmp, SPosition(0, 33), 11, 21, 12)) == nullptr)
        return false;
    if ((face = construct({0, 2, 3}, bmp, SPosition(0, 55), 26, 26, 27)) == nullptr)
        return false;
    return true;
}

void screen_shot()
{
}

void click()
{
}
} // namespace NRead

#endif // READ_H