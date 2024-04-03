#include "../lt128.h"

template <typename Vec>
__attribute__((noinline)) size_t Lt128Highway0(Vec ar, Vec br, MFromV<Vec> &m) {
  auto d = hn::DFromV<Vec>{};
  size_t begin, end;
  GET_CYCLE(("+vr"(ar), "+vr"(br)), (), begin);
  LLVM_MCA_BEGIN("Lt128Highway0", ("+vr"(ar), "+vr"(br)), (), decltype(ar));
  auto rr = hn::Lt128(d, ar, br);
  LLVM_MCA_END("Lt128Highway0", ("+vr"(rr)), (), decltype(ar));
  GET_CYCLE(("+vr"(rr)), (), end);
  m = rr;
  return end - begin;
}

template __attribute__((noinline)) size_t Lt128Highway0<vuint64m1_t>(
    const vuint64m1_t ar, const vuint64m1_t br, vbool64_t &m);

template __attribute__((noinline)) size_t Lt128Highway0<vuint64m2_t>(
    const vuint64m2_t ar, const vuint64m2_t br, vbool32_t &m);

template __attribute__((noinline)) size_t
Lt128Highway0<vuint64m4_t>(vuint64m4_t ar, vuint64m4_t br, vbool16_t &m);

template __attribute__((noinline)) size_t
Lt128Highway0<vuint64m8_t>(vuint64m8_t ar, vuint64m8_t br, vbool8_t &m);

