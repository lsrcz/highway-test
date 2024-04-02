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
  CheckLt128Reg<M1Tag>("Highway0", "X280", Lt128Highway0<>,
                       Lt128OptimizeForX280<>);
  CheckLt128Reg<M2Tag>("Highway0", "X280", Lt128Highway0<M2Tag>,
                       Lt128OptimizeForX280<M2Tag>);
  CheckLt128Reg<M4Tag>("Highway0", "X280", Lt128Highway0<M4Tag>,
                       Lt128OptimizeForX280<M4Tag>);
  CheckLt128Reg<M8Tag>("Highway0", "X280", Lt128Highway0<M8Tag>,
                       Lt128OptimizeForX280<M8Tag>);
  CheckLt128Reg<M1Tag>("Highway0", "X280ChangeVType", Lt128Highway0<>,
                       Lt128OptimizeForX280ChangeVType<>);
  CheckLt128Reg<M2Tag>("Highway0", "X280ChangeVType", Lt128Highway0<M2Tag>,
                       Lt128OptimizeForX280ChangeVType<M2Tag>);
  CheckLt128Reg<M4Tag>("Highway0", "X280ChangeVType", Lt128Highway0<M4Tag>,
                       Lt128OptimizeForX280ChangeVType<M4Tag>);
  CheckLt128Reg<M8Tag>("Highway0", "X280ChangeVType", Lt128Highway0<M8Tag>,
                       Lt128OptimizeForX280ChangeVType<M8Tag>);
  CheckLt128Reg<M1Tag>("Highway0", "X280UseShift", Lt128Highway0<>,
                       Lt128OptimizeForX280UseShift<>);
  CheckLt128Reg<M2Tag>("Highway0", "X280UseShift", Lt128Highway0<M2Tag>,
                       Lt128OptimizeForX280UseShift<M2Tag>);
  CheckLt128Reg<M4Tag>("Highway0", "X280UseShift", Lt128Highway0<M4Tag>,
                       Lt128OptimizeForX280UseShift<M4Tag>);
  CheckLt128Reg<M8Tag>("Highway0", "X280UseShift", Lt128Highway0<M8Tag>,
                       Lt128OptimizeForX280UseShift<M8Tag>);
}
