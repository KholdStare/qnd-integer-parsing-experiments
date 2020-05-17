#include "parsing.hh"

int main(int argc, char* argv[])
{
  std::string_view nano_string{"1585201087123567"}; // 16 chars
  std::cout << "string: " << nano_string << std::endl;
  std::cout << "charconv: " << parse_char_conv(nano_string) << std::endl;
  std::cout << "naive: " << parse_naive(nano_string) << std::endl;
  auto trick_result = parse_trick(nano_string);
  std::cout << "trick: " << trick_result << std::endl;
  trick_result = parse_trick_simd(nano_string);
  std::cout << "trick_simd: " << trick_result << std::endl;

  return 0;
}
