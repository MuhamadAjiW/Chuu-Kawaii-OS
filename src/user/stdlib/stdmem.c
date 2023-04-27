#include "../lib-header/stdtype.h"
#include "../lib-header/stdmem.h"

int memcmpr(const void *s1, const void *s2, uint32_t n) {
    const uint8_t *buf1 = (const uint8_t*) s1;
    const uint8_t *buf2 = (const uint8_t*) s2;
    for (uint32_t i = 0; i < n; i++) {
        if (buf1[i] < buf2[i]) {
            return -1;
        }
        else if (buf1[i] > buf2[i]){
            return 1;
        }       
    }

    return 0;
}

void* memcopy(void* restrict dest, const void* restrict src, uint32_t n) {
    uint8_t *dstbuf       = (uint8_t*) dest;
    const uint8_t *srcbuf = (const uint8_t*) src;
    for (uint32_t i = 0; i < n; i++)
        dstbuf[i] = srcbuf[i];
    return dstbuf;
}