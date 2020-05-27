#pragma once
#include <iostream>
#include <charconv>
#include <cstring>
#include <ios>
#include <bitset>
#include <boost/spirit/include/qi.hpp>
#include <immintrin.h>

inline std::uint64_t parse_char_conv(std::string_view s) noexcept
{
  std::uint64_t result = 0;
  std::from_chars(s.data(), s.data() + s.size(), result);
  return result;
}

inline std::uint64_t parse_qi(std::string_view s) noexcept
{
  using boost::spirit::qi::parse;

  std::uint64_t result = 0;
  parse(s.data(), s.data() + s.size(), result);
  return result;
}

inline std::uint64_t parse_naive(std::string_view s) noexcept
{
  std::uint64_t result = 0;
  for(char digit : s)
  {
    result *= 10;
    result += digit - '0';
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
inline T get_zeros_string() noexcept;

template <>
inline __m128i get_zeros_string<__m128i>() noexcept
{
  __m128i result = {0, 0};
  constexpr char zeros[] = "0000000000000000";
  std::memcpy(&result, zeros, sizeof(result));
  return result;
}

inline std::uint64_t parse_16_chars(const char* string) noexcept
{
  using T = __m128i;
  T chunk = {0, 0};
  std::memcpy(&chunk, string, sizeof(chunk));
  chunk = chunk - get_zeros_string<T>();

  {
    const auto mult = _mm_set_epi8(1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 10);
    chunk = _mm_maddubs_epi16(chunk, mult);
  }
  {
    const auto mult = _mm_set_epi16(1, 100, 1, 100, 1, 100, 1, 100);
    chunk = _mm_madd_epi16(chunk, mult);
  }
  {
    chunk = _mm_packus_epi32(chunk, chunk);
    const auto mult = _mm_set_epi16(0, 0, 0, 0, 1, 10000, 1, 10000);
    chunk = _mm_madd_epi16(chunk, mult);
  }

  return ((chunk[0] & 0xffffffff) * 100000000) + (chunk[0] >> 32);
}

inline std::uint64_t parse_8_chars(const char* string) noexcept
{
  std::uint64_t chunk = 0;
  std::memcpy(&chunk, string, sizeof(chunk));

  // 1-byte mask trick (works on 4 pairs of single digits)
  std::uint64_t lower_digits = (chunk & 0x0f000f000f000f00) >> 8;
  std::uint64_t upper_digits = (chunk & 0x000f000f000f000f) * 10;
  chunk = lower_digits + upper_digits;

  // 2-byte mask trick (works on 2 pairs of two digits)
  lower_digits = (chunk & 0x00ff000000ff0000) >> 16;
  upper_digits = (chunk & 0x000000ff000000ff) * 100;
  chunk = lower_digits + upper_digits;

  // 4-byte mask trick (works on pair of four digits)
  lower_digits = (chunk & 0x0000ffff00000000) >> 32;
  upper_digits = (chunk & 0x000000000000ffff) * 10000;
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
