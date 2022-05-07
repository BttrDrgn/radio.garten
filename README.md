# Radio.Garten
A [radio.garden](https://radio.garden/) desktop client written with SDL2 and ImGui

<p align="center">
  <img width="400" height="300" src="/../assets/preview.png">
</p>

## Compiling

- Run `git clone --recursive https://www.github.com/BttrDrgn/radio.garten`

Windows:
- Install Visual Studio 2022 with C++ support
- Run `generate.bat` for Visual Studio 2022
  - You can check the `generate.bat` and [premake actions](https://premake.github.io/docs/Using-Premake/) to use a different IDE
- Open `./build/Radio.Garten.sln`
- Build

Linux:
- Install `gcc`
- [Setup SDL2](https://wiki.libsdl.org/Installation#linuxunix) with the SDL2 submodule at `./deps/SDL2`
- Run `./generate.sh` for a GNU Makefile
- `cd` to `./build`
- Run `make`
  - Preferably with `-j` argument

## Dependencies
For license information, please check the containing depedency folder in `./deps` or check their website and/or repository page
- [SDL2](https://github.com/libsdl-org/SDL) - Cross-platform renderer and input handler
- [ImGui](https://github.com/ocornut/imgui) - GUI interface
- [BASS](https://www.un4seen.com/) - Audio library
- [freetype2 (2.12.1)](https://sourceforge.net/projects/freetype/files/freetype2/2.12.1/) - For FreeType fonts in ImGui
- [cpp-httplib](https://github.com/yhirose/cpp-httplib) - Single header http(s) request library
- [Nlohmann](https://github.com/nlohmann/json) - C++ JSON parser
