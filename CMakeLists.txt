project(highway-test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(HWY 1.0.7 REQUIRED)

include(CheckCXXSourceCompiles)
check_cxx_source_compiles(
  "int main() {
      #if !defined(__riscv)
      static_assert(false, \"__riscv is not defined\");
      #endif
      return 0;
    }"
  BUILD_RISCV)

set(FLAGS)
if(BUILD_RISCV)
  list(APPEND FLAGS -march=rv64gcv1p0)
  add_subdirectory(rvv-intrinsics-cpp)
endif()

add_executable(main main.cpp)
target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR})
target_compile_options(main PRIVATE ${FLAGS})
target_link_libraries(main PRIVATE hwy rvv)
