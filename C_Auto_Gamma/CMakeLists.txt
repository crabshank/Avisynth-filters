cmake_minimum_required (VERSION 3.4)
project(autogamma)

FILE(GLOB autogamma_sources RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "functions_c.h"
    "auto_gamma.c"
    "../avisynth_c.h"
    "../avs/*.h"
)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib)
add_library("autogamma" SHARED "${autogamma_sources}")
set_target_properties("autogamma" PROPERTIES "OUTPUT_NAME" "autogamma")
if (MINGW)
    set_target_properties("autogamma" PROPERTIES PREFIX "")
    set_target_properties("autogamma" PROPERTIES IMPORT_PREFIX "")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_link_libraries(autogamma "AviSynth_x64.lib")
else()
    target_link_libraries(autogamma "AviSynth_x86.lib")
endif()

set(RUNTIME_INSTALL_DIR bin CACHE STRING "Install location of Windows DLLs")
set(LIB_INSTALL_DIR lib CACHE STRING "Install location of libraries")
set(ARCHIVE_INSTALL_DIR lib CACHE STRING "Install location of static libraries")

INSTALL(TARGETS autogamma
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/${RUNTIME_INSTALL_DIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}")
