
#ifndef _SIPHASH_H_
#define _SIPHASH_H_

#include <stdint.h>

uint64_t siphash(const uint8_t *in, const size_t inlen, const uint8_t *k);
uint64_t siphash_nocase(const uint8_t *in, const size_t inlen, const uint8_t *k);

#endif  // _SIPHASH_H_
