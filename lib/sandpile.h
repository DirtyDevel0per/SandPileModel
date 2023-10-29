#pragma once

#include <cstdint>
#include "../parser/parser.h"

struct Field {
  int32_t cur_width = 0;
  int32_t cur_height = 0;
  uint64_t** pixels;

  Field(int32_t x, int32_t y) {
    cur_width = x;
    cur_height = y;
    pixels = new uint64_t*[y];
    for (int i = 0; i < y; ++i)
      pixels[i] = new uint64_t[x]{0};
  }

  ~Field() {
    delete[] pixels;
  }
};

int Main(const CommandLineArguments& args);

Field FileParser(const char* filename);

bool SaveToBmp(const Field& field,  const int64_t& number_of_picture, const char* path_to_dir);

#pragma pack(1)

struct BitmapFileHeader {
  short header = 0x4d42;
  uint32_t size;
  short reserved_1 = 0;
  short reserved_2 = 0;
  int offset;
};

struct DIBHeader {
  int size_of_header = sizeof(DIBHeader);
  int width;
  int height;
  short color_planes = 1;
  short bit_per_pixel = 4;
  int compression = 0;
  int image_size;
  int horizontal_resolution = 0;
  int vertical_resolution = 0;
  int color_palette = 16;
  int important_colors = 5;
};

struct ColorTable {
  uint32_t colors[16]{0x000000};

  ColorTable() {
    colors[0] = 0xFFFFFF;
    colors[1] = 0x00FF00;
    colors[2] = 0xFFD700;
    colors[3] = 0xFF00FF;
  }
};
#pragma pop