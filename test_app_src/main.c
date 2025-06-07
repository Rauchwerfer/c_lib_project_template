#include "stdlib.h"
#include "stdio.h"

#include <examplec.h>

int main(int argc, char* argv[]) 
{
    int result = examplec_sum(3, 2); 

    printf("3 + 2 = %d", result);

    return EXIT_SUCCESS;
}