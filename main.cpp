#include "lt128.h"
#include "min128.h"
#include <cstdio>
#include <ctime>

int main() {
  srand(time(0));
  printf("%lu\n", __riscv_vlenb() * 8);
  using M1Tag = hn::ScalableTag<uint64_t>;
  using M2Tag = hn::Twice<M1Tag>;
  using M4Tag = hn::Twice<M2Tag>;
  using M8Tag = hn::Twice<M4Tag>;
  CheckLt128(Lt128Highway<>, Lt128HighwayAsm<>);
  CheckLt128(Lt128Highway<>, Lt128HighwayAsmImmOddEven<>);
  CheckLt128(Lt128Highway<>, Lt128Manual1Intrinsics<>);
  CheckLt128(Lt128Highway<>, Lt128Manual2Intrinsics<>);
  CheckLt128(Lt128Highway<>, Lt128Manual1<>);
  CheckLt128(Lt128Highway<>, Lt128Manual1OptimizeForX280<>);
  // CheckLt128(Lt128Highway<hn::Twice<hn::ScalableTag<uint64_t>>>,
  //            Lt128Manual1OptimizeForX280<hn::Twice<hn::ScalableTag<uint64_t>>>);
  CheckLt128(Lt128Highway<>, Lt128Manual1OptimizeForP670<>);
  CheckLt128(Lt128Highway<>, Lt128Manual1WithGlobal<>);
  CheckLt128(Lt128Highway<>, Lt128Manual2<>);
  CheckMin128(Min128Highway<>, Min128HighwayAsm<>);
  CheckMin128(Min128Highway<>, Min128HighwayAsmImmOddEven<>);

  CheckLt128Reg<vuint64m1_t>("Highway0", "X280", Lt128Highway0<>,
                             Lt128OptimizeForX280<>);
  CheckLt128Reg<vuint64m2_t>("Highway0", "X280", Lt128Highway0<>,
                             Lt128OptimizeForX280<>);
  CheckLt128Reg<vuint64m4_t>("Highway0", "X280", Lt128Highway0<>,
                             Lt128OptimizeForX280<>);
  CheckLt128Reg<vuint64m8_t>("Highway0", "X280", Lt128Highway0<>,
                             Lt128OptimizeForX280<>);
  CheckLt128Reg<vuint64m1_t>("Highway0", "X280ChangeVType", Lt128Highway0<>,
                             Lt128OptimizeForX280ChangeVType<>);
  CheckLt128Reg<vuint64m2_t>("Highway0", "X280ChangeVType", Lt128Highway0<>,
                             Lt128OptimizeForX280ChangeVType<>);
  CheckLt128Reg<vuint64m4_t>("Highway0", "X280ChangeVType", Lt128Highway0<>,
                             Lt128OptimizeForX280ChangeVType<>);
  CheckLt128Reg<vuint64m8_t>("Highway0", "X280ChangeVType", Lt128Highway0<>,
                             Lt128OptimizeForX280ChangeVType<>);
  CheckLt128Reg<vuint64m1_t>("Highway0", "X280UseShift", Lt128Highway0<>,
                             Lt128OptimizeForX280UseShift<>);
  CheckLt128Reg<vuint64m2_t>("Highway0", "X280UseShift", Lt128Highway0<>,
                             Lt128OptimizeForX280UseShift<>);
  CheckLt128Reg<vuint64m4_t>("Highway0", "X280UseShift", Lt128Highway0<>,
                             Lt128OptimizeForX280UseShift<>);
  CheckLt128Reg<vuint64m8_t>("Highway0", "X280UseShift", Lt128Highway0<>,
                             Lt128OptimizeForX280UseShift<>);
}
