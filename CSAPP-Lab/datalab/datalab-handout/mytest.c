#include <stdio.h>

#define INTMIN    (0x80000000)
#define toBool(x) (!(!(x)))

int conditional(int x, int y, int z)
{
    int x_is_0 = !x;
    int y_mask = x_is_0 - 1;
    int z_mask = ~y_mask;

    int ret = ((y & y_mask) | (z & z_mask));
    return ret;
}

inline static int isNegative(int x)
{
    return toBool(x & INTMIN);
}

int main()
{
    int x = 0;
    scanf("%d", &x);

    printf("x = %x\r\n", x);

    return 0;
}