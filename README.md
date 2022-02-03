# Day/Night Cycle Gem
This extension (*gem*) for [O3DE (Open 3D Engine)](https://www.o3de.org) provides a component to manage the change between day and night in scenes. It allows to:
- Define the day duration using a human-readable format for time values (e.g `Sunrise` and `Sunset` times).
- Move the sun position according to the scene time.
- Work synchronously with the main time component of the scene (provided by [Date & Time gem](https://github.com/loherangrin/addons.o3de.date-time)), or independently from it.

*VIDEO*

## Examples
A sample project showing the basic usage of this gem is located under `/examples/` directory. Three different variants are available:
- `examples/cpp`: implementation using C++ (native code).
- `examples/lua`: implementation using Lua (scripting).
- `examples/script-canvas`: implementation using ScriptCanvas (visual scripting). 

You can inspect each project variant adding it to your O3DE Project Manager (at least version `2111.2` is required). Or, if you are using **O3Tanks**, you can even run it directly as a standalone application:
```
cd <gem_dir>/examples/<implementation>
o3tanks build client
o3tanks run client
```

## Contribute
We are happy to receive your feeback! Please open an issue in [our tracker](https://github.com/loherangrin/addons.o3de.day-night-cycle/issues) if you would like to propose a new feature or you have encountered any error.

To get involved in the development of o3tanks, please refer to our [contribution guidelines](./CONTRIBUTING.md).

## License
**Day/Night Cycle Gem** is licensed under the Apache License, version 2.0. Please see [LICENSE](./LICENSE) and [NOTICE](./NOTICE) files for further details.
