# Day/Night Cycle Gem
This extension (*gem*) for [O3DE (Open 3D Engine)](https://www.o3de.org) provides a component to manage the change between day and night in scenes. It allows to:
- Define the day duration using a human-readable format for time values (e.g `Sunrise` and `Sunset` times).
- Move the sun position according to the scene time.
- Work synchronously with the main time component of the scene (provided by [Date & Time gem](https://github.com/loherangrin/addons.o3de.date-time)), or independently from it.

https://user-images.githubusercontent.com/51530087/152421714-f27f9632-1a6e-4578-91e7-16b7f1941ec2.mp4

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

## Install
> This gem requires [Date & Time Gem](https://github.com/loherangrin/addons.o3de.date-time) to handle time data values. Please refer to its documentation to install the required gem before proceding further.

Please choose one of the following options to install this gem in your O3DE projects:

### Automatic installation
Following steps require to install **O3Tanks**, a version manager for O3DE to streamline the development workflow. You can find additional information at its [main repository](https://github.com/loherangrin/o3tanks/).
```
o3tanks install gem https://github.com/loherangrin/addons.o3de.day-night-cycle.git --as <any_name>
o3tanks add gem <any_name> --project <project_path>
```

### Manual installation
1. Clone this repository into a directory of your choice:
```
git clone https://github.com/loherangrin/addons.o3de.day-night-cycle.git <gem_dir>
```
2. Register the gem to your engine, following the steps that are described in [Registering Gems to a Project](https://www.o3de.org/docs/user-guide/project-config/register-gems/) page of the official O3DE documentation.
3. Activate the gem in your project, following the steps that are described in [Adding and Removing Gems in a Project](https://www.o3de.org/docs/user-guide/project-config/add-remove-gems/). You will have to look for *Day/Night Cycle* as gem name in the Catalog list.

## Contribute
We are happy to receive your feeback! Please open an issue in [our tracker](https://github.com/loherangrin/addons.o3de.day-night-cycle/issues) if you would like to propose a new feature or you have encountered any error.

To get involved in the development of o3tanks, please refer to our [contribution guidelines](./CONTRIBUTING.md).

## License
**Day/Night Cycle Gem** is licensed under the Apache License, version 2.0. Please see [LICENSE](./LICENSE) and [NOTICE](./NOTICE) files for further details.
