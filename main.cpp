#include "lt128.h"
#include "min128.h"
#include <cstdio>
#include <ctime>

int main() {
  srand(time(0));
  printf("%lu\n", __riscv_vlenb() * 8);
  CheckLt128(Lt128Highway<>, Lt128HighwayAsm<>);
  CheckLt128(Lt128Highway<>, Lt128HighwayAsmImmOddEven<>);
  CheckLt128(Lt128Highway<>, Lt128Manual1Intrinsics<>);
  CheckLt128(Lt128Highway<>, Lt128Manual2Intrinsics<>);
  CheckLt128(Lt128Highway<>, Lt128Manual1<>);
  CheckLt128(Lt128Highway<>, Lt128Manual1OptimizeForX280<>);
  CheckLt128(Lt128Highway<>, Lt128Manual1OptimizeForP670<>);
  CheckLt128(Lt128Highway<>, Lt128Manual1WithGlobal<>);
  CheckLt128(Lt128Highway<>, Lt128Manual2<>);
  CheckMin128(Min128Highway<>, Min128HighwayAsm<>);
  CheckMin128(Min128Highway<>, Min128HighwayAsmImmOddEven<>);
}
