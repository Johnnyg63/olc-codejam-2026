# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 4.1.0)

# Reject any attempt to use a toolchain file. We must not use one because
# we could be downloading it here. If the CMAKE_TOOLCHAIN_FILE environment
# variable is set, the cache variable will have been initialized from it.
unset(CMAKE_TOOLCHAIN_FILE CACHE)
unset(ENV{CMAKE_TOOLCHAIN_FILE})

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(olcutil3_camera2d-populate NONE)



include(ExternalProject)
ExternalProject_Add(olcutil3_camera2d-populate
                     "UPDATE_DISCONNECTED" "False" "EXTERNALPROJECT_INTERNAL_ARGUMENT_SEPARATOR" "URL" "https://raw.githubusercontent.com/OneLoneCoder/olcPixelGameEngine3/develop/utilities/olcUTIL3_Camera2D.h" DOWNLOAD_NO_EXTRACT YES
                    SOURCE_DIR          "/Users/mickymacm4/Documents/olc-codejam-2026/xcode/_deps/olcpixelgameengine3-src/utilities"
                    BINARY_DIR          "/Users/mickymacm4/Documents/olc-codejam-2026/xcode/_deps/olcutil3_camera2d-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


ExternalProject_Get_Property(olcutil3_camera2d-populate DOWNLOADED_FILE)
get_filename_component(dlFileName "${DOWNLOADED_FILE}" NAME)

ExternalProject_Add_Step(olcutil3_camera2d-populate copyfile
  COMMAND    "/opt/homebrew/bin/cmake" -E copy_if_different
             "<DOWNLOADED_FILE>" "/Users/mickymacm4/Documents/olc-codejam-2026/xcode/_deps/olcpixelgameengine3-src/utilities"
  DEPENDEES  patch
  DEPENDERS  configure
  BYPRODUCTS "/Users/mickymacm4/Documents/olc-codejam-2026/xcode/_deps/olcpixelgameengine3-src/utilities/${dlFileName}"
  COMMENT    "Copying file to SOURCE_DIR"
)

