#include "lt128.h"

template <>
void Lt128HighwayAsm<hn::ScalableTag<uint64_t>>(const uint64_t *HWY_RESTRICT a,
                                                const uint64_t *HWY_RESTRICT b,
                                                uint8_t *HWY_RESTRICT r) {
  asm volatile("vsetvli	a3, zero, e64, m1, ta, mu\n\t"
               "vle64.v	v8, (%[a])\n\t"
               "vle64.v	v9, (%[b])\n\t"
               "# LLVM-MCA-BEGIN Lt128HighwayAsm-lmul1\n\t"
               "vmsltu.vv	v0, v8, v9\n\t"
               "vmv.v.i	v12, 0\n\t"
               "vid.v	v13\n\t"
               "vmerge.vim	v14, v12, -1, v0\n\t"
               "vmseq.vv	v0, v8, v9\n\t"
               "vslide1up.vx	v11, v14, zero\n\t"
               "vmerge.vim	v10, v12, -1, v0\n\t"
               "vand.vi	v13, v13, 1\n\t"
               "vand.vv	v10, v10, v11\n\t"
               "vmseq.vi	v0, v13, 0\n\t"
               "vor.vv	v14, v14, v10\n\t"
               "vslide1down.vx	v14, v14, zero, v0.t\n\t"
               "vmsne.vi	v14, v14, 0\n\t"
               "# LLVM-MCA-END Lt128HighwayAsm-lmul1\n\t"
               "li	a0, 7\n\t"
               "vsm.v	v14, (%[r])\n\t"
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
