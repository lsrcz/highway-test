#include "../lt128.h"

template <typename Vec>
__attribute__((always_inline)) size_t
Lt128HighwayManualSchedule124(Vec a, Vec b, MFromV<Vec> &m) {

  register size_t begin asm("t0"), end asm("t1");
  register size_t vl asm("t2");
  register Vec ar asm("v8") = a;
  register Vec br asm("v12") = b;
  register MFromV<Vec> rr asm("v24");
  GET_CYCLE(("+vr"(ar), "+vr"(br)), (), begin);
  LLVM_MCA_BEGIN("Lt128HighwayClang19", ("+vr"(ar), "+vr"(br)), (),
                 decltype(ar));
  SET_VL_MAX_TA_MA(("+vr"(ar), "+vr"(br)), (), vl, decltype(ar));
  asm volatile(
      R"(
              vmsltu.vv	v0, %[ar], %[br]
              vmv.v.i	v16, 0
              vid.v	v20
              vmerge.vim	v24, v16, -1, v0
              vmseq.vv	v0, %[ar], %[br]
              vslide1up.vx	v28, v24, zero
              vmerge.vim	v4, v16, -1, v0
              vand.vi	v20, v20, 1
              vand.vv	v4, v4, v28
              vmseq.vi	v0, v20, 0
              vor.vv	v24, v24, v4
              vslide1down.vx	v24, v24, zero, v0.t
              vmsne.vi	%[rr], v24, 0
      )"
      : [rr] "=vr"(rr), [ar] "+vr"(ar), [br] "+vr"(br)
      :
      : "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v16", "v17", "v18",
        "v19", "v20", "v21", "v22", "v23", "v25", "v26", "v27", "v28", "v29",
        "v30", "v31");
  LLVM_MCA_END("Lt128HighwayClang19", ("+vr"(rr)), (), decltype(ar));
  GET_CYCLE(("+vr"(rr)), (), end);
  m = rr;
  return end - begin;
}

template <>
__attribute__((noinline)) size_t Lt128HighwayManualSchedule<vuint64m1_t>(
    vuint64m1_t a, vuint64m1_t b, vbool64_t &m) {
  return Lt128HighwayManualSchedule124<vuint64m1_t>(a, b, m);
}

template <>
__attribute__((noinline)) size_t Lt128HighwayManualSchedule<vuint64m2_t>(
    vuint64m2_t a, vuint64m2_t b, vbool32_t &m) {
  return Lt128HighwayManualSchedule124<vuint64m2_t>(a, b, m);
}

template <>
__attribute__((noinline)) size_t Lt128HighwayManualSchedule<vuint64m4_t>(
    vuint64m4_t a, vuint64m4_t b, vbool16_t &m) {
  return Lt128HighwayManualSchedule124<vuint64m4_t>(a, b, m);
}
//
// template __attribute__((noinline)) size_t
// Lt128HighwayClang19<vuint64m2_t>(vuint64m2_t a, vuint64m2_t b, vbool32_t &m);
//
// template __attribute__((noinline)) size_t
// Lt128HighwayClang19<vuint64m4_t>(vuint64m4_t a, vuint64m4_t b, vbool16_t &m);
//
// template __attribute__((noinline)) size_t
// Lt128HighwayClang19<vuint64m8_t>(vuint64m8_t a, vuint64m8_t b, vbool8_t &m);
//