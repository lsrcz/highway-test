// Generates code for every target that this compiler can support.
#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "main.cpp" // this file
#include <hwy/foreach_target.h>       // must come before highway.h
#include <hwy/highway.h>
#include <stdio.h>
#include <string.h>
#if defined(__riscv)
#include <rvv/int.h>
#include <rvv/load_store.h>
#include <rvv/misc.h>
#endif

namespace project {
namespace HWY_NAMESPACE { // required: unique per target

// Can skip hn:: prefixes if already inside hwy::HWY_NAMESPACE.
namespace hn = hwy::HWY_NAMESPACE;

using T = float;

// Alternative to per-function HWY_ATTR: see HWY_BEFORE_NAMESPACE
// HWY_ATTR void MulAddLoop(const T* HWY_RESTRICT mul_array,
//                 const T* HWY_RESTRICT add_array,
//                 const size_t size, T* HWY_RESTRICT x_array) {
//   const hn::ScalableTag<T> d;
//   for (size_t i = 0; i < size; i += hn::Lanes(d)) {
//     const auto mul = hn::Load(d, mul_array + i);
//     const auto add = hn::Load(d, add_array + i);
//     auto x = hn::Load(d, x_array + i);
//     x = hn::MulAdd(mul, x, add);
//     hn::Store(x, d, x_array + i);
//   }
// }

HWY_ATTR void ReorderWidenMulAccumulateTest(const uint16_t *HWY_RESTRICT a,
                                            const uint16_t *HWY_RESTRICT b,
                                            uint32_t *HWY_RESTRICT s0,
                                            uint32_t *HWY_RESTRICT s1) {
  const hn::ScalableTag<uint32_t> d32;
  const hn::RepartitionToNarrow<decltype(d32)> d;
  using V32 = hn::VFromD<decltype(d32)>;
  using V16 = hn::VFromD<decltype(d)>;
  const V16 ar = hn::Load(d, a);
  const V16 br = hn::Load(d, b);
  const V32 s0r = hn::Load(d32, s0);
  V32 s1r = hn::Zero(d32);
  const auto r = hn::ReorderWidenMulAccumulate(d32, ar, br, s0r, s1r);

  hn::Store(r, d32, s0);
  hn::Store(s1r, d32, s1);
}

} // namespace HWY_NAMESPACE
} // namespace project

// The table of pointers to the various implementations in HWY_NAMESPACE must
// be compiled only once (foreach_target #includes this file multiple times).
// HWY_ONCE is true for only one of these 'compilation passes'.
#if HWY_ONCE

#if defined(__riscv)
void ManualReorderWidenMulAccumulateTest(const uint16_t *HWY_RESTRICT a,
                                         const uint16_t *HWY_RESTRICT b,
                                         uint32_t *HWY_RESTRICT s0,
                                         uint32_t *HWY_RESTRICT s1) {
  rvv::vl_t<16> vl = rvv::vsetvlmax<16>();
  using V16M1 = rvv::vreg_t<uint16_t, 16>;
  V16M1 ar = rvv::vle(a, vl);
  V16M1 br = rvv::vle(b, vl);

  rvv::vl_t<32> vl32 = rvv::vsetvlmax<32>();
  using V32M1 = rvv::vreg_t<uint32_t, 32>;
  V32M1 s0r = rvv::vle(s0, vl32);
  V32M1 s1r = rvv::vmv_v(uint32_t{0}, vl32);
  // Not supported on clang 17.0.6
  // vuint32m2_t s0s1r = __riscv_vcreate_v_u32m1_u32m2(s0r, s1r);
  using V32M2 = rvv::vreg_t<uint32_t, 16>;
  V32M2 s0s1r = __riscv_vundefined_u32m2();
  s0s1r = __riscv_vset_v_u32m1_u32m2(s0s1r, 0, s0r);
  s0s1r = __riscv_vset_v_u32m1_u32m2(s0s1r, 1, s1r);
  V32M2 cr = rvv::vwmaccu(s0s1r, ar, br, vl);
  V32M1 news0r = __riscv_vget_v_u32m2_u32m1(cr, 0);
  V32M1 news1r = __riscv_vget_v_u32m2_u32m1(cr, 1);
  rvv::vse(s0, news0r, vl32);
  rvv::vse(s1, news1r, vl32);
}

#endif

namespace project {

// This macro declares a static array used for dynamic dispatch.
// HWY_EXPORT(MulAddLoop);
HWY_EXPORT(ReorderWidenMulAccumulateTest);

// void CallMulAddLoop(const float* HWY_RESTRICT mul_array,
//                 const float* HWY_RESTRICT add_array,
//                 const size_t size, float* HWY_RESTRICT x_array) {
//   // This must reside outside of HWY_NAMESPACE because it references (calls
//   the
//   // appropriate one from) the per-target implementations there.
//   // For static dispatch, use HWY_STATIC_DISPATCH.
//   return HWY_DYNAMIC_DISPATCH(MulAddLoop)(mul_array, add_array, size,
//   x_array);
// }

int main_func() {
  uint16_t a[32];
  uint16_t b[32];
  uint32_t s0[32];
  uint32_t s1[32];
  for (int i = 0; i < 32; ++i) {
    a[i] = i;
    b[i] = i;
    s0[i] = 1000;
    s1[i] = 1000;
  }
  HWY_DYNAMIC_DISPATCH(ReorderWidenMulAccumulateTest)(a, b, s0, s1);

  for (int i = 0; i < 32; ++i) {
    printf("%d %d %d\n", i, s0[i], s1[i]);
  }

#if defined(__riscv)
  for (int i = 0; i < 32; ++i) {
    a[i] = i;
    b[i] = i;
    s0[i] = 1000;
    s1[i] = 1000;
  }
  ManualReorderWidenMulAccumulateTest(a, b, s0, s1);

  for (int i = 0; i < 32; ++i) {
    printf("%d %d %d\n", i, s0[i], s1[i]);
  }
#endif
  return 0;
}
} // namespace project

int main() {
  project::main_func();
  return 0;
}

#endif // HWY_ONCE
