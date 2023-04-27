#ifndef _STDMEMUSER_H
#define _STDMEMUSER_H

#include "stdtype.h"

/**
 * C standard memcmp, check man memcmp or
 * https://man7.org/linux/man-pages/man3/memcmp.3.html for more details
 * 
 * @param s1 Pointer to first memory area
 * @param s2 Pointer to second memory area
 * @param n Memory area size in byte 
 * 
 * @return Integer as error code, zero for equality, non-zero for inequality
*/
int memcmpr(const void *s1, const void *s2, uint32_t n);


#endif