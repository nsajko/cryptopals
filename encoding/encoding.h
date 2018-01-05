#pragma once

#include "../buffer/buffer.h"

extern buffer *hexDecodeAlloc(const buffer *in);
extern buffer *hexEncodeAlloc(const buffer *in);
extern buffer *base64DecodeAlloc(const buffer *in);
extern buffer *base64EncodeAlloc(const buffer *in);
