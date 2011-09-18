#include "libpixel/os/definitions.h"

char* itoa (int value, char* str, int base)
{
    sprintf(str, "%d", value);
    return str;
}