#include "color_reading.h"
#include "global_var.h"
#include "get_msx_info.h"
#include "solve.h"
#include "click.h";

int main()
{
    srand(time(NULL));
    color_reading_init_master();
    init_grid();
    generate_bitmap();
    //solve();
}
