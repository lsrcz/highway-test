#include "min128.h"

template <typename tag>
void Min128Highway(const uint64_t *HWY_RESTRICT a,
                   const uint64_t *HWY_RESTRICT b, uint64_t *HWY_RESTRICT r) {
  const tag d64;
  auto ar = hn::Load(d64, a);
  auto br = hn::Load(d64, b);
  auto rr = hn::Min128(d64, ar, br);
  hn::Store(rr, d64, r);
}

template void
Min128Highway<hn::ScalableTag<uint64_t>>(const uint64_t *HWY_RESTRICT a,
                                         const uint64_t *HWY_RESTRICT b,
                                         uint64_t *HWY_RESTRICT r);

template void Min128Highway<hn::Twice<hn::Twice<hn::ScalableTag<uint64_t>>>>(
    const uint64_t *HWY_RESTRICT a, const uint64_t *HWY_RESTRICT b,
    uint64_t *HWY_RESTRICT r);

template <>
void Min128HighwayAsm<hn::ScalableTag<uint64_t>>(const uint64_t *HWY_RESTRICT a,
                                                 const uint64_t *HWY_RESTRICT b,
                                                 uint64_t *HWY_RESTRICT r) {
  asm volatile("vsetvli	a3, zero, e64, m1, ta, ma\n\t"
               "vle64.v	v12, (%[a])\n\t"
               "vle64.v	v16, (%[b])\n\t"
               "# LLVM-MCA-BEGIN highway-scheduled\n\t"
               "vslide1down.vx	v20, v12, zero\n\t"
               "vslide1down.vx	v24, v16, zero\n\t"
               "vid.v	v4\n\t"
               "vminu.vv	v28, v12, v16\n\t"
               "vand.vi	v4, v4, 1\n\t"
               "vmsltu.vv	v0, v20, v24\n\t"
               "vmerge.vvm	v12, v16, v12, v0\n\t"
               "vmseq.vv	v0, v20, v24\n\t"
               "vmerge.vvm	v12, v12, v28, v0\n\t"
               "vmseq.vi	v0, v4, 0\n\t"
               "vmerge.vvm	v8, v28, v12, v0\n\t"
               "# LLVM-MCA-END highway-scheduled\n\t"
               "vse64.v	v8, (%[r])\n\t"
               :
               : [a] "r"(a), [b] "r"(b), [r] "r"(r)
               : "memory");
}
