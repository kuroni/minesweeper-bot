#include "handle.h"
#include "solve.h"

int main()
{
    if (!NHandle::init_skin())
    {
        std::cerr << "Faulty skin!";
        return 0;
    }
    // NHandle::init_game();
}