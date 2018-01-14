#pragma once

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// The following should probably always be used instead of using functions they wrap
// directly, except for the functions in <string.h>, which may legitimately need to be used
// differently.

// <stdio.h> fread fwrite
#define FREAD(B, N, F) fread((B), sizeof((B)[0]), (N), (F))
#define FWRITE(B, N, F) fwrite((B), sizeof((B)[0]), (N), (F))

// <stdlib.h> calloc malloc realloc bsearch qsort
#define MALLOC(P, N) (P) = malloc((N) * sizeof(*(P)))
#define QSORT(B, N, C) qsort((B), (N), sizeof((B)[0]), (C));

// <string.h> memcpy memmove memcmp memchr memset
// ...

// <unistd.h> pread pwrite read write
// ...
