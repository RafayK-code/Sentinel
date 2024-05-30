#include "Dbg.h"

int main()
{
    int x = 23;
    int y = 0;
    DBG_INFO("This var x = {1} is cool, and this y = {0} is even cooler", y, x);

    return 0;
}