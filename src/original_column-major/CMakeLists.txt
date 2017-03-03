cmake_minimum_required (VERSION 2.6)
project (mhm_matrix_driver)
enable_language (C)

# The version number.
set (driver_VERSION_MAJOR 1)
set (driver_VERSION_MINOR 0)

# make sure that the default is a RELEASE
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE RELEASE CACHE STRING
      "Choose the type of build, options are: None Debug Release."
      FORCE)
endif (NOT CMAKE_BUILD_TYPE)

set (CMAKE_C_COMPILER gcc)

# FFLAGS depend on the compiler
get_filename_component (C_COMPILER_NAME ${CMAKE_C_COMPILER} NAME)

 # gfortran
set (CMAKE_C_FLAGS_RELEASE "-O2")  
message ("CMAKE_C_COMPILER full path: " ${CMAKE_C_COMPILER})
message ("C compiler: " ${C_COMPILER_NAME})

set (CMAKE_USE_RELATIVE_PATHS ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/driver.h.in"
  "${PROJECT_BINARY_DIR}/driver.h"
  )

include_directories (${PROJECT_BINARY_DIR})

# add the executable
add_executable (merge mmio.c merge.c)
add_executable (mergerhs mmio.c mergerhs.c)

