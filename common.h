#ifndef COMMON_H_
#define COMMON_H_
#include <riscv_vector.h>

enum class LMul {
  kM8 = 3,
  kM4 = 2,
  kM2 = 1,
  kM1 = 0,
  kMF2 = -1,
  kMF4 = -2,
  kMF8 = -3,
};

template <typename Vec> struct VectorTraits {
  using ElemT = void;
  static constexpr LMul kLMul = static_cast<enum LMul>(-4);
  using MaskT = void;
};

template <typename Vec> using elem_t = typename VectorTraits<Vec>::ElemT;

template <typename Vec> using MFromV = typename VectorTraits<Vec>::MaskT;

template <typename Vec> constexpr LMul lmul = VectorTraits<Vec>::kLMul;

template <typename Vec> size_t Lanes() {
  if (static_cast<int>(lmul<Vec>) >= 1) {
    return __riscv_vlenb() / sizeof(elem_t<Vec>) *
           (1 << static_cast<int>(lmul<Vec>));
  } else {
    return __riscv_vlenb() / sizeof(elem_t<Vec>) /
           (1 << (-static_cast<int>(lmul<Vec>)));
  }
}

template <> struct VectorTraits<vuint64m1_t> {
  using ElemT = uint64_t;
  static constexpr LMul kLMul = LMul::kM1;
  using MaskT = vbool64_t;
};

template <> struct VectorTraits<vuint64m2_t> {
  using ElemT = uint64_t;
  static constexpr LMul kLMul = LMul::kM2;
  using MaskT = vbool32_t;
};

template <> struct VectorTraits<vuint64m4_t> {
  using ElemT = uint64_t;
  static constexpr LMul kLMul = LMul::kM4;
  using MaskT = vbool16_t;
};

template <> struct VectorTraits<vuint64m8_t> {
  using ElemT = uint64_t;
  static constexpr LMul kLMul = LMul::kM8;
  using MaskT = vbool8_t;
};

template <> struct VectorTraits<vint64m1_t> {
  using ElemT = int64_t;
  static constexpr LMul kLMul = LMul::kM1;
  using MaskT = vbool64_t;
};

template <> struct VectorTraits<vint64m2_t> {
  using ElemT = int64_t;
  static constexpr LMul kLMul = LMul::kM2;
  using MaskT = vbool32_t;
};

template <> struct VectorTraits<vint64m4_t> {
  using ElemT = int64_t;
  static constexpr LMul kLMul = LMul::kM4;
  using MaskT = vbool16_t;
};

template <> struct VectorTraits<vint64m8_t> {
  using ElemT = int64_t;
  static constexpr LMul kLMul = LMul::kM8;
  using MaskT = vbool8_t;
};

template <> struct VectorTraits<vuint8mf2_t> {
  using ElemT = uint8_t;
  static constexpr LMul kLMul = LMul::kMF2;
  using MaskT = vbool16_t;
};

template <> struct VectorTraits<vuint8m1_t> {
  using ElemT = uint8_t;
  static constexpr LMul kLMul = LMul::kM1;
  using MaskT = vbool8_t;
};

template <> struct VectorTraits<vuint8m2_t> {
  using ElemT = uint8_t;
  static constexpr LMul kLMul = LMul::kM2;
  using MaskT = vbool4_t;
};

template <> struct VectorTraits<vuint8m4_t> {
  using ElemT = uint8_t;
  static constexpr LMul kLMul = LMul::kM4;
  using MaskT = vbool2_t;
};

template <> struct VectorTraits<vuint8m8_t> {
  using ElemT = uint8_t;
  static constexpr LMul kLMul = LMul::kM8;
  using MaskT = vbool1_t;
};

template <> struct VectorTraits<vint8mf2_t> {
  using ElemT = int8_t;
  static constexpr LMul kLMul = LMul::kMF2;
  using MaskT = vbool16_t;
};

template <> struct VectorTraits<vint8m1_t> {
  using ElemT = int8_t;
  static constexpr LMul kLMul = LMul::kM1;
  using MaskT = vbool8_t;
};

template <> struct VectorTraits<vint8m2_t> {
  using ElemT = int8_t;
  static constexpr LMul kLMul = LMul::kM2;
  using MaskT = vbool4_t;
};

