#include <iostream>
#include <regex>
#include <string>

#include "qxdg.hpp"

int main() {
  qxdg::base base_dirs = qxdg::get_base_directories(true);

  std::string name = "test_program";
  std::regex re(".*");

  auto files = qxdg::search_data_dirs(base_dirs, name, re);

  if (files.size() > 0) {
    for (const auto &file : files) {
      std::cout << file << "\n";
    }
  } else {
    std::cout << "files not found\n";
  }

  return 0;
}
