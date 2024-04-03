#include "../lt128.h"

template <typename Vec>
__attribute__((noinline)) size_t
Lt128OptimizeForX280ChangeVType(Vec a, Vec b, MFromV<Vec> &m) {
  register size_t begin asm("t0"), end asm("t1");
  register size_t vl asm("t2");
  register Vec ar asm("v16") = a;
  register Vec br asm("v24") = b;
  register MFromV<Vec> r asm("v8"), sltu asm("v16"), seqs asm("v17"),
      sltus asm("v18");
  register uint64_t t3 asm("t3"), t4 asm("t4");
  GET_CYCLE(("+vr"(ar), "+vr"(br)), (), begin);
  LLVM_MCA_BEGIN("Lt128OptimizeForX280ChangeVType", ("+vr"(ar), "+vr"(br)), (),
                 decltype(ar));
  SET_VL_MAX_TA_MA(("+vr"(ar), "+vr"(br)), (), vl, decltype(ar));
  asm volatile(
      R"(
               lui	%[t3], 349525
               vslide1down.vx    v0, %[ar], zero
               addiw	%[t3], %[t3], 1365
               slli	%[t4], %[t3], 32
               vslide1down.vx    v8, %[br], zero
               add	%[t3], %[t3], %[t4]
               vmsltu.vv	%[sltu], %[ar], %[br]
               vmseq.vv	%[seqs], v0, v8
               vmsltu.vv	%[sltus], v0, v8
               )"
      : [sltu] "=vr"(sltu), [seqs] "=vr"(seqs), [sltus] "=vr"(sltus),
        [t3] "=r"(t3), [t4] "=r"(t4)
      : [ar] "vr"(ar), [br] "vr"(br)
      : "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v9", "v10", "v11",
        "v12", "v13", "v14", "v15", "memory");
  SET_VL_MAX_TA_MA(("+vr"(sltu)), (), vl, vuint8mf2_t);
  asm volatile(
      R"(
               vmv.v.x v24, %[t3]
               vand.vv	v17, %[seqs], %[sltu]
               vor.vv	v16, v17, %[sltus]
               vand.vv	v16, v16, v24
               vadd.vv	v24, v16, v16
               vor.vv	%[r], v24, v16
               )"
      : [r] "=vr"(r)
      : [sltu] "vr"(sltu), [seqs] "vr"(seqs), [sltus] "vr"(sltus), [t3] "r"(t3)
      : "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31", "memory");
  LLVM_MCA_END("Lt128OptimizeForX280ChangeVType", ("+vr"(r)), (), decltype(ar));
  GET_CYCLE(("+vr"(r)), (), end);
  m = r;
  return end - begin;
}

template __attribute__((noinline)) size_t
Lt128OptimizeForX280ChangeVType<vuint64m1_t>(vuint64m1_t ar, vuint64m1_t br,
                                             vbool64_t &m);

template __attribute__((noinline)) size_t
Lt128OptimizeForX280ChangeVType<vuint64m2_t>(vuint64m2_t ar, vuint64m2_t br,
                                             vbool32_t &m);

template __attribute__((noinline)) size_t
Lt128OptimizeForX280ChangeVType<vuint64m4_t>(vuint64m4_t ar, vuint64m4_t br,
                                             vbool16_t &m);

template __attribute__((noinline)) size_t
Lt128OptimizeForX280ChangeVType<vuint64m8_t>(vuint64m8_t ar, vuint64m8_t br,
                                             vbool8_t &m);
