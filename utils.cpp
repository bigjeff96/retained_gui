#include "utils.h"

void string_copy(char** dst, size_t* dst_bytes, const char* src, int src_bytes) {
    if ( src_bytes == 0 ) src_bytes = strlen(src);
    
    *dst = (char*)realloc(*dst, src_bytes);
    memcpy(*dst, src, src_bytes);
    *dst_bytes = src_bytes;
}
