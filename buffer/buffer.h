#pragma once

#include <stddef.h> // size_t

typedef struct {
	unsigned char *b;
	size_t l;
} buffer;

buffer *bufferAlloc(size_t l);
void bufferFree(buffer *buf);
