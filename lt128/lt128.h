#ifndef LT128_LT128_H
#define LT128_LT128_H
#include <common.h>
#include <cstdio>
#include <cstring>
#include <functional>
#include <hwy/highway.h>

namespace hn = hwy::HWY_NAMESPACE;

#define NAME_USEMUL_REPLICATE(name, kUseMul, kReplicate)                       \
  if constexpr (kUseMul) {                                                     \
    if constexpr (kReplicate) {                                                \
      return name "UseMulReplicate";                                           \
    } else {                                                                   \
      return name "UseMul";                                                    \
    }                                                                          \
  } else {                                                                     \
    if constexpr (kReplicate) {                                                \
      return name "Replicate";                                                 \
    } else {                                                                   \
      return name;                                                             \
    }                                                                          \
  }

#define NAME_REPLICATE(name, kReplicate)                                       \
  if constexpr (kReplicate) {                                                  \
    return name "Replicate";                                                   \
  } else {                                                                     \
    return name;                                                               \
  }

// New framework
template <typename Vec>
__attribute__((noinline)) size_t Lt128Highway0(Vec, Vec, MFromV<Vec> &);

struct Lt128Highway0Obj {
  template <typename Vec> size_t operator()(Vec a, Vec b, MFromV<Vec> &m) {
    return Lt128Highway0(a, b, m);
  }

  const char *Name() const { return "Lt128Highway0"; }
};

template <typename Vec, bool kReplicate = false>
__attribute__((noinline)) size_t Lt128HighwayClang19(Vec, Vec, MFromV<Vec> &);

template <bool kReplicate> struct Lt128HighwayClang19Obj {
  template <typename Vec> size_t operator()(Vec a, Vec b, MFromV<Vec> &m) {
    return Lt128HighwayClang19<Vec, kReplicate>(a, b, m);
  }

  const char *Name() const {
    NAME_REPLICATE("Lt128HighwayClang19", kReplicate);
  }
};

template <typename Vec, bool kReplicate = false>
__attribute__((noinline)) size_t Lt128HighwayManualSchedule(Vec, Vec,
                                                            MFromV<Vec> &);

template <bool kReplicate> struct Lt128HighwayManualScheduleObj {
  template <typename Vec> size_t operator()(Vec a, Vec b, MFromV<Vec> &m) {
    return Lt128HighwayManualSchedule<Vec, kReplicate>(a, b, m);
  }

  const char *Name() const {
    NAME_REPLICATE("Lt128HighwayManualSchedule", kReplicate);
  }
};

template <typename Vec, bool kUseMul = false, bool kReplicate = false>
__attribute__((noinline)) size_t Lt128OptimizeForX280(Vec a, Vec b,
                                                      MFromV<Vec> &m);

template <bool kUseMul, bool kReplicate> struct Lt128OptimizeForX280Obj {
  template <typename Vec> size_t operator()(Vec a, Vec b, MFromV<Vec> &m) {
    return Lt128OptimizeForX280<Vec, kUseMul, kReplicate>(a, b, m);
  }

  const char *Name() const {
    NAME_USEMUL_REPLICATE("Lt128OptimizeForX280", kUseMul, kReplicate);
  }
};

template <typename Vec, bool kUseMul = false, bool kReplicate = false>
__attribute__((noinline)) size_t
Lt128OptimizeForX280ChangeVType(Vec a, Vec b, MFromV<Vec> &m);

template <bool kUseMul, bool kReplicate>
struct Lt128OptimizeForX280ChangeVTypeObj {
  template <typename Vec> size_t operator()(Vec a, Vec b, MFromV<Vec> &m) {
    return Lt128OptimizeForX280ChangeVType<Vec, kUseMul, kReplicate>(a, b, m);
  }

  const char *Name() const {
    NAME_USEMUL_REPLICATE("Lt128OptimizeForX280ChangeVType", kUseMul,
                          kReplicate);
  }
};

template <typename Vec, bool kUseMul = false, bool kReplicate = false>
__attribute__((noinline)) size_t Lt128OptimizeForX280UseShift(Vec a, Vec b,
                                                              MFromV<Vec> &m);

