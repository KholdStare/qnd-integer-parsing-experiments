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

TEST(ParseGeneralTrickSimd, All16Chars)
{
  EXPECT_EQ(0, parse_general_trick_simd("0000000000000000"));
  EXPECT_EQ(1, parse_general_trick_simd("0000000000000001"));
  EXPECT_EQ(1000000000000000, parse_general_trick_simd("1000000000000000"));
  EXPECT_EQ(9999999999999999, parse_general_trick_simd("9999999999999999"));
  EXPECT_EQ(1234567890123456, parse_general_trick_simd("1234567890123456"));
}