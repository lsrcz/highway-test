#include "lt128.h"

template <typename tag>
__attribute__((noinline)) size_t Lt128Highway0(tag d, hn::VFromD<tag> ar,
                                               hn::VFromD<tag> br,
                                               hn::MFromD<tag> &m) {
  size_t begin, end;
  GET_CYCLE(("+vr"(ar), "+vr"(br)), (), begin);
  LLVM_MCA_BEGIN("Lt128Highway0", ("+vr"(ar), "+vr"(br)), (), decltype(ar));
  auto rr = hn::Lt128(d, ar, br);
  LLVM_MCA_END("Lt128Highway0", ("+vr"(rr)), (), decltype(ar));
  GET_CYCLE(("+vr"(rr)), (), end);
  m = rr;
  return end - begin;
}

template __attribute__((noinline)) size_t
Lt128Highway0<hn::ScalableTag<uint64_t>>(hn::ScalableTag<uint64_t> d,
                                         const rvv::vreg_t<uint64_t, 64> ar,
                                         const rvv::vreg_t<uint64_t, 64> br,
                                         rvv::vmask_t<64> &m);

template __attribute__((noinline)) size_t
Lt128Highway0<hn::Twice<hn::ScalableTag<uint64_t>>>(
    hn::Twice<hn::ScalableTag<uint64_t>> d, const rvv::vreg_t<uint64_t, 32> ar,
    const rvv::vreg_t<uint64_t, 32> br, rvv::vmask_t<32> &m);

template __attribute__((noinline)) size_t
Lt128Highway0<hn::Twice<hn::Twice<hn::ScalableTag<uint64_t>>>>(
    hn::Twice<hn::Twice<hn::ScalableTag<uint64_t>>> d,
    rvv::vreg_t<uint64_t, 16> ar, rvv::vreg_t<uint64_t, 16> br,
    rvv::vmask_t<16> &m);

template __attribute__((noinline)) size_t
Lt128Highway0<hn::Twice<hn::Twice<hn::Twice<hn::ScalableTag<uint64_t>>>>>(
    hn::Twice<hn::Twice<hn::Twice<hn::ScalableTag<uint64_t>>>> d,
    rvv::vreg_t<uint64_t, 8> ar, rvv::vreg_t<uint64_t, 8> br,
    rvv::vmask_t<8> &m);

template <typename tag>
void Lt128Highway(const uint64_t *HWY_RESTRICT a,
                  const uint64_t *HWY_RESTRICT b, uint8_t *HWY_RESTRICT r) {
  const tag d64;
  auto ar = hn::Load(d64, a);
  auto br = hn::Load(d64, b);
  LLVM_MCA_BEGIN("Lt128Highway", ("+vr"(ar)), (), decltype(ar));
  auto rr = hn::Lt128(d64, ar, br);
  LLVM_MCA_END("Lt128Highway", ("+vr"(rr)), (), decltype(ar));
  hn::StoreMaskBits(d64, rr, r);
}

template void
Lt128Highway<hn::ScalableTag<uint64_t>>(const uint64_t *HWY_RESTRICT a,
                                        const uint64_t *HWY_RESTRICT b,
                                        uint8_t *HWY_RESTRICT r);

template void Lt128Highway<hn::Twice<hn::ScalableTag<uint64_t>>>(
    const uint64_t *HWY_RESTRICT a, const uint64_t *HWY_RESTRICT b,
    uint8_t *HWY_RESTRICT r);

template void Lt128Highway<hn::Twice<hn::Twice<hn::ScalableTag<uint64_t>>>>(
    const uint64_t *HWY_RESTRICT a, const uint64_t *HWY_RESTRICT b,
    uint8_t *HWY_RESTRICT r);
