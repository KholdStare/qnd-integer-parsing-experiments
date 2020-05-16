#include "parsing.hh"

#include <sstream>
#include <benchmark/benchmark.h>

std::string_view example_stringview{example_timestamp, 16};

static void BM_simple(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(parse_simple(example_stringview));
  }
}

static void BM_atoll(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(std::atoll(example_timestamp));
  }
}

static void BM_sstream(benchmark::State& state) {
  std::stringstream s(example_timestamp);
  for (auto _ : state) {
    s.seekg(0);
    std::uint64_t i = 0;
    s >> i;
    benchmark::DoNotOptimize(i);
    // TODO: reset
  }
}
static void BM_charconv(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(parse_char_conv(example_stringview));
  }
}

static void BM_unrolled(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(parse_unrolled(example_stringview));
  }
}

static void BM_trick(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(parse_trick(example_stringview));
  }
}

static void BM_trick_simd(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(parse_trick_simd(example_stringview));
  }
}


BENCHMARK(BM_simple);
BENCHMARK(BM_atoll);
BENCHMARK(BM_charconv);
BENCHMARK(BM_sstream);
BENCHMARK(BM_unrolled);
BENCHMARK(BM_trick);
BENCHMARK(BM_trick_simd);

BENCHMARK_MAIN();
