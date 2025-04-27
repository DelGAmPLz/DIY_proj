# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)
set(EXTRA_COMPONENT_DIRS espressif__esp32-camera)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(OV2640_test)
