#include <iostream>
#include <string>

#include "qxdg.hpp"

int main() {
  qxdg::base base_dirs = qxdg::get_base_directories(true);

  std::string name = "test_program";
  qxdg::path file_path = "test_file.txt";

  auto path = qxdg::get_data_path(base_dirs, name, file_path, false, false);

  if (path) {
    std::cout << *path << "\n";
  } else {
    std::cout << "file not found: " << file_path << "\n";
  }

  return 0;
}
