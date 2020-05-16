cmake_minimum_required(VERSION 3.16)
project(integer_parsing)

add_library(
  parsing_lib
  parsing.cc
)
set(EXTRA_FLAGS -O3 -std=c++2a -march=native -Wall -Werror)
target_compile_options(
  parsing_lib
  PUBLIC ${EXTRA_FLAGS}
)

add_executable(
  main
  main.cc
)
target_link_libraries(main PRIVATE parsing_lib)

find_package(benchmark REQUIRED)
add_executable(
  bench
  bench.cc
)
target_link_libraries(bench PRIVATE parsing_lib benchmark::benchmark)
