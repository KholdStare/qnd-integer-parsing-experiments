#pragma once
#include <iostream>
#include <iomanip>
#include <charconv>
#include <cstring>
#include <ios>
#include <bitset>
#ifdef QND_TEST_BOOST
  #include <boost/spirit/include/qi.hpp>
#endif
#include <immintrin.h>

inline std::uint64_t parse_char_conv(std::string_view s) noexcept
{
  std::uint64_t result = 0;
  std::from_chars(s.data(), s.data() + s.size(), result);
  return result;
}

#ifdef QND_TEST_BOOST
inline std::uint64_t parse_qi(std::string_view s) noexcept
{
  using boost::spirit::qi::parse;

  std::uint64_t result = 0;
  parse(s.data(), s.data() + s.size(), result);
  return result;
}
#endif

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

inline std::uint64_t assemble_from_128i(__m128i chunk) noexcept
{
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

inline std::uint64_t parse_16_chars(const char* string) noexcept
{
  auto chunk = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(string));
  auto zeros =  _mm_set1_epi8('0');
  chunk = chunk - zeros;
  return assemble_from_128i(chunk);
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

template <typename T>
inline void print_bits(T v)
{
  std::cout << std::setfill('0') << std::setw(sizeof(v) * 2) << std::hex << v
    << " " << std::bitset<sizeof(v) * 8>(v) << std::endl;
}

inline __m128i get_numeric_mask(__m128i chunk)
{
  const auto wrap = _mm_set1_epi8(-128);
  const auto digit_upper_bound = _mm_set1_epi8(10) + wrap;
  return _mm_cmplt_epi8(chunk + wrap, digit_upper_bound);
}

inline std::uint64_t get_digit_count_from_numeric_mask(__m128i mask)
{
  // not using SSE sucks
  auto lower_digits_count = __tzcnt_u64(~mask[0]);
  auto upper_digits_count = __tzcnt_u64(~mask[1]);
  const bool lower_has_all_digits = lower_digits_count == (sizeof(mask[0]) * 8);
  upper_digits_count *= lower_has_all_digits;
  return (lower_digits_count + upper_digits_count) >> 3;
}

inline __m128i shift_bytes_left(__m128i a, std::uint64_t num_bytes)
{
  // branching sucks
  if (num_bytes >= 8)
  {
    auto shifted = _mm_slli_si128(a, 8);
    return _mm_slli_epi64(shifted, (num_bytes - 8) * 8);
  }
  else
  {
    auto partial_result = _mm_slli_epi64(a, num_bytes * 8);
    auto overlay = _mm_srli_epi64(a, (8 - num_bytes) * 8);
    overlay = _mm_slli_si128(overlay, 8);
    return _mm_or_si128(partial_result, overlay);
  }
}

inline std::from_chars_result from_chars(const char* first, const char* last, std::uint64_t& value)
{
  auto chunk = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(first));
  const auto zeros = _mm_set1_epi8('0');
  chunk = chunk - zeros;

  const auto numeric_mask = get_numeric_mask(chunk);
  const auto num_digits = get_digit_count_from_numeric_mask(numeric_mask);

  chunk = shift_bytes_left(chunk, sizeof(chunk) - num_digits);

  value = assemble_from_128i(chunk);
  return {first + num_digits}; // TODO: return an error code
}

inline std::uint64_t parse_general_trick_simd(std::string_view s) noexcept
{
  std::uint64_t result = 0;
  from_chars(s.data(), s.data() + s.size(), result);
  return result;
}

extern const char* example_timestamp;
