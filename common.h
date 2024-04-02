#include <hwy/highway.h>
#include <rvv/type.h>

#define EXPAND(...) __VA_ARGS__
#define EXPAND_LIST(list) EXPAND list
#define CONS0(X, ...) (X, ##__VA_ARGS__)
#define CONS(X, list) CONS0(X, EXPAND_LIST(list))

#define LLVM_MCA(directive, name, pseudo_output_dep, pseudo_input_dep, ty)     \
  if constexpr (rvv::lmul<ty> == rvv::LMul::kM1) {                             \
    asm volatile("# " directive " " name "-lmul1"                              \
                 : EXPAND_LIST(pseudo_output_dep)                              \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "memory");                                                  \
  } else if constexpr (rvv::lmul<ty> == rvv::LMul::kM2) {                      \
    asm volatile("# " directive " " name "-lmul2"                              \
                 : EXPAND_LIST(pseudo_output_dep)                              \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "memory");                                                  \
  } else if constexpr (rvv::lmul<ty> == rvv::LMul::kM4) {                      \
    asm volatile("# " directive " " name "-lmul4"                              \
                 : EXPAND_LIST(pseudo_output_dep)                              \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "memory");                                                  \
  } else if constexpr (rvv::lmul<ty> == rvv::LMul::kM8) {                      \
    asm volatile("# " directive " " name "-lmul8"                              \
                 : EXPAND_LIST(pseudo_output_dep)                              \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "memory");                                                  \
  } else {                                                                     \
    static_assert("Unsupported LMUL");                                         \
  }                                                                            \
  static_assert(true, "Require trailing semicolon.")

#define LLVM_MCA_BEGIN(name, pseudo_output_dep, pseudo_input_dep, ty)          \
  LLVM_MCA("LLVM-MCA-BEGIN", name, pseudo_output_dep, pseudo_input_dep, ty)
#define LLVM_MCA_END(name, pseudo_output_dep, pseudo_input_dep, ty)            \
  LLVM_MCA("LLVM-MCA-END", name, pseudo_output_dep, pseudo_input_dep, ty)

#define GET_CYCLE(pseudo_output_dep, pseudo_input_dep, var)                    \
  asm volatile("fence\n\trdcycle %[cyclevar]"                                  \
               : EXPAND_LIST(CONS([cyclevar] "=r"(var), pseudo_output_dep))    \
               : EXPAND_LIST(pseudo_input_dep)                                 \
               : "memory");

#define SET_VL_MAX_64_TA_MA(pseudo_output_dep, pseudo_input_dep, vl, ty)       \
  if constexpr (rvv::lmul<ty> == rvv::LMul::kM1) {                             \
    asm volatile("vsetvli %[vlreg], zero, e64, m1, ta, ma"                     \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "memory");                                                  \
  } else if constexpr (rvv::lmul<ty> == rvv::LMul::kM2) {                      \
    asm volatile("vsetvli %[vlreg], zero, e64, m2, ta, ma"                     \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "memory");                                                  \
  } else if constexpr (rvv::lmul<ty> == rvv::LMul::kM4) {                      \
    asm volatile("vsetvli %[vlreg], zero, e64, m4, ta, ma"                     \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "memory");                                                  \
  } else if constexpr (rvv::lmul<ty> == rvv::LMul::kM8) {                      \
    asm volatile("vsetvli %[vlreg], zero, e64, m8, ta, ma"                     \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "memory");                                                  \
  } else {                                                                     \
    static_assert("Unsupported LMUL");                                         \
  }                                                                            \
  static_assert(true, "Require trailing semicolon.")

#define SET_VL_MAX_8_TA_MA(pseudo_output_dep, pseudo_input_dep, vl, ty)        \
  if constexpr (rvv::lmul<ty> == rvv::LMul::kMF2) {                            \
    asm volatile("vsetvli %[vlreg], zero, e8, mf2, ta, ma"                     \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "memory");                                                  \
  } else if constexpr (rvv::lmul<ty> == rvv::LMul::kM1) {                      \
    asm volatile("vsetvli %[vlreg], zero, e8, m1, ta, ma"                      \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "memory");                                                  \
  } else if constexpr (rvv::lmul<ty> == rvv::LMul::kM2) {                      \
    asm volatile("vsetvli %[vlreg], zero, e8, m2, ta, ma"                      \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "memory");                                                  \
  } else if constexpr (rvv::lmul<ty> == rvv::LMul::kM4) {                      \
    asm volatile("vsetvli %[vlreg], zero, e8, m4, ta, ma"                      \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "memory");                                                  \
  } else if constexpr (rvv::lmul<ty> == rvv::LMul::kM8) {                      \
    asm volatile("vsetvli %[vlreg], zero, e8, m8, ta, ma"                      \
                 : EXPAND_LIST(CONS([vlreg] "=r"(vl), pseudo_output_dep))      \
                 : EXPAND_LIST(pseudo_input_dep)                               \
                 : "memory");                                                  \
  } else {                                                                     \
    static_assert("Unsupported LMUL");                                         \
  }                                                                            \
  static_assert(true, "Require trailing semicolon.")

#define SET_VL_MAX_TA_MA(pseudo_output_dep, pseudo_input_dep, vl, ty)          \
  if constexpr (std::is_same_v<rvv::elem_t<ty>, uint8_t> ||                    \
                std::is_same_v<rvv::elem_t<ty>, int8_t>) {                     \
    SET_VL_MAX_8_TA_MA(pseudo_output_dep, pseudo_input_dep, vl, ty);           \
  } else if constexpr (std::is_same_v<rvv::elem_t<ty>, uint64_t> ||            \
                       std::is_same_v<rvv::elem_t<ty>, int64_t>) {             \
    SET_VL_MAX_64_TA_MA(pseudo_output_dep, pseudo_input_dep, vl, ty);          \
  } else {                                                                     \
    static_assert("Unsupported type");                                         \
  }                                                                            \
  static_assert(true, "Require trailing semicolon.")
