cmake_minimum_required (VERSION 3.4)
project (avsfilters)

option(BUILD_WHITEPOINT "Build WhitePoint plugin" YES)
option(BUILD_AUTOGAMMA "Build AutoGamma plugin" YES)

if(BUILD_WHITEPOINT)
  add_subdirectory("C White Point")
endif()

if(BUILD_AUTOGAMMA)
  add_subdirectory("C_Auto_Gamma")
endif()

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
