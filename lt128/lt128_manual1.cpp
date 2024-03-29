#include "lt128.h"
#include "rvv/int.h"
#include "rvv/misc.h"

// This one seems to be the best as it maximizes pipelining and minimizes the
// number of instructions
template <>
void Lt128Manual1Intrinsics<hn::ScalableTag<uint64_t>>(
    const uint64_t *HWY_RESTRICT a, const uint64_t *HWY_RESTRICT b,
    uint8_t *HWY_RESTRICT r) {
  const hn::ScalableTag<uint64_t> d64;
  auto ar = hn::Load(d64, a);
  auto br = hn::Load(d64, b);
  auto vl = rvv::vl_t<64>{hn::Lanes(d64)};
  asm volatile("# LLVM-MCA-BEGIN TryLt128Manual1Intrinsics" ::: "memory");
  auto eqm = rvv::vmseq(ar, br, vl);
  auto ltm = rvv::vmsltu(ar, br, vl);
  auto eqr = rvv::vreinterpret<rvv::vreg_t<uint64_t, 64>>(
      rvv::vreinterpret<rvv::vreg_t<uint8_t, 8>>(eqm));
  auto ltr = rvv::vreinterpret<rvv::vreg_t<uint64_t, 64>>(
      rvv::vreinterpret<rvv::vreg_t<uint8_t, 8>>(ltm));
  auto eqrshifted = rvv::vsrl(eqr, 1, vl);
  auto ltrshifted = rvv::vsrl(ltr, 1, vl);
  auto eqrshiftedmasked = rvv::vand(eqrshifted, 0x5555555555555555, vl);
  auto ltrshiftedmasked = rvv::vand(ltrshifted, 0x5555555555555555, vl);
  auto eqhiltlo = rvv::vand(eqrshiftedmasked, ltr, vl);
  auto good = rvv::vor(eqhiltlo, ltrshiftedmasked, vl);
  auto final = rvv::vmul(good, 3, vl);
  auto final_mask = rvv::vreinterpret<rvv::vmask_t<64>>(
      rvv::vreinterpret<rvv::vreg_t<uint8_t, 8>>(final));
  asm volatile("# LLVM-MCA-END TryLt128Manual1Intrinsics" ::: "memory");

  hn::StoreMaskBits(d64, final_mask, r);
}

template <>
void Lt128Manual1<hn::ScalableTag<uint64_t>>(const uint64_t *HWY_RESTRICT a,
                                             const uint64_t *HWY_RESTRICT b,
                                             uint8_t *HWY_RESTRICT r) {
  asm volatile("vsetvli	a3, zero, e64, m1, ta, ma\n\t"
               "vle64.v	v8, (%[a])\n\t"
               "vle64.v	v9, (%[b])\n\t"
               "# LLVM-MCA-BEGIN Lt128Manual1-lmul1\n\t"
               "vmseq.vv	v10, v8, v9\n\t"
               "lui	a0, 349525\n\t"
               "vmsltu.vv	v8, v8, v9\n\t"
               "addiw	a0, a0, 1365\n\t"
               "slli	a1, a0, 32\n\t"
               "vsrl.vi	v9, v10, 1\n\t"
               "add	a0, a0, a1\n\t"
               "vsrl.vi	v10, v8, 1\n\t"
               "vand.vx	v9, v9, a0\n\t"
               "vand.vx	v10, v10, a0\n\t"
               "vand.vv	v8, v9, v8\n\t"
               "vor.vv	v8, v8, v10\n\t"
               "li	a0, 3\n\t"
               "vmul.vx	v16, v8, a0\n\t"
               "# LLVM-MCA-END Lt128Manual1-lmul1\n\t"
               "li	a0, 7\n\t"
               "vsetvli	a1, zero, e8, mf8, ta, ma\n\t"
               "vsm.v	v16, (%[r])\n\t"
               "bltu	a0, a3, final\n\t"
               "li	a0, -1\n\t"
               "sllw	a0, a0, a3\n\t"
               "lbu	a1, 0(%[r])\n\t"
               "not	a0, a0\n\t"
               "and	a0, a0, a1\n\t"
               "sb	a0, 0(%[r])\n\t"
               "final:\n\t"
               :
               : [a] "r"(a), [b] "r"(b), [r] "r"(r)
               : "memory");
}

extern "C" {
uint64_t mask = 0x5555555555555555;
}

template <>
void Lt128Manual1WithGlobal<hn::ScalableTag<uint64_t>>(
    const uint64_t *HWY_RESTRICT a, const uint64_t *HWY_RESTRICT b,
    uint8_t *HWY_RESTRICT r) {
  asm volatile("vsetvli	a3, zero, e64, m1, ta, ma\n\t"
               "vle64.v	v8, (%[a])\n\t"
               "vle64.v	v9, (%[b])\n\t"
               "# LLVM-MCA-BEGIN Lt128Manual1WithGlobal-lmul1\n\t"
               "vmseq.vv	v10, v8, v9\n\t"
               ".Lpcrel_hi0:\n\t"
               "auipc       a0, %%pcrel_hi(mask)\n\t"
               "vmsltu.vv	v8, v8, v9\n\t"
               // "ld a0, mask\n\t"
               // "lui	a0, 349525\n\t"
               "ld a0, %%pcrel_lo(.Lpcrel_hi0)(a0)\n\t"
               // "addiw	a0, a0, 1365\n\t"
               // "slli	a1, a0, 32\n\t"
               "vsrl.vi	v9, v10, 1\n\t"
               "vsrl.vi	v10, v8, 1\n\t"
               "vand.vx	v9, v9, a0\n\t"
               "vand.vx	v10, v10, a0\n\t"
               "vand.vv	v8, v9, v8\n\t"
               "vor.vv	v8, v8, v10\n\t"
               "li	a0, 3\n\t"
               "vmul.vx	v16, v8, a0\n\t"
               "# LLVM-MCA-END Lt128Manual1WithGlobal-lmul1\n\t"
               "li	a0, 7\n\t"
               "vsetvli	a1, zero, e8, mf8, ta, ma\n\t"
               "vsm.v	v16, (%[r])\n\t"
               "bltu	a0, a3, finalWithGlobal\n\t"
               "li	a0, -1\n\t"
               "sllw	a0, a0, a3\n\t"
               "lbu	a1, 0(%[r])\n\t"
               "not	a0, a0\n\t"
               "and	a0, a0, a1\n\t"
               "sb	a0, 0(%[r])\n\t"
               "finalWithGlobal:\n\t"
               :
               : [a] "r"(a), [b] "r"(b), [r] "r"(r), [mask] "r" (mask)
               : "a3", "memory");
}