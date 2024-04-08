#include "../lt128.h"

template <typename Vec, bool kReplicate>
__attribute__((noinline)) size_t Lt128Synthesized2(Vec a, Vec b,
                                                   MFromV<Vec> &m) {
  register size_t begin asm("t0"), end asm("t1");
  register size_t vl asm("t2");
  register Vec ar asm("v16") = a;
  register Vec br asm("v24") = b;
  register MFromV<Vec> r asm("v8"), sltu asm("v9"), seq asm("v10"),
      sltu2 asm("v11");
  register uint64_t t3 asm("t3"), t4 asm("t4");
  GET_CYCLE(("+vr"(ar), "+vr"(br)), (), begin);
  LLVM_MCA_BEGIN_WITH_REP(kReplicate, "Lt128Synthesized2",
                          ("+vr"(ar), "+vr"(br)), (), decltype(ar));
  REPEAT_DEFAULT(
      kReplicate,
      SET_VL_MAX_TA_MA(("+vr"(ar), "+vr"(br)), (), vl, decltype(ar));
      asm volatile(
          R"(
               vmsltu.vv %[sltu], %[ar], %[br]
               lui	%[t3], 0x55555
               vmseq.vv %[seq], %[ar], %[br]
               addiw	%[t3], %[t3], 0x555
               slli	%[t4], %[t3], 32
               )"
          : [sltu] "+vr"(sltu), [seq] "+vr"(seq), [t3] "+r"(t3), [t4] "+r"(t4)
          : [ar] "vr"(ar), [br] "vr"(br));
      SET_VL_MAX_TA_MA(("+vr"(sltu)), (), vl, vuint8mf2_t); asm volatile(
          R"(
               vadd.vv %[sltu2], %[sltu], %[sltu]
               add	%[t3], %[t3], %[t4]
               slli %[t3], %[t3], 1
               vand.vv %[r], %[sltu2], %[seq]
               vmv.v.x v24, %[t3]
               vadd.vv %[r], %[r], %[sltu] # This is where it differs from the first synthesized program
               vand.vv %[r], %[r], v24
               vsrl.vi v24, %[r], 1
               vadd.vv %[r], v24, %[r]
               )"
          : [r] "=vr"(r), [t3] "=r"(t3), [t4] "=r"(t4), [sltu] "+vr"(sltu),
            [seq] "+vr"(seq), [sltu2] "+vr"(sltu2)
          : [ar] "vr"(ar), [br] "vr"(br)
          : "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v12", "v13", "v14",
            "v15", "memory");
      asm volatile(""
                   : [ar] "=vr"(ar), [br] "=vr"(br)::"memory"););
  LLVM_MCA_END_WITH_REP(kReplicate, "Lt128Synthesized2", ("+vr"(r)), (),
                        decltype(ar));
  GET_CYCLE(("+vr"(r)), (), end);
  m = r;
  return end - begin;
}

#define INSTANTIATE(T)                                                         \
  template __attribute__((noinline)) size_t Lt128Synthesized2<T, true>(        \
      T ar, T br, MFromV<T> & m);                                              \
  template __attribute__((noinline)) size_t Lt128Synthesized2<T, false>(       \
      T ar, T br, MFromV<T> & m);

INSTANTIATE(vuint64m1_t)
INSTANTIATE(vuint64m2_t)
INSTANTIATE(vuint64m4_t)
INSTANTIATE(vuint64m8_t)

#undef INSTANTIATE
