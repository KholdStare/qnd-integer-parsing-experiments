#pragma once
#include <iostream>
#include <charconv>
#include <cstring>
#include <ios>
#include <bitset>
#include <immintrin.h>

inline std::uint64_t parse_char_conv(std::string_view s) noexcept
{
  std::uint64_t result = 0;
  std::from_chars(s.data(), s.data() + s.size(), result);
  return result;
}

inline std::uint64_t parse_simple(std::string_view s) noexcept
{
  const char* cursor = s.data();
  const char* last = s.data() + s.size();
  std::uint64_t result = 0;
  for(; cursor != last; ++ cursor)
  {
    result *= 10;
    result += *cursor - '0';
  }
  return result;
}

inline std::uint64_t parse_unrolled(std::string_view s) noexcept
{
  std::uint64_t result = 0;

  result += (s[0] - '0') * 1000000000000000ULL;
  result += (s[1] - '0') * 100000000000000ULL;
  result += (s[2] - '0') * 10000000000000ULL;
  result += (s[3] - '0') * 1000000000000ULL;
  result += (s[4] - '0') * 100000000000ULL;
  result += (s[5] - '0') * 10000000000ULL;
  result += (s[6] - '0') * 1000000000ULL;
  result += (s[7] - '0') * 100000000ULL;
  result += (s[8] - '0') * 10000000ULL;
  result += (s[9] - '0') * 1000000ULL;
  result += (s[10] - '0') * 100000ULL;
  result += (s[11] - '0') * 10000ULL;
  result += (s[12] - '0') * 1000ULL;
  result += (s[13] - '0') * 100ULL;
  result += (s[14] - '0') * 10ULL;
  result += (s[15] - '0');

  return result;
}

template <typename T>
inline T get_baseline() noexcept;

template <>
inline std::uint64_t get_baseline<std::uint64_t>() noexcept
{
  std::uint64_t result = 0;
  constexpr char zeros[] = "00000000";
  std::memcpy(&result, zeros, sizeof(result));
  return result;
}

template <>
inline __m128i get_baseline<__m128i>() noexcept
{
  __m128i result = {0, 0};
  constexpr char zeros[] = "0000000000000000";
  std::memcpy(&result, zeros, sizeof(result));
  return result;
}

template <typename T>
inline T byteswap(T a) noexcept;

template <>
inline std::uint64_t byteswap(std::uint64_t a) noexcept
{
  return __builtin_bswap64(a);
}

template <>
inline __m128i byteswap(__m128i a) noexcept
{
  const auto mask = _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  return _mm_shuffle_epi8(a, mask);
}

inline std::uint64_t parse_16_chars(const char* string) noexcept
{
  using T = __m128i;
  T chunk = {0, 0};
  std::memcpy(&chunk, string, sizeof(chunk));
  chunk = byteswap(chunk - get_baseline<T>());

  {
    const auto mult = _mm_set_epi8(10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1);
    chunk = _mm_maddubs_epi16(chunk, mult);
  }
  {
    const auto mult = _mm_set_epi16(100, 1, 100, 1, 100, 1, 100, 1);
    chunk = _mm_madd_epi16(chunk, mult);
  }
  {
    const auto mult = _mm_set_epi32(10000, 1, 10000, 1);
    auto multiplied = _mm_mullo_epi32(chunk, mult);
    const __m128i zero = {0, 0};
    chunk = _mm_hadd_epi32(multiplied, zero);
  }

  return ((chunk[0] >> 32) * 100000000) + (chunk[0] & 0xffffffff);
}

inline std::uint64_t parse_8_chars(const char* string) noexcept
{
  std::uint64_t chunk = 0;
  std::memcpy(&chunk, string, sizeof(chunk));
  chunk = __builtin_bswap64(chunk - get_baseline<std::uint64_t>());

  //step 1
  std::uint64_t lower_digits = chunk & 0x000f000f000f000f;
  std::uint64_t upper_digits = ((chunk & 0x0f000f000f000f00) >> 7) * 5;
  chunk = lower_digits + upper_digits;

  // step 2
  lower_digits = chunk & 0x000000ff000000ff;
  upper_digits = ((chunk & 0x00ff000000ff0000) >> 14) * 25;
  chunk = lower_digits + upper_digits;

  // step 3
  lower_digits = chunk & 0x000000000000ffff;
  upper_digits = ((chunk & 0x0000ffff00000000) >> 28) * 625;
  chunk = lower_digits + upper_digits;

  return chunk;
}

inline std::uint64_t parse_trick(std::string_view s) noexcept
{
  std::uint64_t upper_digits = parse_8_chars(s.data());
  std::uint64_t lower_digits = parse_8_chars(s.data() + 8);
  return upper_digits * 100000000 + lower_digits;
}

inline std::uint64_t parse_trick_simd(std::string_view s) noexcept
{
  return parse_16_chars(s.data());
}

extern const char* example_timestamp;
