#include "parsing.hh"

#include <sstream>
#include <benchmark/benchmark.h>

static void BM_trick_simd(benchmark::State& state) {
  volatile const char example[] = "1585201087123789";
  for (auto _ : state) {
    std::string_view s{const_cast<const char*>(example), sizeof(example) - 1};
    benchmark::DoNotOptimize(parse_trick_simd(s));
  }
}

static void BM_trick(benchmark::State& state) {
  volatile const char example[] = "1585201087123789";
  for (auto _ : state) {
    std::string_view s{const_cast<const char*>(example), sizeof(example) - 1};
    benchmark::DoNotOptimize(parse_trick(s));
  }
}

static void BM_sstream(benchmark::State& state) {
  volatile const char example[] = "1585201087123789";
  std::stringstream s(const_cast<const char*>(example));
  for (auto _ : state) {
    std::uint64_t i = 0;
    s >> i;
    benchmark::DoNotOptimize(i);
  }
}

static void BM_charconv(benchmark::State& state) {
  volatile const char example[] = "1585201087123789";
  for (auto _ : state) {
    std::string_view s{const_cast<const char*>(example), sizeof(example) - 1};
    benchmark::DoNotOptimize(parse_char_conv(s));
  }
}

static void BM_simple(benchmark::State& state) {
  volatile const char example[] = "1585201087123789";
  for (auto _ : state) {
    std::string_view s{const_cast<const char*>(example), sizeof(example) - 1};
    benchmark::DoNotOptimize(parse_simple(s));
  }
}

BENCHMARK(BM_simple);
BENCHMARK(BM_charconv);
BENCHMARK(BM_trick);
BENCHMARK(BM_trick_simd);
BENCHMARK(BM_sstream);

BENCHMARK_MAIN();
