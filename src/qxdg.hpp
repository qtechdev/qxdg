#ifndef __XDG_HPP__
#define __XDG_HPP__
// https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html

#include <filesystem>
#include <optional>
#include <regex>
#include <string>
#include <vector>

namespace qxdg {
  namespace fs = std::filesystem;
  using path = std::filesystem::path;

  struct base {
    path home;
    path xdg_data_home; // $HOME/.local/share
    path xdg_config_home; // $HOME/.config
    std::vector<path> xdg_data_dirs; // /usr/local/share/:/usr/share/
    std::vector<path> xdg_config_dirs; // /etc/xdg
    path xdg_cache_home; // $HOME/.cache
    path xdg_runtime_dir;
  };

  base get_base_directories(bool include_local=false);

  std::optional<path> get_path(
    const path &xdg_home, const std::vector<path> &xdg_dirs,
    const std::string &name, const path &p,
    const bool create, const bool force_home
  );

  std::optional<path> get_data_path(
    const base &b, const std::string &name, const path &p,
    const bool create=false, const bool force_home=false
  );

  std::optional<path> get_config_path(
    const base &b, const std::string &name, const path &p,
    const bool create=false, const bool force_home=false
  );

  std::vector<path> search_dirs(
    const path &xdg_home, const std::vector<path> &xdg_dirs,
    const std::string &name, const std::regex &re
  );

  std::vector<path> search_data_dirs(
    const base &b, const std::string &name, const std::regex &re
  );

  std::vector<path> search_config_dirs(
    const base &b, const std::string &name, const std::regex &re
  );
}

#endif // __XDG_HPP__
