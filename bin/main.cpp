#include <lib/sandpile.h>
#include <parser/parser.h>


int main(int argc, const char** argv) {
  CommandLineArguments args;
  const char* result_of_parsing = ParserCommandLineArguments(argc, argv, args);

  if (strcmp(result_of_parsing, "") != 0) {
    std::cerr << result_of_parsing;
    exit(EXIT_FAILURE);
  }

  if (!CheckParameters(args)) {
    std::cerr << "Wrong arguments/parameters";
    exit(EXIT_FAILURE);
  }
  Main(args);
  return 0;
}