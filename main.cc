#include "parsing.hh"

int main(int argc, char* argv[])
{
  std::string_view micro_string{"1585201087123567"}; // 16 chars
  std::cout << micro_string << std::endl;
  std::cout << parse_char_conv(micro_string) << std::endl;
  std::cout << parse_simple(micro_string) << std::endl;
  auto trick_result = parse_trick(micro_string);
  std::cout << std::dec << trick_result << std::endl;
  trick_result = parse_trick_simd(micro_string);
  std::cout << std::dec << trick_result << std::endl;
  std::cout << std::hex << trick_result << std::endl;
  std::cout << std::bitset<64>(trick_result) << std::endl;

  return 0;
}
