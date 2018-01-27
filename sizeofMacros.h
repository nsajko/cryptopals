#pragma once

// Macros for avoiding direct usage of sizeof in user code, which comes together with
// avoiding hardcoding of types as arguments to sizeof.

// Parameter A must be an array.
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// The following macro wrappers should almost always be used instead of using directly the
// functions they wrap; except for the functions in <string.h>, which will need to be used
// differently in some cases.
//
// Macros wrapping functions with void return value include the ending ';'.
//
// Parameters named B, D, S, A, or B can be arrays or pointers
// Parameters named P must be pointers.
// Parameters named N must be of an integer type.
//
// The sizeof operator is applied to the first element of the array, or to the object the
// pointer points to. Thus the member width is obtained; and the N parameter is used for
// passing the number of members, array length in other words. In this fashion the size of
// a contiguous memory range is specified with the user providing only a typed identifier
// and a number of elements.

// <stdio.h> fread fwrite

#define FREAD(B, N, F) fread((B), sizeof((B)[0]), (N), (F))
#define FWRITE(B, N, F) fwrite((B), sizeof((B)[0]), (N), (F))

// <stdlib.h> calloc malloc realloc bsearch qsort

#define CALLOC(P, N) (P) = calloc((N), sizeof(*(P)))
#define MALLOC(P, N) (P) = malloc((N) * sizeof(*(P)))
#define REALLOC(P, N) realloc((P), (N) * sizeof(*(P)))

#define BSEARCH(K, B, N, C) bsearch((K), (B), (N), sizeof((B)[0]), (C))
#define QSORT(B, N, C) qsort((B), (N), sizeof((B)[0]), (C));

// <string.h> memcpy memmove memcmp memchr memset

#define MEMCPY(D, S, N) memcpy((D), (S), sizeof((D)[0]) * (N));
#define MEMMOVE(D, S, N) memmove((D), (S), sizeof((D)[0]) * (N));

#define MEMCMP(A, B, N) memcmp((A), (B), sizeof((A)[0]) * (N))

#define MEMCHR(A, C, N) memchr((A), (C), sizeof((A)[0]) * (N))

#define MEMSET(A, C, N) memset((A), (C), sizeof((A)[0]) * (N));

// <unistd.h> pread pwrite read write

#define PREAD(F, B, N, O) pread((F), (B), sizeof((B)[0]) * (N), (O))
#define PWRITE(F, B, N, O) pwrite((F), (B), sizeof((B)[0]) * (N), (O))
#define READ(F, B, N) read((F), (B), sizeof((B)[0]) * (N))
#define WRITE(F, B, N) write((F), (B), sizeof((B)[0]) * (N))
