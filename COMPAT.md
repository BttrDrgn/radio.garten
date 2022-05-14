This list will mainly focus on incompatibilites, but will be updated accordingly if an application begins to work.

### Legend
- :x: - Does not work
- :o: - Works with issues
- :heavy_check_mark: - Works

| Application | Graphics | Architecture | Working | Version | Notes |
|---|---|---|---|---|---|
| A Hat in Time | DX9 | x64 | :heavy_check_mark: | [v0.5.1][v0.5.1] | 
| American Truck Simulator | DX11 or OGL3 | x86 or x64 | :o: | [v0.5.1][v0.5.1] | Game uses custom mouse detection so the overlay will not respond to the mouse |
| Blood: Fresh Supply | DX11, OGL3.2, or Vulkan | x64 | :o: | [v0.6.0][v0.6.0] | Injects and works as intended on DX11. OpenGL3.2 injects, but fails to show the overlay, and Vulkan fails to inject at all.
| Geometry Dash | OGL2 | x86 or x64 | :heavy_check_mark: | [v0.6.0][v0.6.0] | 
| Getting Over It with Bennet Foddy | DX11 | x64 | :o: | [v0.6.0][v0.6.0] | Injects, but fails to show the ovelay.
| Katamari Damacy Reroll | DX11 | x64 | :o: | [v0.6.0][v0.6.0] | Injects, however the overlay will not respond to the mouse.
| Quake 2 | OGL1.4 or OGL3.2 | x86 or x64 | :o: | [v0.6.0][v0.6.0] | Successfully injects and works on both versions of OpenGL, and x86 and x64, however, the mouse cursor will remain stuck on screen. Injecting on x64 OpenGL1.4, causes the fonts to become unreadable.
| Terraria | DX9 | x86 of x64 | :o: | [v0.6.0][v0.6.0] | Injects and works as intended, however the mouse cursor will not show on the overlay. The overlay can still be interacted with. This is likely due to terraria using it's own custom cursor.
| Transformice | DX11 | x86 or x64 | :heavy_check_mark: | [v0.6.0][v0.6.0] | 

<!-- Quick links -->
[v0.3.0]: https://github.com/BttrDrgn/radio.garten/releases/tag/v0.3.0
[v0.4.0]: https://github.com/BttrDrgn/radio.garten/releases/tag/v0.4.0
[v0.5.0]: https://github.com/BttrDrgn/radio.garten/releases/tag/v0.5.0
[v0.5.1]: https://github.com/BttrDrgn/radio.garten/releases/tag/v0.5.1
[v0.6.0]: https://github.com/BttrDrgn/radio.garten/releases/tag/v0.6.0
