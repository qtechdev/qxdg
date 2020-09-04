#include <iostream>
#include <fstream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include <qfio/qfio.hpp>

#include "qxdg.hpp"

std::vector<qxdg::path> split_dirs(std::string s) {
  std::vector<qxdg::path> dirs;

  std::size_t pos = 0;
  std::string dir;

  while ((pos = s.find(":")) != std::string::npos) {
    dir = s.substr(0, pos);
    dirs.push_back(dir);
    s.erase(0, pos + 1);
  }

  dirs.push_back(s);

  return dirs;
}

qxdg::base qxdg::get_base_directories(bool include_local) {
  base base_dirs;
  base_dirs.home = std::getenv("HOME");

  char *xdg_data_home = std::getenv("XDG_DATA_HOME");
  if (xdg_data_home == nullptr) {
    base_dirs.xdg_data_home = base_dirs.home / ".local" / "share";
  } else {
    base_dirs.xdg_data_home = xdg_data_home;
  }

  char *xdg_config_home = std::getenv("XDG_CONFIG_HOME");
  if (xdg_config_home == nullptr) {
    base_dirs.xdg_config_home = base_dirs.home / ".config";
  } else {
    base_dirs.xdg_config_home = xdg_config_home;
  }

  char *xdg_data_dirs = std::getenv("XDG_DATA_DIRS");
  if (xdg_data_dirs == nullptr) {
    base_dirs.xdg_data_dirs = {"/usr/local/share", "/usr/share"};
  } else {
    base_dirs.xdg_data_dirs = split_dirs(xdg_data_dirs);
  }

  char *xdg_config_dirs = std::getenv("XDG_CONFIG_DIRS");
  if (xdg_config_dirs == nullptr) {
    base_dirs.xdg_config_dirs = {"/etc/xdg"};
  } else {
    base_dirs.xdg_config_dirs = split_dirs(xdg_config_dirs);
  }

  if (include_local) {
    base_dirs.xdg_data_dirs.push_back("./data");
    base_dirs.xdg_config_dirs.push_back("./config");
  }

  char *xdg_cache_home = std::getenv("XDG_CACHE_HOME");
  if (xdg_cache_home == nullptr) {
    base_dirs.xdg_cache_home = base_dirs.home / ".cache";
  } else {
    base_dirs.xdg_cache_home = xdg_cache_home;
  }

  char *xdg_runtime_dir = std::getenv("XDG_RUNTIME_DIR");
  if (xdg_runtime_dir == nullptr) {
    throw std::runtime_error("XDG_RUNTIME_DIR not found");
  } else {
    // assume directory is valid
    base_dirs.xdg_runtime_dir = xdg_runtime_dir;
  }

  return base_dirs;
}

std::optional<qxdg::path> qxdg::get_path(
  const path &xdg_home, const std::vector<path> &xdg_dirs,
  const std::string &name, const path &p,
  const bool create, const bool force_home
) {
  path home_path = xdg_home / name / p;
  if (fs::is_regular_file(home_path)) {
    return home_path;
  }

  if (!force_home) {
    for (const auto &dir : xdg_dirs) {
      path system_path = dir / name / p;
      if (fs::is_regular_file(system_path)) {
        return system_path;
      }
    }
  }

  if (create) {
    if (!fs::exists(home_path)) {
      fs::create_directories(home_path.parent_path());
      std::ofstream home_path;
    }

    return home_path;
  }

  return {};
}

std::optional<qxdg::path> qxdg::get_data_path(
  const base &b, const std::string &name, const path &p,
  const bool create, const bool force_home
) {
  return get_path(
    b.xdg_data_home, b.xdg_data_dirs, name, p, create, force_home
  );
}

std::optional<qxdg::path> qxdg::get_config_path(
  const base &b, const std::string &name, const path &p,
  const bool create, const bool force_home
) {
  return get_path(
    b.xdg_config_home, b.xdg_config_dirs, name, p, create, force_home
  );
}

std::vector<qxdg::path> qxdg::search_dirs(
  const path &xdg_home, const std::vector<path> &xdg_dirs,
  const std::string &name, const std::regex &re
) {
  std::vector<path> tmp_dirs;
  tmp_dirs.push_back(xdg_home / name);
  for (const auto &p : xdg_dirs) {
    tmp_dirs.push_back(p / name);
  }

  std::vector<path> tmp_files;
  for (const auto &d : tmp_dirs) {
    for (const auto &f : qfio::get_files_in_directory(d)) {
      tmp_files.push_back(f);
    }
  }

  std::vector<path> files;
  for (const auto &p : tmp_files) {
    if (std::regex_search(p.string(), re)) {
      files.push_back(p);
    }
  }

  return files;
}

std::vector<qxdg::path> qxdg::search_data_dirs(
  const base &b, const std::string &name, const std::regex &re
) {
  return search_dirs(b.xdg_data_home, b.xdg_data_dirs, name, re);
}

std::vector<qxdg::path> qxdg::search_config_dirs(
  const base &b, const std::string &name, const std::regex &re
) {
  return search_dirs(b.xdg_config_home, b.xdg_config_dirs, name, re);
}
