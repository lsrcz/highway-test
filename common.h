#include <hwy/highway.h>
#include <rvv/type.h>

#define LLVM_MCA(directive, name, ty)                                          \
  if constexpr (rvv::lmul<ty> == rvv::LMul::kM1) {                             \
    asm volatile("# " directive " " name "-lmul1" ::: "memory");               \
  } else if constexpr (rvv::lmul<ty> == rvv::LMul::kM4) {                      \
    asm volatile("# " directive " " name "-lmul4" ::: "memory");               \
  } else {                                                                     \
    static_assert("Unsupported LMUL");                                         \
  }                                                                            \
  static_assert(true, "Require trailing semicolon.")

#define LLVM_MCA_BEGIN(name, ty) LLVM_MCA("LLVM-MCA-BEGIN", name, ty)
#define LLVM_MCA_END(name, ty) LLVM_MCA("LLVM-MCA-END", name, ty)
