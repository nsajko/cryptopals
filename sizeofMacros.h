#pragma once

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// <stdio.h> fread fwrite
// ...

// <stdlib.h> calloc malloc realloc bsearch qsort
#define MALLOC(P, N) (P) = malloc((N) * sizeof(*(P)))

// <string.h> memcpy memmove memcmp memchr memset
// ...

// <unistd.h> pread pwrite read write
// ...
