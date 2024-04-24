#include <iostream>
#include <filesystem>
#include "parser.h"

bool CheckParameters(const CommandLineArguments& args) {
  if (std::filesystem::is_directory(args.output_file_path)
  && !std::filesystem::is_empty(args.input_file_path)
  && args.max_iter >= 0 && args.freq >= 0)
    return true;
  return false;
}

bool ShortCommandParser(const char* argv, const char* value, CommandLineArguments& args) {
  if (value[0] == '-')
    return false;

  if (!std::strcmp(argv, "-i")) {
    args.input_file_path = value;
    return true;
  }
  if (!std::strcmp(argv, "-o")) {
    args.output_file_path = value;
    return true;
  }
  if (!std::strcmp(argv, "-m")) {
    args.max_iter = std::stoi(value);
    return true;
  }
  if (!std::strcmp(argv, "-f")) {
    args.freq = std::stoi(value);
    return true;
  }

  return false;
}

bool LongCommandParser(const char* argv, CommandLineArguments& args) {
  if (!strncmp(argv, "--input=", strlen("--input="))) {
    char* value = new char[strlen(argv) - strlen("--input=")]{'\0'};
    size_t argv_it = strlen("--input=");
    size_t value_it = 0;

    while (argv[argv_it] != '\0') {
      value[value_it] = argv[argv_it];
      ++value_it;
      ++argv_it;
    }

    if (value[0] == '\0')
      return false;

    args.input_file_path = value;

    return true;
  }

  if (!strncmp(argv, "--output=", strlen("--output="))) {
    char* value = new char[strlen(argv) - strlen("--output=")]{'\0'};
    size_t argv_it = strlen("--output=");
    size_t value_it = 0;

    while (argv[argv_it] != '\0' && value_it < strlen(value)) {
      value[value_it] = argv[argv_it];
      ++value_it;
      ++argv_it;
    }

    if (value[0] == '\0')
      return false;

    args.input_file_path = value;

    return true;
  }

  if (!strncmp(argv, "--max-iter=", strlen("--max-iter="))) {
    args.max_iter = std::stoul(argv + strlen("--max-iter="));

    return true;
  }

  if (!strncmp(argv, "--freq=", strlen("--freq="))) {
    args.max_iter = std::stoul(argv + strlen("--freq="));

    return true;
  }

  return false;
}

const char* ParserCommandLineArguments(const int& argc, const char** argv, CommandLineArguments& args) {
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-' && argv[i][1] != '-') {
      if (!ShortCommandParser(argv[i], argv[i + 1], args))
        return "Invalid short command";
      ++i;
    } else if (argv[i][0] == '-' && argv[i][1] == '-') {
      if (!LongCommandParser(argv[i], args))
        return "Invalid long command";
    } else
      return "Invalid command line argument";
  }

  return "";
}