template <> struct VectorTraits<vint8m4_t> {
  using ElemT = int8_t;
  static constexpr LMul kLMul = LMul::kM4;
  using MaskT = vbool2_t;
};

template <> struct VectorTraits<vint8m8_t> {
  using ElemT = int8_t;
  static constexpr LMul kLMul = LMul::kM8;
  using MaskT = vbool1_t;
};

#define EXPAND(...) __VA_ARGS__
#define EXPAND_LIST(list) EXPAND list
#define CONS0(X, ...) (X, ##__VA_ARGS__)
#define CONS(X, list) CONS0(X, EXPAND_LIST(list))

template <bool flag = false> void static_unsupported_lmul() {
  static_assert(flag, "Unsupported LMUL");
}

template <bool flag = false> void static_unsupported_type() {
  static_assert(flag, "Unsupported LMUL");
}

#define LLVM_MCA(directive, name, pseudo_output_dep, pseudo_input_dep, ty)     \
  if constexpr (lmul<ty> == LMul::kM1) {                                       \
    asm volatile("# " directive " " name "-lmul1"                              \
                 : EXPAND_LIST(pseudo_output_dep)                              \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "memory");                                                  \
  } else if constexpr (lmul<ty> == LMul::kM2) {                                \
    asm volatile("# " directive " " name "-lmul2"                              \
                 : EXPAND_LIST(pseudo_output_dep)                              \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "memory");                                                  \
  } else if constexpr (lmul<ty> == LMul::kM4) {                                \
    asm volatile("# " directive " " name "-lmul4"                              \
                 : EXPAND_LIST(pseudo_output_dep)                              \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "memory");                                                  \
  } else if constexpr (lmul<ty> == LMul::kM8) {                                \
    asm volatile("# " directive " " name "-lmul8"                              \
                 : EXPAND_LIST(pseudo_output_dep)                              \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "memory");                                                  \
  } else {                                                                     \
    static_unsupported_lmul();                                                 \
  }                                                                            \
  static_assert(true, "Require trailing semicolon.")

#define LLVM_MCA_BEGIN(name, pseudo_output_dep, pseudo_input_dep, ty)          \
  LLVM_MCA("LLVM-MCA-BEGIN", name, pseudo_output_dep, pseudo_input_dep, ty)
#define LLVM_MCA_END(name, pseudo_output_dep, pseudo_input_dep, ty)            \
  LLVM_MCA("LLVM-MCA-END", name, pseudo_output_dep, pseudo_input_dep, ty)

#define LLVM_MCA_BEGIN_WITH_REP(do_replicate, name, pseudo_output_dep,         \
                                pseudo_input_dep, ty)                          \
  if constexpr (do_replicate) {                                                \
    LLVM_MCA("LLVM-MCA-BEGIN", name "-rep", pseudo_output_dep,                 \
             pseudo_input_dep, ty);                                            \
  } else {                                                                     \
    LLVM_MCA("LLVM-MCA-BEGIN", name, pseudo_output_dep, pseudo_input_dep, ty); \
  }                                                                            \
  static_assert(true, "Require trailing semicolon.")
#define LLVM_MCA_END_WITH_REP(do_replicate, name, pseudo_output_dep,           \
                              pseudo_input_dep, ty)                            \
  if constexpr (do_replicate) {                                                \
    LLVM_MCA("LLVM-MCA-END", name "-rep", pseudo_output_dep, pseudo_input_dep, \
             ty);                                                              \
  } else {                                                                     \
    LLVM_MCA("LLVM-MCA-END", name, pseudo_output_dep, pseudo_input_dep, ty);   \
  }                                                                            \
  static_assert(true, "Require trailing semicolon.")

#define GET_CYCLE(pseudo_output_dep, pseudo_input_dep, var)                    \
  asm volatile("fence\n\trdcycle %[cyclevar]"                                  \
               : EXPAND_LIST(CONS([cyclevar] "=r"(var), pseudo_output_dep))    \
               : EXPAND_LIST(pseudo_input_dep)                                 \
               : "memory");

