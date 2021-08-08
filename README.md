# Vulkan-Voxel-Engine
A personal experiment in which I develop an open world voxel engine using C++ and the Vulkan API.

## Dependencies
To be able to compile and run the project, you will need these dependencies. After downloading them, make sure you add the necessary include directories (containing the library's header files) and library directories (containing the static `.lib` and/or dynamic library `.dll` files, this is not necessary for header-only libraries) to your project configuration so the compiler and linker know where to find these.
- [GLFW](https://www.glfw.org/download.html) (3.3.4)
- [glm](https://github.com/g-truc/glm)
- [Vulkan SDK](https://vulkan.lunarg.com/)

## Shaders
Vulkan requires that shaders are compiled into the SPIR-V format. Therefore, I have provided a `compile.bat` file which serves to compile all the shaders in the projects. It uses the `glsc` compiler (installed with the Vulkan SDK) in order to do so. It might be necessary to change the path to the `glsc` compiler within this file when you try to clone and run the project, depending on which platform you are working on and where you have the Vulkan SDK installed. If you add new shaders, you should also add them to this file in order for them to be compiled. Make sure to run `compile.bat` every time after making changes to the shaders.
