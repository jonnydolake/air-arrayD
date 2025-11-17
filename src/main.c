#include <stdio.h>
#include <stdlib.h>
#define AIR_ARRAYD_IMPLEMENTATION
#include "air_arrayd.h"

int main(void)
{
    stringD test = initD(test, "tes");
    stringD test2 = initD(test2, "ting");
    // float arr1[] = {2,4,5,67,8};
    stringD test_float = initD(test_float, 65,66,67,68,99,0);
    stringD test_float2 = initD(test_float, 76,88,71);
    // makeD(test_float, 4,5,6,7);
    appendD(test, 't');
    printD(test);
    popD(test, -1);
    // s_append(&test, "test2\n");
    printD(test);
    concatD(test, test2);

    char bar[] = {"hello"};
    printf("%s\n", bar);
    printD(test);
    printD(test_float);
    popD(test_float, -1);
    printD(test_float);
    concatD(test_float, test_float2);
    printD(test_float);

    return 0;
}
