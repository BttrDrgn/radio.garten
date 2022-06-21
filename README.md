<h1 align="center"> Radio.Garten </h1>
<p align="center"> A <a href="https://radio.garden/"> radio.garden </a> desktop client and overlay written with SDL2 and ImGui </p>

<p align="center">
  <img width="400" height="300" src="/../assets/preview.png">
</p>


<h2 align="center"> Overlay Example </h2>
<p align="center">
  <img width="640" height="360" src="/../assets/overlay_example_nfsu2.png">
</p>

## Overlay Compatibility
Please check the [compatibility list][COMPAT_LIST] if you have any issues. If you are having issues with an application, please add it to the list!

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
- [ini_rw (personal fork)](https://github.com/BttrDrgn/ini_rw) - INI reader/writer
- [kiero (personal fork)](https://github.com/bttrdrgn/kiero/) - Multiple Graphical API hooking tool
- [MinHook](https://github.com/TsudaKageyu/minhook) - Hooking library (used in combination with Kiero)

<!-- Quick links -->
[COMPAT_LIST]: https://github.com/BttrDrgn/radio.garten/blob/main/COMPAT.md