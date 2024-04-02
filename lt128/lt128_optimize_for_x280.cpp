#include "lt128.h"
#include <cstdio>

template <typename tag>
__attribute__((noinline)) size_t Lt128OptimizeForX280(tag d, hn::VFromD<tag> a,
                                                      hn::VFromD<tag> b,
                                                      hn::MFromD<tag> &m) {
  size_t begin, end;
  size_t vl;
  register hn::VFromD<tag> ar asm("v16") = a;
  register hn::VFromD<tag> br asm("v24") = b;
  register hn::MFromD<tag> r asm("v8");
  GET_CYCLE(("+vr"(ar), "+vr"(br)), (), begin);
  LLVM_MCA_BEGIN("Lt128OptimizeForX280", ("+vr"(ar), "+vr"(br)), (),
                 decltype(ar));
  SET_VL_MAX_TA_MA(("+vr"(ar), "+vr"(br)), (), vl, decltype(ar));
  asm volatile(
      R"(
               lui	a0, 349525
               vslide1down.vx    v0, %[ar], zero
               addiw	a0, a0, 1365
               slli	a1, a0, 32
               vslide1down.vx    v8, %[br], zero
               add	a0, a0, a1
               vmsltu.vv	v16, %[ar], %[br]
               vmseq.vv	v17, v0, v8
               vmsltu.vv	v18, v0, v8
               vmv.v.x v24, a0
               vmand.mm	v17, v17, v16
               vmor.mm	v16, v17, v18
               vmand.mm	v16, v16, v24
               vadd.vv	v24, v16, v16
               vmor.mm	%[r], v24, v16
               )"
      : [r] "=vr"(r)
      : [ar] "vr"(ar), [br] "vr"(br)
      : "a0", "a1", "a3", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v9",
        "v10", "v11", "v12", "v13", "v14", "v15", "memory");
  LLVM_MCA_END("Lt128OptimizeForX280", ("+vr"(r)), (), decltype(ar));
  GET_CYCLE(("+vr"(r)), (), end);
  m = r;
  return end - begin;
}

template __attribute__((noinline)) size_t
Lt128OptimizeForX280<hn::ScalableTag<uint64_t>>(hn::ScalableTag<uint64_t> d,
                                                rvv::vreg_t<uint64_t, 64> ar,
                                                rvv::vreg_t<uint64_t, 64> br,
                                                rvv::vmask_t<64> &m);

template __attribute__((noinline)) size_t
Lt128OptimizeForX280<hn::Twice<hn::ScalableTag<uint64_t>>>(
    hn::Twice<hn::ScalableTag<uint64_t>> d, rvv::vreg_t<uint64_t, 32> ar,
    rvv::vreg_t<uint64_t, 32> br, rvv::vmask_t<32> &m);

template __attribute__((noinline)) size_t
Lt128OptimizeForX280<hn::Twice<hn::Twice<hn::ScalableTag<uint64_t>>>>(
    hn::Twice<hn::Twice<hn::ScalableTag<uint64_t>>> d,
    rvv::vreg_t<uint64_t, 16> ar, rvv::vreg_t<uint64_t, 16> br,
    rvv::vmask_t<16> &m);

template __attribute__((noinline)) size_t Lt128OptimizeForX280<
    hn::Twice<hn::Twice<hn::Twice<hn::ScalableTag<uint64_t>>>>>(
    hn::Twice<hn::Twice<hn::Twice<hn::ScalableTag<uint64_t>>>> d,
    rvv::vreg_t<uint64_t, 8> ar, rvv::vreg_t<uint64_t, 8> br,
    rvv::vmask_t<8> &m);
