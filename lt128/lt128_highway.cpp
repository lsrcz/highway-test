#include "lt128.h"

template <typename tag>
void Lt128Highway(const uint64_t *HWY_RESTRICT a,
                  const uint64_t *HWY_RESTRICT b, uint8_t *HWY_RESTRICT r) {
  const tag d64;
  auto ar = hn::Load(d64, a);
  auto br = hn::Load(d64, b);
  LLVM_MCA_BEGIN("Lt128Highway", decltype(ar));
  auto rr = hn::Lt128(d64, ar, br);
  LLVM_MCA_END("Lt128Highway", decltype(ar));
  hn::StoreMaskBits(d64, rr, r);
}

template void
Lt128Highway<hn::ScalableTag<uint64_t>>(const uint64_t *HWY_RESTRICT a,
                                        const uint64_t *HWY_RESTRICT b,
                                        uint8_t *HWY_RESTRICT r);

template void Lt128Highway<hn::Twice<hn::Twice<hn::ScalableTag<uint64_t>>>>(
    const uint64_t *HWY_RESTRICT a, const uint64_t *HWY_RESTRICT b,
    uint8_t *HWY_RESTRICT r);
