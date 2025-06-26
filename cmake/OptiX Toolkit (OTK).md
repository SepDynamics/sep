OptiX Toolkit (OTK)
A set of utilities commonly used in applications utilizing the OptiX ray tracing API.

See the CHANGELOG for recent changes.

After checking out the toolkit, be sure to initialize the submodules before building:

git submodule update --init --recursive
Submodules merged in v1.0.0
If you cloned the OptiX Tookit repository prior to v1.0.0, we recommend cloning a fresh repository. We restructured the repository, merging multiple submodules into the main repository, which complicates performing a pull operation. (If you are intent on pulling, use git submodule deinit on all of the submodules except vcpkg before pulling.)

Current Utilities
DemandLoading - a C++/CUDA library for loading CUDA sparse textures on demand in OptiX renderers.
Memory - Memory allocators (used by DemandLoading library).
OmmBaking - a C++/CUDA library for baking Opacity Micromap Arrays for textured geometry.
PyOptiX - Complete Python bindings for the OptiX host API.
ShaderUtil - Header-only libraries for OptiX kernels (e.g. vector math, Self Intersection Avoidance).
Requirements
C++ compiler (gcc, Clang, or Visual Studio)
git (any modern version with LFS) (download)
CMake 3.27 or later (download).
Using the latest CMake is highly recommended, to ensure up-to-date CUDA language support.
CUDA 11.1 or later (download)
OptiX 7.3 or later (download)
Other third-party libraries are downloaded and built on demand (see below).
On some Linux systems it may be necessary to install some commonly used developer packages with the following commands:

sudo apt-get install curl git-lfs pkg-config
git lfs install
Building the examples included with the OptiX Toolkit requires an OpenGL development environment. On most Linux systems the necessary packages can be installed using the following command:

sudo apt-get install libx11-dev libxcursor-dev libxi-dev libxinerama-dev libxrandr-dev mesa-common-dev
Under Rocky linux, the following packages must be installed to build the GL-based OTK examples:

sudo dnf install libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel mesa-libGLU-devel pkg-config
Specifying the Location of the OptiX SDK
The OptiX Toolkit will do it's best to locate the highest installed version of the OptiX SDK. On Windows, the standard installation location %ProgramData%/NVIDIA Corporation is searched; on Linux, the locations /opt, /usr/local, $HOME and $HOME/Downloads are searched for the default SDK directory name.

If you installed the OptiX SDK in another location, or using a different directory name than the suggested default, you can specify the location of the OptiX SDK by setting the CMake variable OptiX_INSTALL_DIR (case sensitive) at CMake configure time. Set the variable to the path to the directory containing the OptiX SDK include directory.

Building the OptiX Toolkit with the Supplied CMake Presets
CMake presets provide a convenient means of specifying CMake configure settings, build settings and test settings as a named collection. A CMakePresets.json file is provided with the toolkit to cover some basic use cases and for use as building blocks for creating your own preset that covers your specific use case.

To build the toolkit with default settings, issue the following command from the source code directory.

cmake --preset otk-default
cmake --build --preset otk-default -j
The following command runs the tests using the default preset. (Note that ctest requires a value for the -j option.)

ctest --preset otk-default -j 16
All supplied presets begin with the prefix otk- so that they won't conflict with your personal presets. It is recommended that you store your personal presets in the file CMakeUserPresets.json in the source directory so that future updates to the toolkit won't conflict with your personal presets.

The supplied presets create build directories as children of the source directory with the name of the preset in the build directory name, e.g. build-otk-default. This gives each preset a distinct build directory allowing you to experiment with different presets without them interfering with one another.

Consult the CMakePresets.json file for the available building blocks for use in creating your own customized preset that suits your individual needs.

Building the OptiX Toolkit Manually
Building the toolkit follows the standard CMake worfklow: configure, build and test.

Create a directory called build and cd to that directory.
mkdir build
cd build
Configure the toolkit using CMake, optionally specifying the location of the OptiX SDK. This can be accomplished using the CMake GUI tool, the CMake console tool (not available on Windows as of this writing), or from the command-line directly by entering the following command: (Note that .. specifies the path to the source code from the build directory.)
cmake ..
This will configure the toolkit with the default options create a build project using the default CMake generator for your platform.
If the configuration is successful, build the OTK libraries with the following command: cmake --build . --config Release
After building you can execute the tests with the following command: ctest -C Release
If you wish to customize the build of the toolkit, see the section on options.

If you encounter problems or if you have any questions, we encourage you to post on the OptiX developer forum.

OptiX Toolkit Options
The following options may be supplied to CMake at configure time to customize the toolkit:

Variable	Type	Default	Description
OTK_USE_VCPKG	BOOL	ON	Use vcpkg for dependencies.
OTK_USE_VCPKG_OPENEXR	BOOL	${OTK_USE_VCPKG}	Obtain OpenEXR via vcpkg.
OTK_USE_OIIO	BOOL	OFF	Use OpenImageIO to read PNG and JPEG files as image sources.
OTK_FETCH_CONTENT	BOOL	ON	Use FetchContent for dependencies if OTK_USE_VCPKG is OFF.
OTK_BUILD_EXAMPLES	BOOL	ON	Build the examples.
OTK_BUILD_TESTS	BOOL	ON	Build the tests.
OTK_BUILD_DOCS	BOOL	ON	Build the doxygen documentation.
OTK_BUILD_PYOPTIX	BOOL	OFF	Build the PyOptiX python module.
OTK_PROJECT_NAME	STRING	OptiXToolkit	Project name for the generated build scripts.
OTK_LIBRARIES	STRING	ALL	List of libraries to build.
If both OTK_USE_VCPKG and OTK_FETCH_CONTENT are ON, vcpkg will be used for dependencies.

If the option OTK_LIBRARIES is used to configure the libraries to build, the value should be a semi-colon separated list of one or more of the names DemandLoading, Memory, OmmBaking or ShaderUtil. The default value ALL is the same as specifying DemandLoading;Memory;OmmBaking;ShaderUtil. Some libraries depend on other libraries. The CMake build script includes dependent libraries as needed.

Third-party Libraries
The toolkit depends on third party libraries for texture file format parsing, a GUI framework for the examples and a unit test framework for the tests.

Component	Dependency
DemandLoading	Imath 3.1.5 or later
OpenEXR 3.1.5 or later
(optional) OpenImageIO
Examples	imgui
GLFW 3.3 or later
glad (any recent version)
stb
Tests	gtest
The toolkit can automatically obtain these third party libraries in one of two ways: via a vcpkg manifest or via FetchContent as described below.

Using vcpkg is the recommended method of obtaining third-party libraries.

VcPkg
The toolkit repository contains a vcpkg manifest to download and build third party libraries. The repository includes the vcpkg repository as a submodule to select the specific versions of dependencies used. A vcpkg manifest, vcpkg.json, specifies the dependencies to be used. The vcpkg standard CMake integration via a toolchain file is used to bootstrap vcpkg and obtain the dependencies.

The use of vcpkg can be disabled by configuring with OTK_USE_VCPKG=OFF, which will cause the toolkit to use FetchContent for third party libraries.

When the toolkit is used as a subdirectory, e.g. a git submodule, of another project, the vcpkg manifest for toolkit must be incorporated into the parent project's manifest for vcpkg to correctly fetch the third party libraries used by the toolkit.