#define SET_VL_MAX_64_TA_MA(pseudo_output_dep, pseudo_input_dep, vl, ty)       \
  if constexpr (lmul<ty> == LMul::kM1) {                                       \
    asm volatile("vsetvli %[vlreg], zero, e64, m1, ta, ma"                     \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "vl", "memory");                                                  \
  } else if constexpr (lmul<ty> == LMul::kM2) {                                \
    asm volatile("vsetvli %[vlreg], zero, e64, m2, ta, ma"                     \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "vl", "memory");                                                  \
  } else if constexpr (lmul<ty> == LMul::kM4) {                                \
    asm volatile("vsetvli %[vlreg], zero, e64, m4, ta, ma"                     \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "vl", "memory");                                                  \
  } else if constexpr (lmul<ty> == LMul::kM8) {                                \
    asm volatile("vsetvli %[vlreg], zero, e64, m8, ta, ma"                     \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "vl", "memory");                                                  \
  } else {                                                                     \
    static_unsupported_lmul();                                                 \
  }                                                                            \
  static_assert(true, "Require trailing semicolon.")

#define SET_VL_MAX_8_TA_MA(pseudo_output_dep, pseudo_input_dep, vl, ty)        \
  if constexpr (lmul<ty> == LMul::kMF2) {                                      \
    asm volatile("vsetvli %[vlreg], zero, e8, mf2, ta, ma"                     \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "vl", "memory");                                                  \
  } else if constexpr (lmul<ty> == LMul::kM1) {                                \
    asm volatile("vsetvli %[vlreg], zero, e8, m1, ta, ma"                      \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "vl", "memory");                                                  \
  } else if constexpr (lmul<ty> == LMul::kM2) {                                \
    asm volatile("vsetvli %[vlreg], zero, e8, m2, ta, ma"                      \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "vl", "memory");                                                  \
  } else if constexpr (lmul<ty> == LMul::kM4) {                                \
    asm volatile("vsetvli %[vlreg], zero, e8, m4, ta, ma"                      \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "vl", "memory");                                                  \
  } else if constexpr (lmul<ty> == LMul::kM8) {                                \
    asm volatile("vsetvli %[vlreg], zero, e8, m8, ta, ma"                      \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "vl", "memory");                                                  \
  } else {                                                                     \
    static_unsupported_lmul();                                                 \
  }                                                                            \
  static_assert(true, "Require trailing semicolon.")

#define SET_VL_MAX_TA_MA(pseudo_output_dep, pseudo_input_dep, vl, ty)          \
  if constexpr (std::is_same_v<elem_t<ty>, uint8_t> ||                         \
                std::is_same_v<elem_t<ty>, int8_t>) {                          \
    SET_VL_MAX_8_TA_MA(pseudo_output_dep, pseudo_input_dep, vl, ty);           \
  } else if constexpr (std::is_same_v<elem_t<ty>, uint64_t> ||                 \
                       std::is_same_v<elem_t<ty>, int64_t>) {                  \
    SET_VL_MAX_64_TA_MA(pseudo_output_dep, pseudo_input_dep, vl, ty);          \
  } else {                                                                     \
    static_unsupported_type();                                                 \
  }                                                                            \
  static_assert(true, "Require trailing semicolon.")

#define REPEAT_1(X) X
#define REPEAT_2(X) X X
#define REPEAT_4(X) REPEAT_2(X) REPEAT_2(X)
#define REPEAT_8(X) REPEAT_4(X) REPEAT_4(X)
#define REPEAT_16(X) REPEAT_8(X) REPEAT_8(X)
#define REPEAT_32(X) REPEAT_16(X) REPEAT_16(X)
#define REPEAT_64(X) REPEAT_32(X) REPEAT_32(X)
#define REPEAT_128(X) REPEAT_64(X) REPEAT_64(X)

#define REPEAT_N0(N, X) REPEAT_##N(X)

#define REPEAT_N(N, X) REPEAT_N0(N, X)

#define NUM_REPEAT 4

#define REPEAT_DEFAULT(do_replicate, X)                                        \
  if constexpr (do_replicate) {                                                \
    REPEAT_N(NUM_REPEAT, X);                                                   \
  } else {                                                                     \
    X;                                                                         \
  }                                                                            \
  static_assert(true, "Require trailing semicolon.")

#endif
