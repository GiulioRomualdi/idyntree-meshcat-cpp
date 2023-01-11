<p align="center">
<h1 align="center">idyntree-meshcat-cpp</h1>
</p>
<p align="center">
<a href="https://isocpp.org"><img src="https://img.shields.io/badge/standard-C++17-blue.svg?style=flat&logo=c%2B%2B" alt="C++ Standard"/></a>
<a href="./LICENSE"><img src="https://img.shields.io/badge/License-BSD_3--Clause-orange.svg" alt="Size" /></a>
</p>

A simple bridge between [iDynTree](https://github.com/robotology/idyntree) and the [meshcat-cpp](https://github.com/GiulioRomualdi/meshcat-cpp) interface.

This repository contains the C++ implementation of the already existing [iDynTree Meshcat visualizer python class](https://github.com/robotology/idyntree/blob/7ecf7b536be9e03fe05a0d49c8e7adbc5f84dc31/bindings/python/visualize/meshcat_visualizer.py).

---

<p align="center">
  <b> 🚧 REPOSITORY UNDER DEVELOPMENT 🚧 </b>
  <br>The library implemented in this repository is still experimental and we cannot guarantee stable API
</p>

---

## 🏗️ Dependencies

**idyntree-meshcat-cpp** depends on `iDynTree` and `meshcat-cpp`.

## ⚒️ Build the library

You can build the library coping and paste the following snippet into a terminal
```console
git clone https://github.com/GiulioRomualdi/idyntree-meshcat-cpp.git
cd idyntree-meshcat-cpp
mkdir build && cd build
cmake ..
cmake --build .
[sudo] make install
```
## 🏃 Example

**idyntree-meshcat-cpp** provides native `CMake` support which allows the library to be easily used in `CMake` projects. Please add in your `CMakeLists.txt`
```cmake
project(foo)
find_package(iDynTreeMeshcatCpp REQUIRED)
add_executable(${PROJECT_NAME} src/foo.cpp)
target_link_libraries(${PROJECT_NAME} iDynTreeMeshcatCpp::iDynTreeMeshcatCpp)
```

The following example shows you how to display the iCub Humanoid robot with the visualizer. To compile the example please:
1. Build and install the [`icub-models`](https://github.com/robotology/icub-models) project 
2. Enable `IDYNTREE_MESHCAT_CPP_BUILT_EXAMPLES` cmake option
```cpp
#include <iDynTreeMeshcatCpp/Visualizer.h>
#include <iDynTree/Model/Model.h>
#include <iDynTree/ModelIO/ModelLoader.h>
#include <iCubModels/iCubModels.h>

int main()
{
    iDynTreeMeshcatCpp::Visualizer viz;

    const std::string modelPath = iCubModels::getModelFile("iCubGenova09");
    iDynTree::ModelLoader loader;
    loader.loadModelFromFile(modelPath);

    viz.loadModel(loader.model(), "iCub", {0,0,0});

    viz.join();

    return 0;
}
```

Once you have run the [example](./examples/example_viewer.cpp), the `MeshcatCpp::Meshcat` class will print the `URL` at which the MeshCat server runs. Please open the link in your browser and you should be able to see the following screen

![meshcat_icub](https://user-images.githubusercontent.com/16744101/211934912-95591ede-a93c-4fae-b1d0-8466105f8fa0.png)

## 🐛 Bug reports and support

All types of [issues](https://github.com/GiulioRomualdi/idyntree-meshcat-cpp/issues/new) are welcome.

## 📝 License
Materials in this repository are distributed under the following license:

> All software is licensed under the BSD 3-Clause "New" or "Revised" License. See [LICENSE](https://github.com/GiulioRomualdi/idyntree-meshcat-cpp/blob/master/LICENSE) file for details.
