# qxdg

Basic implementation of the XDG base directory specification.
https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html

## TODO:
- [ ] function to copy file `XDG_*_DIRS` to `XDG_*_HOME`

## usage
Environment variables must be loaded first:
```
qxdg::base base_dirs = qxdg::get_base_directories();
```

Then a path can be found according to the XDG priority:
```
std::string name = "test_program";
qxdg::path file_path = "shaders/vshader.glsl";
bool create = false;
bool force_home = false;

std::optional<qxdg::path> path = qxdg::get_data_path(
  base_dirs, name, file_path, create, force_home
)
```

Where `name` is a directory grouping files for the running program, and
`file_path` is the path to the file beneath the `name` directory.
The value of `*path` will be the first valid path, for example:
`/home/<username>/.local/share/test_program/shaders/v_shader.glsl`

If `create` is set to true then the specified file will be created under
`XDG_*_HOME` if it cannot be found.  
If `force_home` is set to true, then `create` will run if the file does not
exist under `XDG_*_HOME`