template <bool kUseMul, bool kReplicate>
struct Lt128OptimizeForX280UseShiftObj {
  template <typename Vec> size_t operator()(Vec a, Vec b, MFromV<Vec> &m) {
    return Lt128OptimizeForX280UseShift<Vec, kUseMul, kReplicate>(a, b, m);
  }

  const char *Name() const {
    NAME_USEMUL_REPLICATE("Lt128OptimizeForX280UseShift", kUseMul, kReplicate);
  }
};

template <typename Vec, bool kUseMul = false, bool kReplicate = false>
__attribute__((noinline)) size_t Lt128OptimizeForX280UseAdd(Vec a, Vec b,
                                                            MFromV<Vec> &m);

template <bool kUseMul, bool kReplicate> struct Lt128OptimizeForX280UseAddObj {
  template <typename Vec> size_t operator()(Vec a, Vec b, MFromV<Vec> &m) {
    return Lt128OptimizeForX280UseAdd<Vec, kUseMul, kReplicate>(a, b, m);
  }

  const char *Name() const {
    NAME_USEMUL_REPLICATE("Lt128OptimizeForX280UseAdd", kUseMul, kReplicate);
  }
};

template <typename Vec, bool kReplicate = false>
__attribute__((noinline)) size_t Lt128Synthesized(Vec, Vec, MFromV<Vec> &);

template <bool kReplicate> struct Lt128SynthesizedObj {
  template <typename Vec> size_t operator()(Vec a, Vec b, MFromV<Vec> &m) {
    return Lt128Synthesized<Vec, kReplicate>(a, b, m);
  }

  const char *Name() const { NAME_REPLICATE("Lt128Synthesized", kReplicate); }
};

template <typename Vec, bool kReplicate = false>
__attribute__((noinline)) size_t Lt128Synthesized2(Vec, Vec, MFromV<Vec> &);

template <bool kReplicate> struct Lt128Synthesized2Obj {
  template <typename Vec> size_t operator()(Vec a, Vec b, MFromV<Vec> &m) {
    return Lt128Synthesized2<Vec, kReplicate>(a, b, m);
  }

  const char *Name() const { NAME_REPLICATE("Lt128Synthesized2", kReplicate); }
};

// Run a single template on a single type, you need to change the printing.
template <typename Obj, typename T>
void RunAndPrintCyclesObj(const uint64_t *pa, const uint64_t *pb) {
  const hn::DFromV<T> d;
  auto ar = hn::Load(d, pa);
  auto br = hn::Load(d, pb);
  MFromV<T> m;
  Obj obj;
  obj(ar, br, m); // warmup
  size_t cycles = obj(ar, br, m);
  // Change printing

  printf("%s<%s>: %zu cycles.\n", obj.Name(), typeid(T).name(), cycles);
}

// Run a single template on a list of types. No need to change this.
template <typename Obj, typename... T>
void RunAndPrintCyclesObjAllTypes(const uint64_t *pa, const uint64_t *pb) {
  ((void)RunAndPrintCyclesObj<Obj, T>(pa, pb), ...);
}

// Run a list of templates on a list of types. To add a template, directly add
// it to the function body.
template <bool kReplicate, typename... T>
void RunAndPrintCyclesObjAllTemplatesTypes(const uint64_t *pa,
                                           const uint64_t *pb) {
  RunAndPrintCyclesObjAllTypes<Lt128Highway0Obj, T...>(pa, pb);
  RunAndPrintCyclesObjAllTypes<Lt128HighwayClang19Obj<kReplicate>, T...>(pa,
                                                                         pb);
  RunAndPrintCyclesObjAllTypes<Lt128HighwayManualScheduleObj<kReplicate>, T...>(
      pa, pb);
  RunAndPrintCyclesObjAllTypes<Lt128OptimizeForX280Obj<true, kReplicate>, T...>(
      pa, pb);
  RunAndPrintCyclesObjAllTypes<Lt128OptimizeForX280Obj<false, kReplicate>,
                               T...>(pa, pb);
  RunAndPrintCyclesObjAllTypes<
      Lt128OptimizeForX280ChangeVTypeObj<true, kReplicate>, T...>(pa, pb);
  RunAndPrintCyclesObjAllTypes<
      Lt128OptimizeForX280ChangeVTypeObj<false, kReplicate>, T...>(pa, pb);
  RunAndPrintCyclesObjAllTypes<
      Lt128OptimizeForX280UseShiftObj<true, kReplicate>, T...>(pa, pb);
  RunAndPrintCyclesObjAllTypes<
      Lt128OptimizeForX280UseShiftObj<false, kReplicate>, T...>(pa, pb);
}

