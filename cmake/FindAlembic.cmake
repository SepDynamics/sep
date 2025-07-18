# FindAlembic.cmake
# Find the Alembic library and its headers.
#
# This module defines:
#  Alembic_FOUND - True if Alembic is found
#  Alembic_INCLUDE_DIRS - The Alembic include directory
#  Alembic_LIBRARIES - The libraries needed to use Alembic
#  Alembic::Abc - Imported target for the main Alembic library
#  Alembic::AbcGeom - Imported target for Alembic's geometry components
#  Alembic::AbcCoreAbstract - Imported target for Alembic's core abstract components
#  Alembic::AbcCoreFactory - Imported target for Alembic's core factory components
#
# This module will look for Alembic in standard system locations and can be
# influenced by setting the ALEMBIC_ROOT environment variable.

# Try to find Alembic using pkg-config first
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PC_ALEMBIC QUIET Alembic_Abc) # Assuming pkg-config might provide a general Alembic module
endif()

# Find the Alembic include directory
find_path(ALEMBIC_INCLUDE_DIR
  NAMES Alembic/Abc/All.h
  PATHS
    ${PC_ALEMBIC_INCLUDE_DIRS}
    $ENV{ALEMBIC_ROOT}/include
    $ENV{ALEMBIC_DIR}/include
    $ENV{ALEMBIC_PATH}/include
    /usr/include
    /usr/local/include
    /opt/local/include
    /usr/include/Alembic
    /usr/local/include/Alembic
  DOC "Alembic include directory"
)

# If we still can't find it, try a simpler approach
if(NOT ALEMBIC_INCLUDE_DIR AND EXISTS "/usr/include/Alembic")
  set(ALEMBIC_INCLUDE_DIR "/usr/include")
  message(STATUS "Setting Alembic include directory to /usr/include")
endif()

# First try to find the unified Alembic library (used on some systems like Fedora)
find_library(ALEMBIC_UNIFIED_LIBRARY
  NAMES Alembic alembic
  PATHS
    ${PC_ALEMBIC_LIBDIR}
    $ENV{ALEMBIC_ROOT}/lib
    $ENV{ALEMBIC_ROOT}/lib64
    /usr/lib
    /usr/lib64
    /usr/local/lib
    /usr/local/lib64
    /opt/local/lib
  DOC "Unified Alembic library"
)

# If the unified library is found, use it for all components
if(ALEMBIC_UNIFIED_LIBRARY)
  set(ALEMBIC_ABC_LIBRARY ${ALEMBIC_UNIFIED_LIBRARY})
  set(ALEMBIC_ABCGEOM_LIBRARY ${ALEMBIC_UNIFIED_LIBRARY})
  set(ALEMBIC_ABCCOREABSTRACT_LIBRARY ${ALEMBIC_UNIFIED_LIBRARY})
  set(ALEMBIC_ABCCOREFACTORY_LIBRARY ${ALEMBIC_UNIFIED_LIBRARY})
  message(STATUS "Found unified Alembic library: ${ALEMBIC_UNIFIED_LIBRARY}")
else()
  # Otherwise, try to find individual component libraries
  # Find the core Alembic library
  find_library(ALEMBIC_ABC_LIBRARY
    NAMES Alembic_Abc Abc
    PATHS
      ${PC_ALEMBIC_LIBDIR}
      $ENV{ALEMBIC_ROOT}/lib
      $ENV{ALEMBIC_ROOT}/lib64
      /usr/lib
      /usr/lib64
      /usr/local/lib
      /usr/local/lib64
      /opt/local/lib
    DOC "Alembic Abc library"
  )

  # Find the Alembic Geom library
  find_library(ALEMBIC_ABCGEOM_LIBRARY
    NAMES Alembic_AbcGeom AbcGeom
    PATHS
      ${PC_ALEMBIC_LIBDIR}
      $ENV{ALEMBIC_ROOT}/lib
      $ENV{ALEMBIC_ROOT}/lib64
      /usr/lib
      /usr/lib64
      /usr/local/lib
      /usr/local/lib64
      /opt/local/lib
    DOC "Alembic AbcGeom library"
  )

  # Find the Alembic Core Abstract library
  find_library(ALEMBIC_ABCCOREABSTRACT_LIBRARY
    NAMES Alembic_AbcCoreAbstract AbcCoreAbstract
    PATHS
      ${PC_ALEMBIC_LIBDIR}
      $ENV{ALEMBIC_ROOT}/lib
      $ENV{ALEMBIC_ROOT}/lib64
      /usr/lib
      /usr/lib64
      /usr/local/lib
      /usr/local/lib64
      /opt/local/lib
    DOC "Alembic AbcCoreAbstract library"
  )

  # Find the Alembic Core Factory library
  find_library(ALEMBIC_ABCCOREFACTORY_LIBRARY
    NAMES Alembic_AbcCoreFactory AbcCoreFactory
    PATHS
      ${PC_ALEMBIC_LIBDIR}
      $ENV{ALEMBIC_ROOT}/lib
      $ENV{ALEMBIC_ROOT}/lib64
      /usr/lib
      /usr/lib64
      /usr/local/lib
      /usr/local/lib64
      /opt/local/lib
    DOC "Alembic AbcCoreFactory library"
  )
