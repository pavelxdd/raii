# RAII

RAII is a single header wrapper around compiler `cleanup` attribute.

For GCC it uses nested functions for cleanup handlers.
For Clang it uses Blocks language extension.

````bash
cmake . -Bbuild -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build build --target install
````

## Example

````c
#include <stdlib.h>
#include <stdio.h>
#include <raii.h>

static void cleanup_x(int *x)
{
    printf("cleanup x %p %d\n", x, *x);
    free(x);
}

static void cleanup_y(void *y)
{
    printf("cleanup y %p\n", y);
    free(y);
}

int main(void)
{
    {
        // cleanup_x is called when x goes out of scope.
        RAII(int *, x, cleanup_x, malloc(sizeof(int)));
        *x = 123;
        printf("init x %p %d\n", x, *x);
    }
    {
        // unlike standard `cleanup` attribute,
        // RAII can use any argument type in cleanup handler.
        RAII(int *, y, cleanup_y, malloc(sizeof(int)));
        *y = 456;
        printf("init y %p %d\n", y, *y);
    }
    {
        // you can use `free()` as a cleanup handler directly.
        RAII(int *, z, free, malloc(sizeof(int)));
        *z = 789;
        printf("init z %p %d\n", z, *z);
    }
    printf("done\n");
    return 0;
}

// init x 0x1b3f2a0 123
// cleanup x 0x1b3f2a0 123
// init y 0x1b3f2a0 456
// cleanup y 0x1b3f2a0
// init z 0x1b3f2a0 789
// done
````
