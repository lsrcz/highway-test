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

  CheckLt128RegAllTypes<Lt128Highway0Obj, Lt128HighwayClang19Obj<false>,
                        vuint64m1_t, vuint64m2_t, vuint64m4_t, vuint64m8_t>();
  CheckLt128RegAllTypes<Lt128Highway0Obj, Lt128HighwayManualScheduleObj<false>,
                        vuint64m1_t, vuint64m2_t, vuint64m4_t, vuint64m8_t>();
  CheckLt128RegAllTypes<Lt128Highway0Obj, Lt128OptimizeForX280Obj<true, false>,
                        vuint64m1_t, vuint64m2_t, vuint64m4_t, vuint64m8_t>();
  CheckLt128RegAllTypes<Lt128Highway0Obj, Lt128OptimizeForX280Obj<false, false>,
                        vuint64m1_t, vuint64m2_t, vuint64m4_t, vuint64m8_t>();
  CheckLt128RegAllTypes<Lt128Highway0Obj,
                        Lt128OptimizeForX280ChangeVTypeObj<true, false>,
                        vuint64m1_t, vuint64m2_t, vuint64m4_t, vuint64m8_t>();
  CheckLt128RegAllTypes<Lt128Highway0Obj,
                        Lt128OptimizeForX280ChangeVTypeObj<false, false>,
                        vuint64m1_t, vuint64m2_t, vuint64m4_t, vuint64m8_t>();
  CheckLt128RegAllTypes<Lt128Highway0Obj,
                        Lt128OptimizeForX280UseShiftObj<true, false>,
                        vuint64m1_t, vuint64m2_t, vuint64m4_t, vuint64m8_t>();
  CheckLt128RegAllTypes<Lt128Highway0Obj,
                        Lt128OptimizeForX280UseShiftObj<false, false>,
                        vuint64m1_t, vuint64m2_t, vuint64m4_t, vuint64m8_t>();

  uint64_t data[1024];
  // List all the types in the template argument.
  RunAndPrintCyclesObjAllTemplatesTypes<false, vuint64m1_t, vuint64m2_t,
                                        vuint64m4_t, vuint64m8_t>(data,
                                                                  data + 1);
}
