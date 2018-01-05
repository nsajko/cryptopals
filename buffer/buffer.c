#include <stdlib.h> // malloc free

#include "buffer.h"
#include "../sizeofMacros.h"

buffer *
bufferAlloc(size_t l) {
	buffer *buf;
	unsigned char *b;
	MALLOC(b, l);
	if (b == NULL) {
		return NULL;
	}
	MALLOC(buf, 1);
	if (buf == NULL) {
		free(b);
		return NULL;
	}
	buf->b = b;
	return buf;
}

void
bufferFree(buffer *buf) {
	free(buf->b);
	free(buf);
}
