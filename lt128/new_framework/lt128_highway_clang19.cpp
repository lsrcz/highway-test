#include "../lt128.h"
template <typename Vec, bool kReplicate>
__attribute__((noinline)) size_t Lt128HighwayClang19_12(Vec a, Vec b,
                                                        MFromV<Vec> &m) {

  register size_t begin asm("t0"), end asm("t1");
  register size_t vl asm("t2");
  register Vec ar asm("v8") = a;
  register Vec br asm("v16") = b;
  register MFromV<Vec> rr asm("v24");
  GET_CYCLE(("+vr"(ar), "+vr"(br)), (), begin);
  LLVM_MCA_BEGIN_WITH_REP(kReplicate, "Lt128HighwayClang19",
                          ("+vr"(ar), "+vr"(br)), (), decltype(ar));

  REPEAT_DEFAULT(kReplicate,
                 SET_VL_MAX_TA_MA(("+vr"(ar), "+vr"(br)), (), vl, decltype(ar));
                 asm volatile(
                     R"(
          vmseq.vv  v0, %[ar], %[br]
          vmsltu.vv  v7, %[ar], %[br]
          vmv.v.i  v24, 0
          vmerge.vim  v16, v24, -1, v0
          vmv1r.v  v0, v7
          vmerge.vim  v8, v24, -1, v0
          vid.v  v28
          vslide1up.vx  v24, v8, zero
          vand.vi  v28, v28, 1
          vand.vv  v16, v16, v24
          vmseq.vi  v0, v28, 0
          vor.vv  v8, v8, v16
          vslide1down.vx  v8, v8, zero, v0.t
          vmsne.vi  %[rr], v8, 0
      )"
                     : [rr] "=vr"(rr), [ar] "+vr"(ar), [br] "+vr"(br)
                     :
                     : "v0", "v7");
                 asm volatile(""
                              : [ar] "=vr"(ar), [br] "=vr"(br)::"memory"););
  LLVM_MCA_END_WITH_REP(kReplicate, "Lt128HighwayClang19", ("+vr"(rr)), (),
                        decltype(ar));
  GET_CYCLE(("+vr"(rr)), (), end);
  m = rr;
  return end - begin;
}

template <typename Vec, bool kReplicate>
__attribute__((noinline)) size_t Lt128HighwayClang19_48(Vec a, Vec b,
                                                        MFromV<Vec> &m) {

  register size_t begin asm("t0"), end asm("t1");
  register size_t vl asm("t2");
  register Vec ar asm("v8") = a;
  register Vec br asm("v16") = b;
  register MFromV<Vec> rr asm("v24");
  GET_CYCLE(("+vr"(ar), "+vr"(br)), (), begin);
  LLVM_MCA_BEGIN_WITH_REP(kReplicate, "Lt128HighwayClang19",
                          ("+vr"(ar), "+vr"(br)), (), decltype(ar));
  REPEAT_DEFAULT(kReplicate,
                 SET_VL_MAX_TA_MA(("+vr"(ar), "+vr"(br)), (), vl, decltype(ar));
                 asm volatile(
                     R"(
          vmseq.vv  v0, %[ar], %[br]
          vmsltu.vv  v7, %[ar], %[br]
          vmv.v.i  v24, 0
          vmerge.vim  v16, v24, -1, v0
          vmv1r.v  v0, v7
          vmerge.vim  v8, v24, -1, v0
          vslide1up.vx  v24, v8, zero
          vand.vv  v16, v16, v24
          vid.v  v24
          vand.vi  v24, v24, 1
          vmseq.vi  v0, v24, 0
          vor.vv  v8, v8, v16
          vslide1down.vx  v8, v8, zero, v0.t
          vmsne.vi  %[rr], v8, 0
      )"
                     : [rr] "=vr"(rr), [ar] "+vr"(ar), [br] "+vr"(br)
                     :
                     : "v0", "v7");
                 asm volatile(""
                              : [ar] "=vr"(ar), [br] "=vr"(br)::"memory"););
  LLVM_MCA_END_WITH_REP(kReplicate, "Lt128HighwayClang19", ("+vr"(rr)), (),
                        decltype(ar));
  GET_CYCLE(("+vr"(rr)), (), end);
  m = rr;
  return end - begin;
}

#define SPECIALIZE(T, postfix)                                                 \
  template <>                                                                  \
  __attribute__((noinline)) size_t Lt128HighwayClang19<T, true>(               \
      T ar, T br, MFromV<T> & m) {                                             \
    return Lt128HighwayClang19_##postfix<T, true>(ar, br, m);                  \
  }                                                                            \
  template <>                                                                  \
  __attribute__((noinline)) size_t Lt128HighwayClang19<T, false>(              \
      T ar, T br, MFromV<T> & m) {                                             \
    return Lt128HighwayClang19_##postfix<T, false>(ar, br, m);                 \
  }

SPECIALIZE(vuint64m1_t, 12)
SPECIALIZE(vuint64m2_t, 12)
SPECIALIZE(vuint64m4_t, 48)
SPECIALIZE(vuint64m8_t, 48)

#undef SPECIALIZE
