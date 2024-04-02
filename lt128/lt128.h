#include <common.h>
#include <cstdio>
#include <cstring>
#include <functional>
#include <string>

namespace hn = hwy::HWY_NAMESPACE;

template <typename tag = hn::ScalableTag<uint64_t>>
__attribute__((noinline)) size_t Lt128Highway0(tag, hn::VFromD<tag>,
                                               hn::VFromD<tag>,
                                               hn::MFromD<tag> &);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128Highway(const uint64_t *HWY_RESTRICT a,
                  const uint64_t *HWY_RESTRICT b, uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128HighwayAsm(const uint64_t *HWY_RESTRICT a,
                     const uint64_t *HWY_RESTRICT b, uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128HighwayAsmImmOddEven(const uint64_t *HWY_RESTRICT a,
                               const uint64_t *HWY_RESTRICT b,
                               uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128Manual1(const uint64_t *HWY_RESTRICT a,
                  const uint64_t *HWY_RESTRICT b, uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128Manual1OptimizeForX280(const uint64_t *HWY_RESTRICT a,
                                 const uint64_t *HWY_RESTRICT b,
                                 uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
__attribute__((noinline)) size_t Lt128OptimizeForX280(tag, hn::VFromD<tag>,
                                                      hn::VFromD<tag>,
                                                      hn::MFromD<tag> &);

template <typename tag = hn::ScalableTag<uint64_t>>
__attribute__((noinline)) size_t Lt128OptimizeForX280ChangeVType(
    tag, hn::VFromD<tag>, hn::VFromD<tag>, hn::MFromD<tag> &);

template <typename tag = hn::ScalableTag<uint64_t>>
__attribute__((noinline)) size_t Lt128OptimizeForX280UseShift(
    tag, hn::VFromD<tag>, hn::VFromD<tag>, hn::MFromD<tag> &);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128Manual1OptimizeForP670(const uint64_t *HWY_RESTRICT a,
                                 const uint64_t *HWY_RESTRICT b,
                                 uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128Manual1WithGlobal(const uint64_t *HWY_RESTRICT a,
                            const uint64_t *HWY_RESTRICT b,
                            uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128Manual1Intrinsics(const uint64_t *HWY_RESTRICT a,
                            const uint64_t *HWY_RESTRICT b,
                            uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128Manual2(const uint64_t *HWY_RESTRICT a,
                  const uint64_t *HWY_RESTRICT b, uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128Manual2Intrinsics(const uint64_t *HWY_RESTRICT a,
                            const uint64_t *HWY_RESTRICT b,
                            uint8_t *HWY_RESTRICT r);

void CheckLt128(
    std::function<void(const uint64_t *, const uint64_t *, uint8_t *)> f0,
    std::function<void(const uint64_t *, const uint64_t *, uint8_t *)> f1);

template <typename tag = hn::ScalableTag<uint64_t>>
void CheckLt128Reg(const std::string &name0, const std::string &name1,
                   size_t (*f0)(tag, hn::VFromD<tag>, hn::VFromD<tag>,
                                hn::MFromD<tag> &),
                   size_t (*f1)(tag, hn::VFromD<tag>, hn::VFromD<tag>,
                                hn::MFromD<tag> &),
                   int printCycleIter = 5) {
  auto numElem = hn::Lanes(tag());
  printf("Comparing %s and %s, num_elem = %zu\n", name0.c_str(), name1.c_str(),
         numElem);
  for (int i = 0; i < 100; ++i) {
    uint64_t a[128] = {0, 1};
    uint64_t b[128] = {1, 0};
    uint8_t r0[128];
    uint8_t r1[128];
    memset(r0, 0xcc, sizeof(r0));
    memset(r1, 0xcc, sizeof(r1));
    for (int i = 0; i < numElem; ++i) {
      a[i] = rand() % 100;
      b[i] = rand() % 100;
    }
    tag d;
    auto ar = hn::Load(d, a);
    auto br = hn::Load(d, b);
    hn::MFromD<tag> mr0;
    hn::MFromD<tag> mr1;
    size_t cycle_f0 = f0(d, ar, br, mr0);
    size_t cycle_f1 = f1(d, ar, br, mr1);
    hn::StoreMaskBits(d, mr0, r0);
    hn::StoreMaskBits(d, mr1, r1);
    if (i <= printCycleIter)
      printf("Cycle: %s:%lu, %s:%lu\n", name0.c_str(), cycle_f0, name1.c_str(),
             cycle_f1);

    for (int i = 0; i < numElem; i++) {
      bool mismatch = false;
      if (r0[i] != r1[i]) {
        printf("Mismatch at %d: %d != %d\n", i, r0[i], r1[i]);
        mismatch = true;
      }
      if (mismatch) {
        printf("Raw\ta\tb\tr0\tr1:\n");
        for (int i = 0; i < numElem; ++i) {
          printf("%lu\t%lu\t%u\t%u\t%d\n", a[i], b[i], r0[i / 8], r1[i / 8],
                 i / 8);
        }
        return;
      }
    }
  }
}
