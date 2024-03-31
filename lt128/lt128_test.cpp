#include "lt128.h"
#include <cstdio>
#include <string.h>

void CheckLt128(
    std::function<void(const uint64_t *HWY_RESTRICT,
                       const uint64_t *HWY_RESTRICT, uint8_t *HWY_RESTRICT)>
        f0,
    std::function<void(const uint64_t *HWY_RESTRICT,
                       const uint64_t *HWY_RESTRICT, uint8_t *HWY_RESTRICT)>
        f1) {
  for (int i = 0; i < 100; ++i) {
    uint64_t a[16] = {0, 1};
    uint64_t b[16] = {1, 0};
    uint8_t r0[16];
    uint8_t r1[16];
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
        printf("Mismatch at %d: %d != %d\n", i, r0[i], r1[i]);
        mismatch = true;
      }
      if (mismatch) {
        printf("Raw:\n");
        for (int i = 0; i < __riscv_vlenb() / 8; ++i) {
          printf("%lu\t%lu\t%u\t%u\n", a[i], b[i], r0[i], r1[i]);
        }
        return;
      }
    }
  }
}