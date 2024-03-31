#include "lt128.h"
#include "rvv/int.h"
#include "rvv/misc.h"
#include <cstdio>

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

uint64_t bufv11[128];
uint64_t bufv12[128];
uint64_t bufv8[128];
uint64_t bufv9[128];
uint8_t buf0[128];
uint8_t buf1[128];
uint8_t buf2[128];
extern "C" {
uint64_t mask = 0x5555555555555555;
}

template <>
void Lt128Manual1OptimizeForX280<hn::ScalableTag<uint64_t>>(
    const uint64_t *HWY_RESTRICT a, const uint64_t *HWY_RESTRICT b,
    uint8_t *HWY_RESTRICT r) {
  asm volatile(R"(
               vsetvli	a3, zero, e64, m1, ta, ma
               vle64.v	v8, (%[a])
               vle64.v	v9, (%[b])
               # LLVM-MCA-BEGIN Lt128Manual1-X280-lmul1
               lui	a0, 349525
               vslide1down.vx    v11, v8, zero
               addiw	a0, a0, 1365
               slli	a1, a0, 32
               vslide1down.vx    v12, v9, zero
               add	a0, a0, a1
               vmsltu.vv	v8, v8, v9
               vmv.v.x v13, a0
               vmseq.vv	v14, v11, v12
               vmsltu.vv	v10, v11, v12
               vmand.mm	v14, v14, v13
               vmand.mm	v10, v10, v13
               vmand.mm	v8, v14, v8
               vmor.mm	v8, v8, v10
               vadd.vv	v16, v8, v8
               vmor.mm	v16, v16, v8
               # LLVM-MCA-END Lt128Manual1-X280-lmul1
               li	a0, 7
               vsetvli	a1, zero, e8, mf8, ta, ma
               vsm.v	v16, (%[r])
               bltu	a0, a3, final_280
               li	a0, -1
               sllw	a0, a0, a3
               lbu	a1, 0(%[r])
               not	a0, a0
               and	a0, a0, a1
               sb	a0, 0(%[r])
               final_280:
               )"
               :
               : [a] "r"(a), [b] "r"(b), [r] "r"(r), [mask] "r"(mask)
               : "a0", "a1", "a3", "v8", "v9", "v10", "v11", "v12", "v13",
                 "v14", "v16", "memory");
}

template <>
void Lt128Manual1OptimizeForP670<hn::ScalableTag<uint64_t>>(
    const uint64_t *HWY_RESTRICT a, const uint64_t *HWY_RESTRICT b,
    uint8_t *HWY_RESTRICT r) {
  asm volatile("vsetvli	a3, zero, e64, m1, ta, ma\n\t"
               "vle64.v	v8, (%[a])\n\t"
               "vle64.v	v9, (%[b])\n\t"
               "# LLVM-MCA-BEGIN Lt128Manual1-P670-lmul1\n\t"
               "lui	a0, 349525\n\t"
               "vslide1down.vx    v11, v8, zero\n\t"
               "addiw	a0, a0, 1365\n\t"
               "vslide1down.vx    v12, v9, zero\n\t"
               "slli	a1, a0, 32\n\t"
               "vmseq.vv	v14, v11, v12\n\t"
               "add	a0, a0, a1\n\t"
               "vmsltu.vv	v10, v11, v12\n\t"
               "vand.vx	v14, v14, a0\n\t"
               "vmsltu.vv	v8, v8, v9\n\t"
               "vand.vx	v10, v10, a0\n\t"
               "vand.vv	v8, v14, v8\n\t"
               "vor.vv	v8, v8, v10\n\t"
               "vadd.vv	v16, v8, v8\n\t"
               "vor.vv	v16, v16, v8\n\t"
               // "li	a0, 3\n\t"
               // "vmul.vx	v16, v8, a0\n\t"
               "# LLVM-MCA-END Lt128Manual1-P670-lmul1\n\t"
               "li	a0, 7\n\t"
               "vsetvli	a1, zero, e8, mf8, ta, ma\n\t"
               "vsm.v	v16, (%[r])\n\t"
               "bltu	a0, a3, final_670\n\t"
               "li	a0, -1\n\t"
               "sllw	a0, a0, a3\n\t"
               "lbu	a1, 0(%[r])\n\t"
               "not	a0, a0\n\t"
               "and	a0, a0, a1\n\t"
               "sb	a0, 0(%[r])\n\t"
               "final_670:\n\t"
               :
               : [a] "r"(a), [b] "r"(b), [r] "r"(r), [mask] "r"(mask)
               : "a0", "a3", "memory");
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
               : [a] "r"(a), [b] "r"(b), [r] "r"(r), [mask] "r"(mask)
               : "a3", "memory");
}