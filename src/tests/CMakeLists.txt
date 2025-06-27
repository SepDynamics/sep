# CMakeLists.txt for Cycles integration tests

# Build the Cycles test application regardless of Cycles availability
add_executable(cycles_test cycles_test.cpp)

# For the test specifically, we'll use stub implementations to avoid
# complex linking issues with multiple versions of libraries
# REMOVED: target_compile_definitions(cycles_test PRIVATE SEP_USE_CYCLES_STUB=1)
# The goal is a full build, so remove the stub definition unless explicitly needed for a debug test target.

target_include_directories(cycles_test
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# First, find all the required dependencies
# Ensure these are found and ideally, full paths are used for system libraries.
find_library(OPENIMAGEIO_LIBRARY NAMES OpenImageIO)
find_library(OPENIMAGEIO_UTIL_LIBRARY NAMES OpenImageIO_Util)
find_library(OSL_COMP_LIBRARY NAMES oslcomp)
find_library(OSL_EXEC_LIBRARY NAMES oslexec)
find_library(OSL_QUERY_LIBRARY NAMES oslquery)
find_library(OPENVDB_LIBRARY NAMES openvdb) # This is usually a shared library, ensure it's found
find_library(ALEMBIC_LIBRARY NAMES Alembic) # This is usually a shared library
find_library(ZSTD_LIBRARY NAMES zstd)
find_library(OPENCOLORIO_LIBRARY NAMES OpenColorIO)
find_library(IMATH_LIBRARY NAMES Imath)
find_library(OPENEXR_LIBRARY NAMES OpenEXR)
find_library(BLOSC_LIBRARY NAMES blosc)
find_library(BOOST_THREAD_LIBRARY NAMES boost_thread)
find_library(BOOST_SYSTEM_LIBRARY NAMES boost_system)
find_library(BOOST_IOSTREAMS_LIBRARY NAMES boost_iostreams)
find_library(BOOST_REGEX_LIBRARY NAMES boost_regex)
find_library(BOOST_FILESYSTEM_LIBRARY NAMES boost_filesystem)
find_library(GLOG_LIBRARY NAMES glog) # From the build log, glog is mentioned.
find_library(ZLIB_LIBRARY NAMES z) # Assuming zlib, not ZLib_LIBRARIES directly
find_library(EMBREE_LIBRARY NAMES embree4) # Version-specific, check your installation
find_library(TBB_LIBRARY NAMES tbb) # General TBB library

# Print status of found libraries
message(STATUS "Found OpenImageIO: ${OPENIMAGEIO_LIBRARY}")
message(STATUS "Found OpenImageIO_Util: ${OPENIMAGEIO_UTIL_LIBRARY}")
message(STATUS "Found OSL Comp: ${OSL_COMP_LIBRARY}")
message(STATUS "Found OSL Exec: ${OSL_EXEC_LIBRARY}")
message(STATUS "Found OSL Query: ${OSL_QUERY_LIBRARY}")
message(STATUS "Found OpenVDB: ${OPENVDB_LIBRARY}")
message(STATUS "Found Alembic: ${ALEMBIC_LIBRARY}")
message(STATUS "Found ZSTD: ${ZSTD_LIBRARY}")
message(STATUS "Found OpenColorIO: ${OPENCOLORIO_LIBRARY}")
message(STATUS "Found Imath: ${IMATH_LIBRARY}")
message(STATUS "Found OpenEXR: ${OPENEXR_LIBRARY}")
message(STATUS "Found Blosc: ${BLOSC_LIBRARY}")
message(STATUS "Found Boost Thread: ${BOOST_THREAD_LIBRARY}")
message(STATUS "Found Boost System: ${BOOST_SYSTEM_LIBRARY}")
message(STATUS "Found Boost IOStreams: ${BOOST_IOSTREAMS_LIBRARY}")
message(STATUS "Found Boost Regex: ${BOOST_REGEX_LIBRARY}")
message(STATUS "Found Boost Filesystem: ${BOOST_FILESYSTEM_LIBRARY}")
message(STATUS "Found GLog: ${GLOG_LIBRARY}")
message(STATUS "Found ZLib: ${ZLIB_LIBRARY}")
message(STATUS "Found Embree: ${EMBREE_LIBRARY}")
message(STATUS "Found TBB: ${TBB_LIBRARY}")


# Set include directories
target_include_directories(cycles_test
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/extern/cycles/src # This path might not be sufficient for all headers. Consider /sep/cycles/src/app or /sep/cycles/src/kernel
        /usr/include/OpenImageIO
        /usr/include/OpenColorIO
        /usr/include/openexr
        /usr/include/openvdb # Add OpenVDB includes
        /usr/include/Alembic # Add Alembic includes
)

# Find Blender internal libraries
# Ensure these paths are correct for your installed Blender libraries.
# Usually, these are `.a` (static) libraries.
find_library(BF_GUARDEDALLOC_LIBRARY NAMES bf_intern_guardedalloc PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(BF_INTERN_OPENSUBDIV_LIBRARY NAMES bf_intern_opensubdiv PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(BF_INTERN_SKY_LIBRARY NAMES bf_intern_sky PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(BF_BLENKERNEL_LIBRARY NAMES bf_blenkernel PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(BF_BLENLIB_LIBRARY NAMES bf_blenlib PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(BF_DRAW_LIBRARY NAMES bf_draw PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(BF_GPU_LIBRARY NAMES bf_gpu PATHS ${CMAKE_SOURCE_DIR}/lib)

# Find external library dependencies (cuew, hipew)
find_library(EXTERN_CUEW_LIBRARY NAMES extern_cuew PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(EXTERN_HIPEW_LIBRARY NAMES extern_hipew PATHS ${CMAKE_SOURCE_DIR}/lib)

# Find OpenSubdiv libraries
find_library(OPENSUBDIV_CPU_LIBRARY NAMES
  osdCPU
  libosdCPU.so
  libosdCPU.so.3.6.0
  PATHS
  ${CMAKE_SOURCE_DIR}/lib
  /usr/lib64
  /usr/lib
  /usr/local/lib
)

# Find OpenSubdiv GPU library if available
find_library(OPENSUBDIV_GPU_LIBRARY NAMES
  osdGPU
  libosdGPU.so
  libosdGPU.so.3.6.0
  PATHS
  ${CMAKE_SOURCE_DIR}/lib
  /usr/lib64
  /usr/lib
  /usr/local/lib
)

# Find specific TBB version required by OpenSubdiv (2.x)
# This prevents version conflicts with other TBB libraries (12.x)
find_library(TBB_OPENSUBDIV_LIBRARY NAMES
  tbb.so.2
  libtbb.so.2
  PATHS
  /usr/lib64
  /usr/lib
  /usr/local/lib
)

message(STATUS "Found TBB for OpenSubdiv: ${TBB_OPENSUBDIV_LIBRARY}")

# Try different library name variations for the OpenPGL library
find_library(EXTERN_OPENPGL_LIBRARY NAMES # Renamed from EXTERN_OPENGL_LIBRARY to avoid confusion
  extern_openpgl
  extern_pgl
  openpgl
  PATHS
  ${CMAKE_SOURCE_DIR}/lib
  /usr/lib64
  /usr/lib
)

# If the OpenPGL library is not found, provide a warning but continue
if(NOT EXTERN_OPENPGL_LIBRARY) # Use new variable name
  message(WARNING "OpenPGL library not found, will continue without it")
  add_compile_definitions(SEP_NO_OPENPGL)
endif()

message(STATUS "Found Blender Guardian Alloc: ${BF_GUARDEDALLOC_LIBRARY}")
message(STATUS "Found Blender OpenSubdiv: ${BF_INTERN_OPENSUBDIV_LIBRARY}")
message(STATUS "Found Blender Sky: ${BF_INTERN_SKY_LIBRARY}")
message(STATUS "Found Blender Kernel: ${BF_BLENKERNEL_LIBRARY}")
message(STATUS "Found Blender Lib: ${BF_BLENLIB_LIBRARY}")
message(STATUS "Found Blender Draw: ${BF_DRAW_LIBRARY}") # Added from tree
message(STATUS "Found Blender GPU: ${BF_GPU_LIBRARY}")   # Added from tree
message(STATUS "Found Extern CUEW: ${EXTERN_CUEW_LIBRARY}")
message(STATUS "Found Extern HIPEW: ${EXTERN_HIPEW_LIBRARY}")

# Find missing Cycles BVH implementation libraries
find_library(CYCLES_BVH_LIBRARY NAMES
  cycles_bvh
  libcycles_bvh.a
  PATHS
  ${CMAKE_SOURCE_DIR}/lib
  ${CMAKE_SOURCE_DIR}/extern/cycles/lib # Check in extern/cycles/lib too
)

# Cycles specific libraries (important for `ccl::Node`, `ccl::NodeType`, etc.)
# These need to be explicitly found and linked in the correct order.
# Based on the structure, these are likely static libraries (.a)
# Assuming they are in ${CMAKE_SOURCE_DIR}/lib as per your tree
find_library(CYCLES_KERNEL_LIBRARY NAMES cycles_kernel libcycles_kernel.a PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(CYCLES_DEVICE_LIBRARY NAMES cycles_device libcycles_device.a PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(CYCLES_UTIL_LIBRARY NAMES cycles_util libcycles_util.a PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(CYCLES_SCENE_LIBRARY NAMES cycles_scene libcycles_scene.a PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(CYCLES_SESSION_LIBRARY NAMES cycles_session libcycles_session.a PATHS ${CMAKE_SOURCE_DIR}/lib)
# Look for OSL-related libraries in multiple locations with expanded naming schemes
find_library(CYCLES_OSL_LIBRARY NAMES cycles_osl libcycles_osl.a PATHS
  ${CMAKE_SOURCE_DIR}/lib
  ${CYCLES_ROOT}/lib
  ${BLENDER_LIB_DIR}
  /usr/lib/cycles
  /usr/local/lib/cycles
)

# Look for additional OSL components (often needed for full OSL support)
find_library(CYCLES_OSL_EXEC_LIBRARY NAMES cycles_osl_exec libcycles_osl_exec.a PATHS
  ${CMAKE_SOURCE_DIR}/lib
  ${CYCLES_ROOT}/lib
  ${BLENDER_LIB_DIR}
  /usr/lib/cycles
  /usr/local/lib/cycles
)

find_library(CYCLES_OSL_SERVICES_LIBRARY NAMES cycles_osl_services libcycles_osl_services.a PATHS
  ${CMAKE_SOURCE_DIR}/lib
  ${CYCLES_ROOT}/lib
  ${BLENDER_LIB_DIR}
  /usr/lib/cycles
  /usr/local/lib/cycles
)

# Look for math libraries with explicit AVX2 support first
find_library(CYCLES_MATH_AVX2_LIBRARY NAMES cycles_math_avx2 libcycles_math_avx2.a PATHS
  ${CMAKE_SOURCE_DIR}/lib
  ${CYCLES_ROOT}/lib
  ${BLENDER_LIB_DIR}
  /usr/lib/cycles
  /usr/local/lib/cycles
)

# Fallback to regular math library if AVX2 not found
find_library(CYCLES_MATH_LIBRARY NAMES cycles_math libcycles_math.a PATHS
  ${CMAKE_SOURCE_DIR}/lib
  ${CYCLES_ROOT}/lib
  ${BLENDER_LIB_DIR}
  /usr/lib/cycles
  /usr/local/lib/cycles
)

find_library(CYCLES_TRANSFORMS_LIBRARY NAMES cycles_transforms libcycles_transforms.a PATHS
  ${CMAKE_SOURCE_DIR}/lib
  ${CYCLES_ROOT}/lib
  ${BLENDER_LIB_DIR}
  /usr/lib/cycles
  /usr/local/lib/cycles
)


message(STATUS "Found Cycles Kernel Library: ${CYCLES_KERNEL_LIBRARY}")
message(STATUS "Found Cycles Device Library: ${CYCLES_DEVICE_LIBRARY}")
message(STATUS "Found Cycles Util Library: ${CYCLES_UTIL_LIBRARY}")
message(STATUS "Found Cycles Scene Library: ${CYCLES_SCENE_LIBRARY}")
message(STATUS "Found Cycles Session Library: ${CYCLES_SESSION_LIBRARY}")
message(STATUS "Found Cycles OSL Library: ${CYCLES_OSL_LIBRARY}")
message(STATUS "Found Cycles Math Library: ${CYCLES_MATH_LIBRARY}")
message(STATUS "Found Cycles Transforms Library: ${CYCLES_TRANSFORMS_LIBRARY}")
message(STATUS "Found Cycles BVH Library: ${CYCLES_BVH_LIBRARY}") # Re-confirm this one

# List all Cycles libraries explicitly found
set(CYCLES_CORE_LIBS
    ${CYCLES_SESSION_LIBRARY}
    ${CYCLES_SCENE_LIBRARY}
    ${CYCLES_KERNEL_LIBRARY}
    ${CYCLES_DEVICE_LIBRARY}
    ${CYCLES_UTIL_LIBRARY}
    ${CYCLES_BVH_LIBRARY}
    ${CYCLES_OSL_LIBRARY}
    ${CYCLES_MATH_LIBRARY}
    ${CYCLES_TRANSFORMS_LIBRARY}
)
# Filter out any "NOTFOUND" strings from the list
list(FILTER CYCLES_CORE_LIBS INCLUDE REGEX ".*\\.a$|.*\\.so$")

message(STATUS "Cycles Core Libraries to link: ${CYCLES_CORE_LIBS}")

# Find gflags (required by libcycles_util.a)
find_library(GFLAGS_LIBRARY NAMES gflags libgflags.so.2.2 libgflags.so)
if(GFLAGS_LIBRARY)
  message(STATUS "Found GFlags library: ${GFLAGS_LIBRARY}")
else()
  message(WARNING "GFlags library not found, using default path /usr/lib64/libgflags.so.2.2")
  set(GFLAGS_LIBRARY "/usr/lib64/libgflags.so.2.2")
endif()

# Check for Alembic (required by libcycles_scene.a)
if(NOT ALEMBIC_LIBRARY)
  # Explicit fallback for Alembic
  find_library(ALEMBIC_LIBRARY NAMES Alembic libAlembic.so.1.8 libAlembic.so.1 libAlembic.so)
  if(ALEMBIC_LIBRARY)
    message(STATUS "Found Alembic library: ${ALEMBIC_LIBRARY}")
  else()
    message(WARNING "Alembic library not found, using default path /usr/lib64/libAlembic.so.1.8")
    set(ALEMBIC_LIBRARY "/usr/lib64/libAlembic.so.1.8")
  endif()
endif()

# Find Cycles node libraries (for ccl::Node::Node implementation)
find_library(CYCLES_NODE_LIBRARY NAMES cycles_node libcycles_node.a PATHS
  ${CMAKE_SOURCE_DIR}/lib
  ${CYCLES_ROOT}/lib
  ${BLENDER_LIB_DIR}
  /usr/lib/cycles
  /usr/local/lib/cycles
)
if(CYCLES_NODE_LIBRARY)
  message(STATUS "Found Cycles Node library: ${CYCLES_NODE_LIBRARY}")
else()
  message(WARNING "Cycles Node library not found, will try to use existing libraries")
endif()

# Ensure OpenImageIO libraries are properly linked (needed for Node system)
find_library(OPENIMAGEIO_LIBRARY NAMES OpenImageIO)
find_library(OPENIMAGEIO_UTIL_LIBRARY NAMES OpenImageIO_Util)
if(OPENIMAGEIO_LIBRARY)
  message(STATUS "Found OpenImageIO: ${OPENIMAGEIO_LIBRARY}")
else()
  message(WARNING "OpenImageIO library not found, linking may fail")
endif()



# OpenVDB dependencies (needed by OpenImageIO and potentially Cycles itself)
# Ensure these are found as full paths
set(OPENVDB_DEPS "")
if(BLOSC_LIBRARY)
  list(APPEND OPENVDB_DEPS ${BLOSC_LIBRARY})
endif()
if(BOOST_THREAD_LIBRARY)
  list(APPEND OPENVDB_DEPS ${BOOST_THREAD_LIBRARY})
endif()
if(BOOST_SYSTEM_LIBRARY)
  list(APPEND OPENVDB_DEPS ${BOOST_SYSTEM_LIBRARY})
endif()
if(BOOST_IOSTREAMS_LIBRARY)
  list(APPEND OPENVDB_DEPS ${BOOST_IOSTREAMS_LIBRARY})
endif()
if(BOOST_REGEX_LIBRARY)
  list(APPEND OPENVDB_DEPS ${BOOST_REGEX_LIBRARY})
endif()
if(TBB_LIBRARY) # Use the general TBB for OpenVDB too
  list(APPEND OPENVDB_DEPS ${TBB_LIBRARY})
endif()


# Alembic shared library and its dependencies
# Your script sets `ALEMBIC_SHARED_LIBRARY` directly, but find_library is better.
# Make sure ALEMBIC_LIBRARY contains the full path to libAlembic.so
set(ALEMBIC_DEPS "")
if(BOOST_FILESYSTEM_LIBRARY)
  list(APPEND ALEMBIC_DEPS ${BOOST_FILESYSTEM_LIBRARY})
endif()
if(IMATH_LIBRARY)
  list(APPEND ALEMBIC_DEPS ${IMATH_LIBRARY})
endif()
if(OPENEXR_LIBRARY) # OpenEXR is a common dependency for Alembic
  list(APPEND ALEMBIC_DEPS ${OPENEXR_LIBRARY})
endif()
if(ZLIB_LIBRARY) # ZLib also often needed
  list(APPEND ALEMBIC_DEPS ${ZLIB_LIBRARY})
endif()

# The crucial part: Linking order
# General rule: libraries that depend on others should come *before* their dependencies.
# More specific components should come before more general ones.
# Static libraries are especially sensitive to order.
target_link_libraries(cycles_test
    PRIVATE
        # Core SEP libraries (compiled by your project)
        sep_blender
        sep_core
        sep_quantum
        sep_memory
        sep_compat # compat library, critical for CUDA stubs if used
        sep_api    # API layer might also have dependencies

        # Cycles core libraries (from your ${CMAKE_SOURCE_DIR}/lib)
        # Order is critical here: libraries that depend on others first.
        # This is an educated guess based on typical Cycles dependencies:
        ${CYCLES_CORE_LIBS} # This variable now contains all resolved paths.

        # Required by libcycles_util.a
        ${GFLAGS_LIBRARY}
        
        # Required by libcycles_scene.a
        ${ALEMBIC_LIBRARY}
        
        # Blender internal libraries (static, order important)
        ${BF_BLENKERNEL_LIBRARY}
        ${BF_BLENLIB_LIBRARY}
        ${BF_DRAW_LIBRARY}
        ${BF_GPU_LIBRARY}
        ${BF_GUARDEDALLOC_LIBRARY}
        ${BF_INTERN_SKY_LIBRARY} # Sky module uses core Blender libs
        ${BF_INTERN_OPENSUBDIV_LIBRARY} # OpenSubdiv uses core Blender libs

        # OpenSubdiv libraries (often require specific TBB)
        ${OPENSUBDIV_CPU_LIBRARY}
        ${OPENSUBDIV_GPU_LIBRARY}
        ${TBB_OPENSUBDIV_LIBRARY} # Specific TBB for OpenSubdiv

        # External libraries (order might matter for some)
        # OpenVDB and its dependencies should come before OpenImageIO/OSL
        ${OPENVDB_LIBRARY} # Full OpenVDB library
        ${OPENVDB_DEPS}    # Its dependencies
        ${EMBREE_LIBRARY}  # Embree for ray tracing

        # OSL libraries
        ${OSL_COMP_LIBRARY}
        ${OSL_EXEC_LIBRARY}
        ${OSL_QUERY_LIBRARY}

        # OpenImageIO (depends on OpenEXR, OSL, OpenVDB, ZLib, etc.)
        ${OPENIMAGEIO_LIBRARY}
        ${OPENIMAGEIO_UTIL_LIBRARY}

        # Alembic (often depends on OpenEXR, Imath, Boost Filesystem/System)
        ${ALEMBIC_LIBRARY} # The resolved Alembic library
        ${ALEMBIC_DEPS}    # Its dependencies

        # Other external libraries
        ${ZSTD_LIBRARY}
        ${OPENCOLORIO_LIBRARY}
        ${OPENEXR_LIBRARY}
        ${GLOG_LIBRARY} # Google Logging
        ${ZLIB_LIBRARY} # ZLib

        # HIPEW/CUEW (should come before CUDA/HIP runtime if they provide wrappers)
        ${EXTERN_CUEW_LIBRARY}
        ${EXTERN_HIPEW_LIBRARY}

        # CUDA/HIP runtime libraries
        -lcuda # Generic CUDA driver library
        -lcudart # CUDA runtime library
        # If using HIP, also include:
        # -lhip_hcc
        # -lhip_runtime

        # Standard system libraries - usually last
        -lpthread
        -ldl
        -lm

        # General TBB for rest of system (if not already satisfied by OpenVDB/OpenSubdiv's TBB)
        ${TBB_LIBRARY}
)

# Add a custom target to run the test
add_custom_target(run_cycles_test
    COMMAND cycles_test ${CMAKE_BINARY_DIR}/cycles_render.ppm
    DEPENDS cycles_test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running Cycles renderer test"
)

message(STATUS "Cycles test application enabled (will use stub implementation if Cycles not available)")
