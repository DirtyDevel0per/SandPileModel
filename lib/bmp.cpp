#include <fstream>
#include "sandpile.h"


char GetPixel(const Field& field, const int& index, const int& height) {

  char res = 0;

  if (field.pixels[height][index] > 3)
    res += 4;
  if (field.pixels[height][index] == 1)
    res += 1;
  if (field.pixels[height][index] == 2)
    res += 2;
  if (field.pixels[height][index] == 3)
    res += 3;
  res <<= 4;
  if (index + 1 < field.cur_width) {
    if (field.pixels[height][index + 1] > 3)
      res += 4;
    if (field.pixels[height][index + 1] == 1)
      res += 1;
    if (field.pixels[height][index + 1] == 2)
      res += 2;
    if (field.pixels[height][index + 1] == 3)
      res += 3;
  }
  return res;
}
bool SaveToBmp(const Field& field, const int64_t& number_of_picture, const char* path_to_dir) {
  const char* digit = std::to_string(number_of_picture).c_str();
  char* filename = new char[strlen(digit) + 5];
  memcpy(filename, digit, strlen(digit));
  memcpy(filename + strlen(digit), ".bmp", 5);
  std::filesystem::path p{path_to_dir};
  p /= filename;
  std::ofstream f(p, std::ios_base::binary);
  BitmapFileHeader header;
  DIBHeader dib;
  ColorTable palette;

  header.offset = sizeof(DIBHeader) + sizeof(header) + sizeof(ColorTable);
  dib.width =  field.cur_width;
  dib.height = field.cur_height;
  dib.image_size = field.cur_width * dib.height;
  header.size = sizeof(DIBHeader) + sizeof(header) + sizeof(ColorTable) + dib.image_size;

  f.write((char*)(&header), sizeof(header));
  f.write((char*)(&dib), sizeof(dib));
  f.write((char*)(&palette), sizeof(palette));
  for (int h = 0; h < dib.height; ++h) {
    int w = 0;
    int bytes_counter = 0;
    while (w < field.cur_width) {
      f.put(GetPixel(field, w, h));
      w += 2;
      ++bytes_counter;
    }

    while (bytes_counter < ((field.cur_width * 4 + 31) / 32) * 4) {
      f << char(0);
      ++bytes_counter;
    }
  }

  return true;
}