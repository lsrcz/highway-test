#include "lt128.h"
#include <cstdio>

template <typename tag>
__attribute__((noinline)) size_t Lt128OptimizeForX280UseShift(
    tag d, hn::VFromD<tag> a, hn::VFromD<tag> b, hn::MFromD<tag> &m) {
  size_t begin, end;
  size_t vl;
  register hn::VFromD<tag> ar asm("v16") = a;
  register hn::VFromD<tag> br asm("v24") = b;
  register hn::MFromD<tag> r asm("v8"), sltu asm("v8"), seq asm("v9");
  register uint64_t a0 asm("a0"), a1 asm("a1");
  GET_CYCLE(("+vr"(ar), "+vr"(br)), (), begin);
  LLVM_MCA_BEGIN("Lt128OptimizeForX280UseShift", ("+vr"(ar), "+vr"(br)), (),
                 decltype(ar));
  SET_VL_MAX_TA_MA(("+vr"(ar), "+vr"(br)), (), vl, decltype(ar));
  asm volatile(
      R"(
               vmseq.vv %[seq], %[ar], %[br]
               lui	%[a0], 349525
               vmsltu.vv %[sltu], %[ar], %[br]
               addiw	%[a0], %[a0], 1365
               slli	%[a1], %[a0], 32
               )"
      : [sltu] "=vr"(sltu), [seq] "=vr"(seq), [a0] "=r"(a0), [a1] "=r"(a1)
      : [ar] "vr"(ar), [br] "vr"(br)
      : "memory");
  SET_VL_MAX_TA_MA(("+vr"(sltu)), (), vl, vuint8mf2_t);
  asm volatile(
      R"(
               add	a0, a0, a1
               vsrl.vi    v0, %[seq], 1
               vmv.v.x v24, %[a0]
               vsrl.vi   v1, %[sltu], 1
               vand.vv	v17, v0, %[sltu]
               vor.vv	v16, v17, v1
               vand.vv	v16, v16, v24
               vadd.vv	v24, v16, v16
               vor.vv	%[r], v24, v16
               )"
      : [r] "=vr"(r)
      : [sltu] "vr"(sltu), [seq] "vr"(seq), [a0] "r"(a0), [a1] "r"(a1)
      : "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31", "memory");
  LLVM_MCA_END("Lt128OptimizeForX280UseShift", ("+vr"(r)), (), decltype(ar));
  GET_CYCLE(("+vr"(r)), (), end);
  m = r;
  return end - begin;
}

template __attribute__((noinline)) size_t
Lt128OptimizeForX280UseShift<hn::ScalableTag<uint64_t>>(
    hn::ScalableTag<uint64_t> d, rvv::vreg_t<uint64_t, 64> ar,
    rvv::vreg_t<uint64_t, 64> br, rvv::vmask_t<64> &m);

template __attribute__((noinline)) size_t
Lt128OptimizeForX280UseShift<hn::Twice<hn::ScalableTag<uint64_t>>>(
    hn::Twice<hn::ScalableTag<uint64_t>> d, rvv::vreg_t<uint64_t, 32> ar,
    rvv::vreg_t<uint64_t, 32> br, rvv::vmask_t<32> &m);

template __attribute__((noinline)) size_t
Lt128OptimizeForX280UseShift<hn::Twice<hn::Twice<hn::ScalableTag<uint64_t>>>>(
    hn::Twice<hn::Twice<hn::ScalableTag<uint64_t>>> d,
    rvv::vreg_t<uint64_t, 16> ar, rvv::vreg_t<uint64_t, 16> br,
    rvv::vmask_t<16> &m);

template __attribute__((noinline)) size_t Lt128OptimizeForX280UseShift<
    hn::Twice<hn::Twice<hn::Twice<hn::ScalableTag<uint64_t>>>>>(
    hn::Twice<hn::Twice<hn::Twice<hn::ScalableTag<uint64_t>>>> d,
    rvv::vreg_t<uint64_t, 8> ar, rvv::vreg_t<uint64_t, 8> br,
    rvv::vmask_t<8> &m);
