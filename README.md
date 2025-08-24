# Executor
Executor is a lightweight launcher application designed to store, organise and execute all of your .exe files for ease of access, without clutter. Making it useful for those people with too many applications to count. Instead of rummaging through your pc to find that one .exe, Open executor and find it with ease. Execute all of your files with the simple yet elegant GUI.

![Image of Executor GUI](https://i.imgur.com/QE9y0XR.png)

## Installation
#### For compiled binaries check the [github releases page](https://github.com/Zentifyyy/Executor/releases).

- Otherwise, make sure you have the [Vulkan SDK](https://vulkan.lunarg.com/sdk/home) and [CMake](https://cmake.org/download) installed.

- Some of these instructions are [Visual Studio 2022](https://visualstudio.microsoft.com) specific, which is recommended but not strictly necessary.

- Clone the repository.
```
git clone --recursive https://github.com/Zentifyyy/Executor
```
- Then, run ```scripts/Setup.bat``` to build the libtinyfiledialogs library and generate a Visual Studio solution file for the main project.

- Then open up the ```WalnutApp.sln``` in the base directory. We will then need to add ```tinyfiledialogs``` as a dependency by right clicking on Dependencies in your solution explorer, navigating to add and pressing Existing Project... Then go to ```Walnut/vendor/libtinyfiledialogs/build``` and add ```tinyfiledialogs.vcxproj```.

- And finally, navigate to the WalnutApp solution in Visual Studio and right click references and then add reference. Tick ```libtinyfiledialogs``` and you are good to go.

### Still in development. Setting up the project should get easier in the future once i'm more comfortable with premake.

## Additional
- Executor uses the [Walnut application framework](https://github.com/StudioCherno/Walnut) made by Studio Cherno, ([MIT License](https://github.com/Zentifyyy/Executor/blob/master/LICENSE.txt)).

- Executor uses the [Roboto](https://fonts.google.com/specimen/Roboto) Font, [(Apache License, Version 2.0)](https://www.apache.org/licenses/LICENSE-2.0).

- Executor uses [Material Symbols](https://fonts.google.com/icons), [(Apache License, Version 2.0)](https://www.apache.org/licenses/LICENSE-2.0).

### 3rd Party Libraries
- [Dear ImGui](https://github.com/ocornut/imgui)
- [GLFW](https://github.com/glfw/glfw)
- [stb_image](https://github.com/nothings/stb)
- [GLM](https://github.com/g-truc/glm)
- [libtinyfiledialogs](https://github.com/IamtheMZI/libtinyfiledialogs)