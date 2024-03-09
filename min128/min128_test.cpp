#include "min128.h"
#include <cstdio>
#include <string.h>

void CheckMin128(
    std::function<void(const uint64_t *HWY_RESTRICT,
                       const uint64_t *HWY_RESTRICT, uint64_t *HWY_RESTRICT)>
        f0,
    std::function<void(const uint64_t *HWY_RESTRICT,
                       const uint64_t *HWY_RESTRICT, uint64_t *HWY_RESTRICT)>
        f1) {
  uint64_t a[16] = {0, 1};
  uint64_t b[16] = {1, 0};
  uint64_t r0[16];
  uint64_t r1[16];
  for (int i = 0; i < 10; ++i) {
    memset(r0, 0xcc, sizeof(r0));
    memset(r1, 0xcc, sizeof(r1));
    for (int i = 0; i < 16; ++i) {
      a[i] = rand() % 100;
      b[i] = rand() % 100;
    }
  }
  f0(a, b, r0);
  f1(a, b, r1);
  for (int i = 0; i < 16; i++) {
    bool mismatch = false;
    if (r0[i] != r1[i]) {
      printf("Mismatch at %d: %ld != %ld\n", i, r0[i], r1[i]);
      mismatch = true;
    }
    if (mismatch) {
      printf("Raw:\n");
      for (int i = 0; i < 16; ++i) {
        printf("%lu\t%lu\n", a[i], b[i]);
      }
    }
  }
}
