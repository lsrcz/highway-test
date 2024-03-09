#include <common.h>
#include <functional>

namespace hn = hwy::HWY_NAMESPACE;

template <typename tag = hn::ScalableTag<uint64_t>>
void Min128Highway(const uint64_t *HWY_RESTRICT a,
                   const uint64_t *HWY_RESTRICT b, uint64_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Min128HighwayAsm(const uint64_t *HWY_RESTRICT a,
                      const uint64_t *HWY_RESTRICT b, uint64_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Min128HighwayAsmImmOddEven(const uint64_t *HWY_RESTRICT a,
                                const uint64_t *HWY_RESTRICT b,
                                uint64_t *HWY_RESTRICT r);

void CheckMin128(
    std::function<void(const uint64_t *, const uint64_t *, uint64_t *)> f0,
    std::function<void(const uint64_t *, const uint64_t *, uint64_t *)> f1);