// Old code

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128Highway(const uint64_t *HWY_RESTRICT a,
                  const uint64_t *HWY_RESTRICT b, uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128HighwayAsm(const uint64_t *HWY_RESTRICT a,
                     const uint64_t *HWY_RESTRICT b, uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128HighwayAsmImmOddEven(const uint64_t *HWY_RESTRICT a,
                               const uint64_t *HWY_RESTRICT b,
                               uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128Manual1(const uint64_t *HWY_RESTRICT a,
                  const uint64_t *HWY_RESTRICT b, uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128Manual1OptimizeForX280(const uint64_t *HWY_RESTRICT a,
                                 const uint64_t *HWY_RESTRICT b,
                                 uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128Manual1OptimizeForP670(const uint64_t *HWY_RESTRICT a,
                                 const uint64_t *HWY_RESTRICT b,
                                 uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128Manual1WithGlobal(const uint64_t *HWY_RESTRICT a,
                            const uint64_t *HWY_RESTRICT b,
                            uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128Manual1Intrinsics(const uint64_t *HWY_RESTRICT a,
                            const uint64_t *HWY_RESTRICT b,
                            uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128Manual2(const uint64_t *HWY_RESTRICT a,
                  const uint64_t *HWY_RESTRICT b, uint8_t *HWY_RESTRICT r);

template <typename tag = hn::ScalableTag<uint64_t>>
void Lt128Manual2Intrinsics(const uint64_t *HWY_RESTRICT a,
                            const uint64_t *HWY_RESTRICT b,
                            uint8_t *HWY_RESTRICT r);

void CheckLt128(
    std::function<void(const uint64_t *, const uint64_t *, uint8_t *)> f0,
    std::function<void(const uint64_t *, const uint64_t *, uint8_t *)> f1);

template <typename Obj0, typename Obj1, typename Vec>
void CheckLt128Reg1(int printCycleIter = -1) {
  Obj0 obj0;
  Obj1 obj1;
  using D = hn::DFromV<Vec>;
  auto numElem = hn::Lanes(D());
  printf("Comparing %s and %s, num_elem = %zu\n", obj0.Name(), obj1.Name(),
         numElem);
  for (int i = 0; i < 100; ++i) {
    uint64_t a[128] = {0, 1};
    uint64_t b[128] = {1, 0};
    uint8_t r0[128];
    uint8_t r1[128];
    memset(r0, 0xcc, sizeof(r0));
    memset(r1, 0xcc, sizeof(r1));
    for (int i = 0; i < numElem; ++i) {
      a[i] = rand() % 100;
      b[i] = rand() % 100;
    }
    D d;
    auto ar = hn::Load(d, a);
    auto br = hn::Load(d, b);
    hn::MFromD<D> mr0;
    hn::MFromD<D> mr1;
    size_t cycle_f0 = obj0(ar, br, mr0);
    size_t cycle_f1 = obj1(ar, br, mr1);
    hn::StoreMaskBits(d, mr0, r0);
    hn::StoreMaskBits(d, mr1, r1);
    if (i <= printCycleIter)
      printf("Cycle: %s:%lu, %s:%lu\n", obj0.Name(), cycle_f0, obj1.Name(),
             cycle_f1);

    for (int i = 0; i < numElem; i++) {
      bool mismatch = false;
      if (r0[i] != r1[i]) {
        printf("Mismatch at %d: %d != %d\n", i, r0[i], r1[i]);
        mismatch = true;
      }
      if (mismatch) {
        printf("a\tb\tr0\tr1\tidx:\n");
        for (int i = 0; i < numElem; ++i) {
          printf("%lu\t%lu\t%u\t%u\t%d\n", a[i], b[i], r0[i / 8], r1[i / 8],
                 i / 8);
        }
        return;
      }
    }
  }
}

template <typename Obj0, typename Obj1, typename... Vec>
void CheckLt128RegAllTypes(int printCycleIter = -1) {
  ((void)CheckLt128Reg1<Obj0, Obj1, Vec>(), ...);
}

#endif
