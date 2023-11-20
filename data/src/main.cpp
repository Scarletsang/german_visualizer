#include <fstream>
#include <iostream>

#include "MarkdownParser.hpp"

int main(int argc, const char** argv)
{
  if (argc > 2)
  {
    std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
    return EXIT_FAILURE;
  }
  else if (argc == 2)
  {
    if (!freopen(argv[1], "r", stdin))
    {
      std::perror("Error opening file");
      return EXIT_FAILURE;
    }
  }
  MarkdownParser  parser(std::cin);

  if (!parser.parse()
        .chain(&MarkdownParser::print<Json>, parser).is_ok())
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
