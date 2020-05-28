#include "parsing.hh"

#include <sstream>
#include <benchmark/benchmark.h>

std::string_view example_stringview{example_timestamp, 16};

static void BM_mov(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(1585201087123789);
  }
}

static void BM_naive(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(parse_naive(example_stringview));
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

#ifdef QND_TEST_BOOST
static void BM_boost_spirit(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(parse_qi(example_stringview));
  }
}
#endif

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


BENCHMARK(BM_mov);
BENCHMARK(BM_atoll);
BENCHMARK(BM_sstream);
BENCHMARK(BM_charconv);
#ifdef QND_TEST_BOOST
BENCHMARK(BM_boost_spirit);
#endif
BENCHMARK(BM_naive);
BENCHMARK(BM_unrolled);
BENCHMARK(BM_trick);
BENCHMARK(BM_trick_simd);

BENCHMARK_MAIN();
