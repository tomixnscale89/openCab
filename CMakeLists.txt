cmake_minimum_required(VERSION 3.13)

project(CABPC CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/bin" )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/bin" )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/bin" )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

set(BIN_DIR ${CMAKE_BINARY_DIR}/bin)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(TMCC REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${TMCC_INCLUDE_DIRS})
include_directories(src/imgui)



set(CABPC_SOURCES
    src/imgui/imconfig.h
    src/imgui/imgui.cpp
    src/imgui/imgui.h
    src/imgui/misc/cpp/imgui_stdlib.cpp
    src/imgui/misc/cpp/imgui_stdlib.h
    src/imgui/imgui_demo.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_impl_opengl3.cpp
    src/imgui/imgui_impl_opengl3.h
    src/imgui/imgui_impl_opengl3_loader.h
    src/imgui/imgui_impl_sdl.cpp
    src/imgui/imgui_impl_sdl.h
    src/imgui/imgui_internal.h
    src/imgui/imgui_tables.cpp
    src/imgui/imgui_widgets.cpp
    src/imgui/imstb_rectpack.h
    src/imgui/imstb_textedit.h
    src/imgui/imstb_truetype.h
    src/EngineManagement.cpp
    src/EngineManagement.h    
    src/Image.cpp
    src/Image.h
    src/stb_image.h
    src/json.hpp
    src/imgui-knobs.cpp
    src/imgui-knobs.h
    src/interface/throttlemenu.cpp
    src/interface/throttlemenu.h
    src/interface/cab1menu.cpp
    src/interface/cab1menu.h    
    src/interface/KeypadTypes.cpp
    src/interface/KeypadTypes.h
    src/surface/DeviceListener.cpp
    src/surface/DeviceListener.h
    src/surface/stdafx.cpp
    src/surface/stdafx.h
    src/surface/targetver.h
    src/CABPC.cpp)

add_executable(CABPC ${CABPC_SOURCES})
target_link_libraries(CABPC PUBLIC opengl32 ${SDL2_LIBRARIES} ${TMCC_LIBRARIES})


foreach(source IN LISTS CABPC_SOURCES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()