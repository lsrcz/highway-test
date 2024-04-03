#include "../lt128.h"
#include "common.h"

template <typename Vec, bool kReplicate>
__attribute__((noinline)) size_t Lt128OptimizeForX280UseShift(Vec a, Vec b,
                                                              MFromV<Vec> &m) {
  register size_t begin asm("t0"), end asm("t1");
  register size_t vl asm("t2");
  register Vec ar asm("v16") = a;
  register Vec br asm("v24") = b;
  register MFromV<Vec> r asm("v8"), sltu asm("v8"), seq asm("v9");
  register uint64_t t3 asm("t3"), t4 asm("t4");
  GET_CYCLE(("+vr"(ar), "+vr"(br)), (), begin);
  LLVM_MCA_BEGIN_WITH_REP(kReplicate, "Lt128OptimizeForX280UseShift",
                          ("+vr"(ar), "+vr"(br)), (), decltype(ar));

  REPEAT_DEFAULT(
      kReplicate,
      SET_VL_MAX_TA_MA(("+vr"(ar), "+vr"(br)), (), vl, decltype(ar));
      asm volatile(
          R"(
               vmseq.vv %[seq], %[ar], %[br]
               lui	%[t3], 349525
               vmsltu.vv %[sltu], %[ar], %[br]
               addiw	%[t3], %[t3], 1365
               slli	%[t4], %[t3], 32
               )"
          : [sltu] "=vr"(sltu), [seq] "=vr"(seq), [t3] "=r"(t3), [t4] "=r"(t4)
          : [ar] "vr"(ar), [br] "vr"(br)
          : "memory");
      SET_VL_MAX_TA_MA(("+vr"(sltu)), (), vl, vuint8mf2_t); asm volatile(
          R"(
               add	%[t3], %[t3], %[t4]
               vsrl.vi    v0, %[seq], 1
               vmv.v.x v24, %[t3]
               vsrl.vi   v1, %[sltu], 1
               vand.vv	v17, v0, %[sltu]
               vor.vv	v16, v17, v1
               vand.vv	v16, v16, v24
               vadd.vv	v24, v16, v16
               vor.vv	%[r], v24, v16
               )"
          : [r] "=vr"(r)
          : [sltu] "vr"(sltu), [seq] "vr"(seq), [t3] "r"(t3), [t4] "r"(t4)
          : "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31", "memory");
      asm volatile(""
                   : [ar] "=vr"(ar), [br] "=vr"(br)::"memory"););
  LLVM_MCA_END_WITH_REP(kReplicate, "Lt128OptimizeForX280UseShift", ("+vr"(r)),
                        (), decltype(ar));
  GET_CYCLE(("+vr"(r)), (), end);
  m = r;
  return end - begin;
}

#define INSTANTIATE(T)                                                         \
  template __attribute__((noinline)) size_t                                    \
  Lt128OptimizeForX280UseShift<T, true>(T ar, T br, MFromV<T> & m);            \
  template __attribute__((noinline)) size_t                                    \
  Lt128OptimizeForX280UseShift<T, false>(T ar, T br, MFromV<T> & m);

INSTANTIATE(vuint64m1_t)
INSTANTIATE(vuint64m2_t)
INSTANTIATE(vuint64m4_t)
INSTANTIATE(vuint64m8_t)
