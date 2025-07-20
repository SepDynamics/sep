@PACKAGE_INIT@

include(CMakeFindDependencyMacro)

# Find dependencies
find_dependency(Threads)
find_dependency(CURL)
find_dependency(fmt)
find_dependency(http_parser)
find_dependency(spdlog)
# find_dependency(OpenVDB) # Removed - not needed
find_dependency(OpenSubdiv)
find_dependency(OpenPGL)
find_dependency(OpenColorIO)
find_dependency(Imath)
find_dependency(TBB)
find_dependency(OpenImageIO)
find_dependency(embree)
find_dependency(OSL)
find_dependency(Alembic)

# Include targets file
include("${CMAKE_CURRENT_LIST_DIR}/sep-targets.cmake")