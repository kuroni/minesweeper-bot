#include "handle.hpp"
#include "solve.hpp"

int main()
{
    // while (true)
    // {
    //     int m, n;
    //     std::cin >> m >> n;
    //     NHandle::click(SPosition(m, n));
    // }
    if (!NHandle::init_skin())
    {
        std::cerr << "Faulty skin!";
        return 0;
    }
    // NHandle::init_game();
}