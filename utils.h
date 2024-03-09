#pragma once

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

void string_copy(char** dst, size_t* dst_bytes, const char* src, int src_bytes);
