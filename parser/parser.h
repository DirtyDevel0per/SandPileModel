#pragma once
#include <iostream>

struct CommandLineArguments {
  const char* input_file_path = "";
  const char* output_file_path = "/Users/nikolaiveselov/OpLabs_1Sem/labwork3-DirtyDevel0per/bmp_files";
  size_t max_iter = SIZE_T_MAX;
  size_t freq = 0;
};

const char* ParserCommandLineArguments(const int& argc, const char** argv, CommandLineArguments& args);

bool CheckParameters(const CommandLineArguments& args);