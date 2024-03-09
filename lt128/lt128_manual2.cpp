#include "lt128.h"
#include "rvv/int.h"
#include "rvv/misc.h"

template <>
void Lt128Manual2Intrinsics<hn::ScalableTag<uint64_t>>(
    const uint64_t *HWY_RESTRICT a, const uint64_t *HWY_RESTRICT b,
    uint8_t *HWY_RESTRICT r) {
  const hn::ScalableTag<uint64_t> d64;
  auto ar = hn::Load(d64, a);
  auto br = hn::Load(d64, b);
  auto vl = rvv::vl_t<64>{hn::Lanes(d64)};
  auto eqm = rvv::vmseq(ar, br, vl);
  auto ltm = rvv::vmsltu(ar, br, vl);
  auto eqr = rvv::vreinterpret<rvv::vreg_t<uint64_t, 64>>(
      rvv::vreinterpret<rvv::vreg_t<uint8_t, 8>>(eqm));
  auto ltr = rvv::vreinterpret<rvv::vreg_t<uint64_t, 64>>(
      rvv::vreinterpret<rvv::vreg_t<uint8_t, 8>>(ltm));
  auto ltrshifted = rvv::vsll(ltr, 1, vl);
  auto ltloeqhi = rvv::vand(ltrshifted, eqr, vl);
  auto good = rvv::vor(ltr, ltloeqhi, vl);
  auto masked = rvv::vand(good, 0xaaaaaaaaaaaaaaaa, vl);
  auto masked_shifted_right = rvv::vsrl(masked, 1, vl);
  auto final = rvv::vor(masked, masked_shifted_right, vl);
  auto final_mask = rvv::vreinterpret<rvv::vmask_t<64>>(
      rvv::vreinterpret<rvv::vreg_t<uint8_t, 8>>(final));
  hn::StoreMaskBits(d64, final_mask, r);
}
template <>
void Lt128Manual2<hn::ScalableTag<uint64_t>>(const uint64_t *HWY_RESTRICT a,
                                             const uint64_t *HWY_RESTRICT b,
                                             uint8_t *HWY_RESTRICT r) {
  asm volatile("vsetvli	a3, zero, e64, m1, ta, ma\n\t"
               "vle64.v	v8, (a0)\n\t"
               "vle64.v	v9, (a1)\n\t"
               "# LLVM-MCA-BEGIN Lt128Manual2-lmul1\n\t"
               "vmsltu.vv	v10, v8, v9\n\t"
               "lui	a0, 349525\n\t"
               "vmseq.vv	v8, v8, v9\n\t"
               "addiw	a0, a0, 1365\n\t"
               "vadd.vv	v9, v10, v10\n\t"
               "slli	a1, a0, 32\n\t"
               "vand.vv	v8, v9, v8\n\t"
               "add	a0, a0, a1\n\t"
               "vor.vv	v8, v10, v8\n\t"
               "slli	a0, a0, 1\n\t"
               "vand.vx	v8, v8, a0\n\t"
               "vsrl.vi	v9, v8, 1\n\t"
               "vor.vv	v16, v8, v9\n\t"
               "# LLVM-MCA-END Lt128Manual2-lmul1\n\t"
               "li	a0, 7\n\t"
               "vsm.v	v16, (a2)\n\t"
               "bltu	a0, a3, final\n\t"
               "li	a0, -1\n\t"
               "sllw	a0, a0, a3\n\t"
               "lbu	a1, 0(a2)\n\t"
               "not	a0, a0\n\t"
               "and	a0, a0, a1\n\t"
               "sb	a0, 0(a2)\n\t"
               "final:\n\t"
               :
               : [a] "r"(a), [b] "r"(b), [r] "r"(r)
               : "memory");
}
