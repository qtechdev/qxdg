#include <iostream>

#include "qxdg.hpp"

int main() {
  qxdg::base base_dirs = qxdg::get_base_directories();

  std::cout << "+----------------------------------------------------------\n";
  std::cout << "| Fetching XDG environment variables\n";
  std::cout << "+----------------------------------------------------------\n";
  std::cout << "| HOME            : " << base_dirs.home << "\n";
  std::cout << "| XDG_DATA_HOME   : " << base_dirs.xdg_data_home << "\n";
  std::cout << "| XDG_CONFIG_HOME : " << base_dirs.xdg_config_home << "\n";
  std::cout << "| XDG_CACHE_HOME  : " << base_dirs.xdg_cache_home << "\n";
  std::cout << "| XDG_RUNTIME_DIR : " << base_dirs.xdg_runtime_dir << "\n";
  std::cout << "|\n";
  std::cout << "| XDG_DATA_DIRS:\n";
  for (const qxdg::path &p : base_dirs.xdg_data_dirs) {
    std::cout << "| -> " << p << "\n";
  }
  std::cout << "|\n";
  std::cout << "| XDG_CONFIG_DIRS:\n";
  for (const qxdg::path &p : base_dirs.xdg_config_dirs) {
    std::cout << "| -> " << p << "\n";
  }
  std::cout << "+----------------------------------------------------------\n";

  return 0;
}
