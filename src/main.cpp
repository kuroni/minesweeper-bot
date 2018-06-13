#include "handle.h"
#include "solve.h"

int main()
{
    if (!NHandle::init())
    {
        std::cerr << "Faulty skin!";
        return 0;
    }
    NHandle::new_game();
}