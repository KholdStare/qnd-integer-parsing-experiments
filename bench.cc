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

static void BM_charconv_range(benchmark::State& state) {
  char buffer[32] = {0,};
  std::sprintf(buffer, "%ld", state.range(0));
  const char* last = buffer + sizeof(buffer);
  for (auto _ : state) {
    std::uint64_t result = 0;
    std::from_chars(buffer, last, result);
    benchmark::DoNotOptimize(result);
  }
}

static void BM_general_trick_simd_range(benchmark::State& state) {
  char buffer[32] = {0,};
  std::sprintf(buffer, "%ld", state.range(0));
  const char* last = buffer + sizeof(buffer);
  for (auto _ : state) {
    std::uint64_t result = 0;
    from_chars(buffer, last, result);
    benchmark::DoNotOptimize(result);
  }
}

static void BM_charconv_short_string_range(benchmark::State& state) {
  auto buffer = std::to_string(state.range(0));
  const char* first = buffer.data();
  const char* last = first + buffer.size();
  for (auto _ : state) {
    std::uint64_t result = 0;
    std::from_chars(first, last, result);
    benchmark::DoNotOptimize(result);
  }
}

static void BM_general_trick_simd_short_string_range(benchmark::State& state) {
  auto buffer = std::to_string(state.range(0));
  const char* first = buffer.data();
  const char* last = first + buffer.size();
  for (auto _ : state) {
    std::uint64_t result = 0;
    from_chars(first, last, result);
    benchmark::DoNotOptimize(result);
  }
}

// from_chars benchmarks
BENCHMARK(BM_charconv_range)->Range(1ull, 9999999999999999ull);
BENCHMARK(BM_general_trick_simd_range)->Range(1ull, 9999999999999999ull);
BENCHMARK(BM_charconv_short_string_range)->Range(1ull, 999999999999999ull);
BENCHMARK(BM_general_trick_simd_short_string_range)->Range(1ull, 999999999999999ull);

static void BM_tzcnt(benchmark::State& state) {
  const volatile std::uint64_t value = 0x00000fffffffffff;
  for (auto _ : state) {
    benchmark::DoNotOptimize(__tzcnt_u64(~value));
  }
}

static void BM_get_digit_count_from_numeric_mask(benchmark::State& state) {
  const volatile __m128i value = _mm_set1_epi8(0xff);

  for (auto _ : state) {
    auto result = detail::get_digit_count_from_numeric_mask(value);
    benchmark::DoNotOptimize(result);
  }
}

static void BM_shift_bytes_left(benchmark::State& state) {
  std::uint64_t num_bytes = state.range(0);
  const volatile __m128i value = _mm_set1_epi8(0xff);

  for (auto _ : state) {
    __m128i result = detail::shift_bytes_left(value, num_bytes);
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK(BM_tzcnt);
BENCHMARK(BM_get_digit_count_from_numeric_mask);
BENCHMARK(BM_shift_bytes_left)->DenseRange(0, 16, 1);

BENCHMARK_MAIN();
