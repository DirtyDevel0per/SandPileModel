#include <iostream>
#include <fstream>
#include "sandpile.h"

void FieldExtension(Field& field) {
  uint32_t it_y = 0;
  uint32_t it_x = 0;
  uint32_t skip_flag_height = 0;
  uint32_t skip_flag_width = 0;
  int32_t new_width = field.cur_width;
  int32_t new_height = field.cur_height;
  for (int i = 0; i < field.cur_height; ++i) {
    if (field.pixels[i][0] >= 4) {
      ++new_width;
      it_x = 1;
      break;
    }
  }
  for (int i = 0; i < field.cur_height; ++i) {
    if (field.pixels[i][field.cur_width - 1] >= 4) {
      ++new_width;
      skip_flag_width = 1;
      break;
    }
  }
  for (int i = 0; i < field.cur_width; ++i) {
    if (field.pixels[0][i] >= 4) {
      ++new_height;
      it_y = 1;
      break;
    }
  }
  for (int i = 0; i < field.cur_width; ++i) {
    if (field.pixels[field.cur_height - 1][i] >= 4) {
      ++new_height;
      skip_flag_height = 1;
      break;
    }
  }

  uint64_t** new_pixels = new uint64_t*[new_height];
  for (int i = 0; i < new_height; ++i)
    new_pixels[i] = new uint64_t[new_width]{0};
  for (int j = it_y, last_y = 0; j < new_height - skip_flag_height; ++j, ++last_y) {
    for (int k = it_x, last_x = 0; k < new_width - skip_flag_width; ++k) {
      new_pixels[j][k] = field.pixels[last_y][last_x];
      ++last_x;
    }
  }

  uint64_t** tmp = field.pixels;
  field.pixels = new_pixels;
  for (int i = 0; i < field.cur_height; ++i)
    delete[] tmp[i];
  delete[] tmp;
  field.cur_width = new_width;
  field.cur_height = new_height;

}
void CellTopple(Field& field, int pos_x, int pos_y) {
    ++field.pixels[pos_y + 1][pos_x];
    ++field.pixels[pos_y - 1][pos_x];
    ++field.pixels[pos_y][pos_x + 1];
    ++field.pixels[pos_y][pos_x - 1];
    field.pixels[pos_y][pos_x] -= 4;
}

bool FieldTopple(Field& field) {
  std::pair<int32_t, int32_t>* cells = new std::pair<int32_t, int32_t>[field.cur_height * field.cur_width];
  int cells_counter = 0;
  bool flag = true;
  FieldExtension(field);

  for (int i = 0; i < field.cur_height; ++i)
    for (int j = 0; j < field.cur_width; ++j)
      if (field.pixels[i][j] >= 4) {
        cells[cells_counter] = {i, j};
        ++cells_counter;
      }
  if (cells_counter == 0)
    flag = false;
  for (int i = 0; i < cells_counter; ++i)
    CellTopple(field, cells[i].second, cells[i].first);
  delete[] cells;
  return flag;
}

int SandPile(const CommandLineArguments& args) {
  Field field = FileParser(args.input_file_path);
  size_t it = 0;
  int64_t number_of_picture = 1;

  while (it < args.max_iter) {
    if (!FieldTopple(field))
      break;
    ++it;
    if (args.freq && it % args.freq == 0) {
      SaveToBmp(field, number_of_picture, args.output_file_path);
      ++number_of_picture;
    }
  }

  if (!args.freq)
    SaveToBmp(field, number_of_picture, args.output_file_path);
  return 0;
}

Field FileParser(const char* filename) {
  std::ifstream f(filename, std::ios_base::binary);
  int16_t max_x = 0;
  int16_t max_y = 0;
  int16_t min_x = 0;
  int16_t min_y = 0;
  char str[30]{'\0'};

  if (!f.is_open())
    exit(EXIT_FAILURE);

  while (f.good()) {
    if (f.peek() == EOF)
      break;
    f.getline(str, 20,'\t');
    int16_t tmp_x = static_cast<int16_t>(std::stoi(str));
    f.getline(str, 20,'\t');
    int16_t tmp_y = static_cast<int16_t>(std::stoll(str));
    if (tmp_x > max_x)
      max_x = tmp_x;
    if (tmp_x < min_x)
      min_x = tmp_x;
    if (tmp_y > max_y)
      max_y = tmp_y;
    if (tmp_y < min_y)
      min_y = tmp_y;
    f.getline(str, 30, '\n');
  }

  Field field(max_x - min_x + 1, max_y - min_y + 1);

  f.seekg(0);
  while (f.good()) {
    if (f.peek() == EOF)
      break;
    f.getline(str, 20,'\t');
    int16_t x = static_cast<int16_t>(std::stoi(str));
    f.getline(str, 20, '\t');
    int16_t y = static_cast<int16_t>(std::stoi(str));
    f.getline(str, 20,'\n');
    uint64_t value = std::stoul(str);
    field.pixels[y - min_y][x - min_x] = value;
  }

  return field;
}

