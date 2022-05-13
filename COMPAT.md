This list will mainly focus on incompatibilites, but will be updated accordingly if an application begins to work.

### Legend
- :x: - Does not work
- :o: - Works with issues
- :heavy_check_mark: - Works

| Application | Graphics | Architecture | Working | Version | Notes |
|---|---|---|---|---|---|
| A Hat in Time | Direct3D 9 | x64 | :x: | [v0.4.0±][v0.4.0] | Causes a crash on injection, likely due to Unreal 3 doing forward rendering.
| American Truck Simulator | DX11 or OGL3 | x86 or x64 | :o: | [v0.4.0±][v0.4.0] | Crashes on injection when running the game in x64 mode, overlay work in x86 mode (both DX11 and OGL3); Game uses custom mouse detection so the overlay will not respond to the mouse |

<!-- Quick links -->
[v0.3.0]: https://github.com/BttrDrgn/radio.garten/releases/tag/v0.3.0
[v0.4.0]: https://github.com/BttrDrgn/radio.garten/releases/tag/v0.4.0
