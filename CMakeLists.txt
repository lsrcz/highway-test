project(highway-test CXX)

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
  list(
    APPEND
    FLAGS
    -march=rv64gcv1p0
    -O3
    -mcpu=sifive-x280
    -mllvm
    -enable-misched
    -save-temps)
  add_subdirectory(rvv-intrinsics-cpp)
endif()

include_directories(${PROJECT_SOURCE_DIR})

file(GLOB_RECURSE LT128_SOURCES "lt128/*.cpp")
add_library(lt128 STATIC ${LT128_SOURCES})
target_link_libraries(lt128 PRIVATE rvv)
target_compile_options(lt128 PRIVATE ${FLAGS})

file(GLOB_RECURSE MIN128_SOURCES "min128/*.cpp")
add_library(min128 STATIC ${MIN128_SOURCES})
target_link_libraries(min128 PRIVATE rvv)
target_compile_options(min128 PRIVATE ${FLAGS})

add_executable(main main.cpp)
target_include_directories(main PRIVATE lt128 min128)
target_compile_options(main PRIVATE ${FLAGS})
target_link_libraries(main PRIVATE lt128 min128 hwy -static)
if(BUILD_RISCV)
  target_link_libraries(main PRIVATE rvv)
endif()
