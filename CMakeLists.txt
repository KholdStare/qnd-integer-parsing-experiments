cmake_minimum_required(VERSION 3.16)
project(integer_parsing)

add_executable(
  main
  main.cc
)
set(EXTRA_FLAGS -O3 -std=c++2a -march=native -Wall -Werror)
target_compile_options(
  main
  PRIVATE ${EXTRA_FLAGS}
)

find_package(benchmark REQUIRED)
add_executable(
  bench
  bench.cc
)
target_link_libraries(bench PRIVATE benchmark::benchmark)
target_compile_options(
  bench
  PRIVATE ${EXTRA_FLAGS}
)
