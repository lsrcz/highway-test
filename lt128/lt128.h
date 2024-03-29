#include <common.h>
#include <functional>

namespace hn = hwy::HWY_NAMESPACE;

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
