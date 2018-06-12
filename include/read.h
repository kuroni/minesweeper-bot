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
} * rt, *num, *fac, *rtxp, *numxp, *facxp, *rt98, *num98, *fac98;

void init_xp()
{
}

void init_98()
{
}

int hash(RGBApixel *px)
{
    return ((int)px->Red << 16) | ((int)px->Green << 8) | ((int)px->Blue);
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
    STrie *rt = new STrie(-1, ans.second);
    hsh.clear();
    for (int &v : ve)
        hsh.push_back(hash(bmp(st.x + dis * v + rt->pos.x, st.y + rt->pos.y)));
    sort(hsh.begin(), hsh.end());
    hsh.resize(std::distance(hsh.begin(), std::unique(hsh.begin(), hsh.end())));
    for (int &u : hsh)
    {
        std::vector<int> ne;
        for (int &v : ve)
            if (hash(bmp(16 * v + rt->pos.x, rt->pos.y)) == u)
                ne.push_back(v);
        rt->ve.push_back(std::make_pair(u, construct(ne, bmp, st, h, w, dis)));
        if (rt->ve.back().second == nullptr)
            return nullptr;
    }
    return rt;
}

bool init()
{
    HKEY reg;
    if (RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Brightsoft\\Minesweeper X"), &reg) != ERROR_SUCCESS)
    {
        rt = rtxp;
        num = numxp;
        return;
    }
    char val[128];
    DWORD len = 128;
    RegQueryValueEx(reg, "Skin", NULL, NULL, (LPBYTE)&val, &len);
    if (val[0] == 0)
    {
        rt = rtxp;
        num = numxp;
        return;
    }
    if (val[0] == 1)
    {
        rt = rt98;
        num = num98;
        return;
    }
    RegQueryValueEx(reg, "SkinPath", NULL, NULL, (LPBYTE)&val, &len);
    BMP bmp;
    bmp.ReadFromFile(val);
    if ((rt = construct({0, 1, 2, 3, 4, 5, 6, 7, 8}, bmp, SPosition(0, 0), 16, 16, 16)) == nullptr)
        return false;
    if ((num = construct({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, bmp, SPosition(0, 33), 11, 21, 12)) == nullptr)
        return false;
    if ((fac = construct({0, 2, 3}, bmp, SPosition(0, 55), 26, 26, 27)) == nullptr)
        return false;
    return true;
}

int read(const SPosition &u)
{
}
} // namespace NRead

#endif // READ_H