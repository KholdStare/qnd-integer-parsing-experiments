#include "../parsing.hh"

#include <gtest/gtest.h>

TEST(Parse16Chars, Examples)
{
  EXPECT_EQ(0, parse_16_chars("0000000000000000"));
  EXPECT_EQ(1, parse_16_chars("0000000000000001"));
  EXPECT_EQ(1000000000000000, parse_16_chars("1000000000000000"));
  EXPECT_EQ(9999999999999999, parse_16_chars("9999999999999999"));
  EXPECT_EQ(1234567890123456, parse_16_chars("1234567890123456"));
}

TEST(ParseGeneralTrickSimd, SingleDigit16Chars)
{
  EXPECT_EQ(1, parse_general_trick_simd("1               "));
  EXPECT_EQ(1, parse_general_trick_simd("1///////////////"));
  EXPECT_EQ(1, parse_general_trick_simd("1aaaaaaaaaaaaaaa"));
}

TEST(ParseGeneralTrickSimd, LessThan16Digits)
{
  EXPECT_EQ(1,               parse_general_trick_simd("1               "));
  EXPECT_EQ(12,              parse_general_trick_simd("12              "));
  EXPECT_EQ(123,             parse_general_trick_simd("123             "));
  EXPECT_EQ(1234,            parse_general_trick_simd("1234            "));
  EXPECT_EQ(12345,           parse_general_trick_simd("12345           "));
  EXPECT_EQ(123456,          parse_general_trick_simd("123456          "));
  EXPECT_EQ(1234567,         parse_general_trick_simd("1234567         "));
  EXPECT_EQ(12345678,        parse_general_trick_simd("12345678        "));
  EXPECT_EQ(123456789,       parse_general_trick_simd("123456789       "));
  EXPECT_EQ(1234567890,      parse_general_trick_simd("1234567890      "));
  EXPECT_EQ(12345678901,     parse_general_trick_simd("12345678901     "));
  EXPECT_EQ(123456789012,    parse_general_trick_simd("123456789012    "));
  EXPECT_EQ(1234567890123,   parse_general_trick_simd("1234567890123   "));
  EXPECT_EQ(12345678901234,  parse_general_trick_simd("12345678901234  "));
  EXPECT_EQ(123456789012345, parse_general_trick_simd("123456789012345 "));
}

TEST(ParseGeneralTrickSimd, LessThan16DigitsShortStrings)
{
  EXPECT_EQ(1,               parse_general_trick_simd("1 "));
  EXPECT_EQ(12,              parse_general_trick_simd("12 "));
  EXPECT_EQ(123,             parse_general_trick_simd("123 "));
  EXPECT_EQ(1234,            parse_general_trick_simd("1234 "));
  EXPECT_EQ(12345,           parse_general_trick_simd("12345 "));
  EXPECT_EQ(123456,          parse_general_trick_simd("123456 "));
  EXPECT_EQ(1234567,         parse_general_trick_simd("1234567 "));
  EXPECT_EQ(12345678,        parse_general_trick_simd("12345678 "));
  EXPECT_EQ(123456789,       parse_general_trick_simd("123456789 "));
  EXPECT_EQ(1234567890,      parse_general_trick_simd("1234567890 "));
  EXPECT_EQ(12345678901,     parse_general_trick_simd("12345678901 "));
  EXPECT_EQ(123456789012,    parse_general_trick_simd("123456789012 "));
  EXPECT_EQ(1234567890123,   parse_general_trick_simd("1234567890123 "));
  EXPECT_EQ(12345678901234,  parse_general_trick_simd("12345678901234 "));
  EXPECT_EQ(123456789012345, parse_general_trick_simd("123456789012345"));
}

TEST(ParseGeneralTrickSimd, MultipleNumbersInString)
{
  EXPECT_EQ(1,               parse_general_trick_simd("1, 2, 3, 4, 5, 6"));
  EXPECT_EQ(12,              parse_general_trick_simd("12, 34, 56, 78, "));
  EXPECT_EQ(123,             parse_general_trick_simd("123, 456, 789, 0"));
  EXPECT_EQ(1234,            parse_general_trick_simd("1234, 5678, 9012"));
  EXPECT_EQ(12345,           parse_general_trick_simd("12345, 67890, 12"));
  EXPECT_EQ(123456,          parse_general_trick_simd("123456, 789012, "));
  EXPECT_EQ(1234567,         parse_general_trick_simd("1234567, 8901234"));
  EXPECT_EQ(12345678,        parse_general_trick_simd("12345678, 901234"));
  EXPECT_EQ(123456789,       parse_general_trick_simd("123456789, 01234"));
  EXPECT_EQ(1234567890,      parse_general_trick_simd("1234567890, 1234"));
  EXPECT_EQ(12345678901,     parse_general_trick_simd("12345678901, 123"));
  EXPECT_EQ(123456789012,    parse_general_trick_simd("123456789012, 12"));
  EXPECT_EQ(1234567890123,   parse_general_trick_simd("1234567890123, 1"));
  EXPECT_EQ(12345678901234,  parse_general_trick_simd("12345678901234, "));
  EXPECT_EQ(123456789012345, parse_general_trick_simd("123456789012345 "));
}

TEST(ParseGeneralTrickSimd, All16Chars)
{
  EXPECT_EQ(0, parse_general_trick_simd("0000000000000000"));
  EXPECT_EQ(1, parse_general_trick_simd("0000000000000001"));
  EXPECT_EQ(1000000000000000, parse_general_trick_simd("1000000000000000"));
  EXPECT_EQ(9999999999999999, parse_general_trick_simd("9999999999999999"));
  EXPECT_EQ(1234567890123456, parse_general_trick_simd("1234567890123456"));
}