endif()

# Check if all required components are found
include(FindPackageHandleStandardArgs)

# If we found the unified library, only require that and the include dir
if(ALEMBIC_UNIFIED_LIBRARY)
  find_package_handle_standard_args(Alembic
    REQUIRED_VARS
      ALEMBIC_INCLUDE_DIR
      ALEMBIC_UNIFIED_LIBRARY
  )
else()
  # Otherwise require all component libraries
  find_package_handle_standard_args(Alembic
    REQUIRED_VARS
      ALEMBIC_INCLUDE_DIR
      ALEMBIC_ABC_LIBRARY
      ALEMBIC_ABCGEOM_LIBRARY
      ALEMBIC_ABCCOREABSTRACT_LIBRARY
      ALEMBIC_ABCCOREFACTORY_LIBRARY
  )
endif()

if(Alembic_FOUND)
  set(Alembic_LIBRARIES
    ${ALEMBIC_ABC_LIBRARY}
    ${ALEMBIC_ABCGEOM_LIBRARY}
    ${ALEMBIC_ABCCOREABSTRACT_LIBRARY}
    ${ALEMBIC_ABCCOREFACTORY_LIBRARY}
  )
  set(Alembic_INCLUDE_DIRS ${ALEMBIC_INCLUDE_DIR})

  # Create imported targets
  if(NOT TARGET Alembic::Abc)
    add_library(Alembic::Abc UNKNOWN IMPORTED)
    set_target_properties(Alembic::Abc PROPERTIES
      IMPORTED_LOCATION "${ALEMBIC_ABC_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${ALEMBIC_INCLUDE_DIR}"
    )
  endif()
  if(NOT TARGET Alembic::AbcGeom)
    add_library(Alembic::AbcGeom UNKNOWN IMPORTED)
    set_target_properties(Alembic::AbcGeom PROPERTIES
      IMPORTED_LOCATION "${ALEMBIC_ABCGEOM_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${ALEMBIC_INCLUDE_DIR}"
      INTERFACE_LINK_LIBRARIES "Alembic::Abc" # AbcGeom depends on Abc
    )
  endif()
  if(NOT TARGET Alembic::AbcCoreAbstract)
    add_library(Alembic::AbcCoreAbstract UNKNOWN IMPORTED)
    set_target_properties(Alembic::AbcCoreAbstract PROPERTIES
      IMPORTED_LOCATION "${ALEMBIC_ABCCOREABSTRACT_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${ALEMBIC_INCLUDE_DIR}"
    )
  endif()
  if(NOT TARGET Alembic::AbcCoreFactory)
    add_library(Alembic::AbcCoreFactory UNKNOWN IMPORTED)
    set_target_properties(Alembic::AbcCoreFactory PROPERTIES
      IMPORTED_LOCATION "${ALEMBIC_ABCCOREFACTORY_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${ALEMBIC_INCLUDE_DIR}"
      INTERFACE_LINK_LIBRARIES "Alembic::AbcCoreAbstract" # AbcCoreFactory depends on AbcCoreAbstract
    )
  endif()

  message(STATUS "Found Alembic: ${Alembic_LIBRARIES}")
  message(STATUS "Alembic include dirs: ${Alembic_INCLUDE_DIRS}")
else()
  message(STATUS "Alembic not found. Alembic-related functionality in Blender module may be disabled.")
  set(Alembic_LIBRARIES "")
  set(Alembic_INCLUDE_DIRS "")
endif()

mark_as_advanced(
  ALEMBIC_INCLUDE_DIR
  ALEMBIC_ABC_LIBRARY
  ALEMBIC_ABCGEOM_LIBRARY
  ALEMBIC_ABCCOREABSTRACT_LIBRARY
  ALEMBIC_ABCCOREFACTORY_LIBRARY
)