The CMake module ProjectOptions (from the toolkit's CMake directory) should be included by the parent project's CMakeLists.txt before the first call to project. This gives the toolkit the chance to configure optional features from the manifest and configure options controlling how the toolkit is built.

If the variable CMAKE_TOOLCHAIN_FILE is not set when ProjectOptions is included, it will be set to point to the CMake integration in the toolkit's vcpkg submodule. A parent project using vcpkg for dependency management may be using its own submodule of vcpkg for toolchain integration and should set CMAKE_TOOLCHAIN_FILE as appropriate before including the toolkit's ProjectOptions module.

FetchContent
If vcpkg is disabled, the toolkit will use CMake's FetchContent feature to download and build any missing third-party libraries. The use of FetchContent can be disabled by setting OTK_FETCH_CONTENT=OFF during CMake configuration, which is necessary when building statically linked libraries, as described below.

When FetchContent is disabled, the following CMake configuration variables should be used to specify the locations of the third-party libraries: Imath_DIR, OpenEXR_DIR, glfw3_DIR, and glad_DIR. The directory specified for each of these variables should be the location of the project's CMake configuration file. For example:

cd build
cmake \
-DBUILD_SHARED_LIBS:BOOL=OFF \
-DOTK_FETCH_CONTENT:BOOL=OFF \
-DOTK_BUILD_EXAMPLES:BOOL=OFF \
-DOTK_BUILD_TESTS:BOOL=OFF \
-DImath_DIR:PATH=/usr/local/Imath/lib/cmake/Imath \
-DOpenEXR_DIR:PATH=/usr/local/OpenEXR/lib/cmake/OpenEXR \
-Dglfw3_DIR:PATH=/usr/local/glfw3/lib/cmake/glfw3 \
-Dglad_DIR:PATH=/usr/local/glad/lib/cmake/glad \
-DOptiX_ROOT_DIR:PATH=/usr/local/OptiX-SDK-7.5 \
../optix-toolkit
When FetchContent is disabled, using vcpkg as described above is recommended. Alternatively, the necessary third-party libraries from source code downloaded from the following locations:

Imath 3.1.5: https://github.com/AcademySoftwareFoundation/Imath.git
OpenEXR 3.1.5: https://github.com/AcademySoftwareFoundation/openexr.git
GLFW 3.3: https://github.com/glfw/glfw.git
glad: https://github.com/Dav1dde/glad
Building Statically Linked Libraries
OptiX Toolkit components are compiled into dynamic libraries (DSOs/DLLs) to simplify linking client applications. This eliminates the need for client applications to link with third-party libraries like OpenEXR and GLFW.

Some clients of the toolkit might prefer to use statically linked libraries. This can be accomplished by setting the CMake configuration variable BUILD_SHARED_LIBS=OFF.

Important: when building statically linked libraries, the CMake configuration variable OTK_FETCH_CONTENT should be set to OFF, and various third-party libraries must be installed as described above.

Troubleshooting
Problem: CMake configuration error: "OTK_USE_VCPKG is ON, but could not locate vcpkg toolchain file"
Solution: vcpkg submodule must be initialized, e.g. git submodule update --init --recursive

Problem: add_library cannot create ALIAS target "OpenEXR::Config" because another target with the same name already exists.
Solution: Install OpenEXR 3.1 or later or set OpenEXR_DIR to such an installation.

Problem: CMake configuration error: "could not find git for clone of glad-populate"
Solution: git is required in order to download third party libraries (e.g. glad)

Problem: Runtime error: OPTIX_ERROR_UNSUPPORTED_ABI_VERSION: Optix call 'optixInit()' failed
Solution: Download newer driver

Problem: CMake configuration error: "Failed to detect a default cuda architecture"
Solution: Set configuration variable CMAKE_CUDA_COMPILER to the full path of the NVCC compiler.

Problem: Tests fail with "Cannot read image file filename.exr. File is not an image file." Solution: Install git lfs (git lfs install) and re-clone repository.

Problem: Compiling the CUDA compiler identification source file failed. gcc versions later than 13 are not supported. Solution: Install gcc-13 (sudo apt-get install gcc-13) and use update-alternatives to make gcc-13 the default.

Problem: Running vcpkg install - failed Solution: git submodule deinit vcpkg; git submodule update --init vcpkg

If you encounter a problem, we encourage you to post on the OptiX forums or open a ticket on the OptiX Toolkit issues page on GitHub.

Attributions
This project contains build logic from the OptiX Wrapper Library (OWL), which is redistributed under the terms of the Apache License Version 2.0.


Building Cycles
===============

## Prerequisites

Ensure the following software is installed and available in the PATH:
- Git
- Git LFS
- Python 3
- CMake

### Standalone Build

Get the source code:

    git clone https://projects.blender.org/blender/cycles.git
    cd cycles

Download precompiled libraries and build:

    make update
    make

The resulting binary will be at:

    ./install/cycles

## Hydra Render Delegate with USD Repository

This will make the render delegate work with usdview and other applications built using the USD repository. USD version 24.03 or newer is required.

USD includes a script to build itself and all required dependencies and then install the result a specified directory.

    git clone https://github.com/PixarAnimationStudios/USD.git
    cd USD
    python3 build_scripts/build_usd.py "<path to USD install>"

Get the Cycles source code:

    git clone https://projects.blender.org/blender/cycles.git
    cd cycles

By default older precompiled libraries need to be used, for compatibility with older VFX platforms and TBB.
Download the libraries and build pointing to the USD directory like this.

    make update_legacy
    cmake -B ./build -DPXR_ROOT="<path to USD install>" -DWITH_LEGACY_LIBRARIES=ON
    make

When using a newer VFX platform and USD was built with `--onetbb`, do this instead:

    make update
    cmake -B ./build -DPXR_ROOT="<path to USD install>"
    make

Test in usdview.

    PYTHONPATH=<path to USD install>/lib/python PXR_PLUGINPATH_NAME=<path to cycles>/install/hydra <path to USD install>/bin/usdview

## Hydra Render Delegate for Houdini

For use in Houdini, Cycles must be built using Houdini's USD libraries. Houdini version 20 or newer is required.
Currently older libraries must be used for compatibility. Future Houdini versions will not need the legacy options.

Get the source code:

    git clone https://projects.blender.org/blender/cycles.git
    cd cycles

Download precompiled libraries and build.

    make update_legacy
    cmake -B ./build -DHOUDINI_ROOT="<path to Houdini>" -DWITH_LEGACY_LIBRARIES=ON
    make

The path to Houdini depends on the operating system, typically:
- Linux: `/opt/hfsX.Y`
- macOS: `/Applications/Houdini/HoudiniX.Y.ZZZ`
- Windows: `C:/Program Files/Side Effects Software/Houdini X.Y.ZZZ`

Test in Houdini using an environment variable.

    PXR_PLUGINPATH_NAME=<path to cycles>/install/houdini/dso/usd_plugins houdini

Or copy `install/houdini/packages/cycles.json` to the Houdini packages directory to make it always available.

## Build System

Cycles uses the CMake build system. As an alternative to the `make` wrapper, CMake can be manually configured.

See the CMake configuration to enable and disable various features.

The precompiled libraries are shared with Blender, and will be automatically downloaded from the Blender repository with `make update`. This will populate a submodule in the `lib/` folder, matching the platform.

## Dependencies

Core Cycles has the following required and optional library dependencies. These are all included in precompiled libraries.

Required:
- OpenImageIO
- TBB

Optional:
- Alembic
- Embree
- OpenColorIO
- OpenVDB / NanoVDB
- OpenShadingLanguage
- OpenImageDenoise
- USD

For GUI support, the following libraries are required. The SDL library must be manually provided, it's not part of the precompiled libraries.
- OpenGL
- GLEW
- SDL

For GPU rendering support on NVIDIA cards, these need to be downloaded and installed from the NVIDIA website.
- CUDA Toolkit 11 or newer
- OptiX 7.3 SDK or newer