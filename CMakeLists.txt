cmake_minimum_required(VERSION 3.15)

project(Infecdead LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Files
set(HEADERS
	common.hpp
	game/game.hpp
	graphics/text.hpp graphics/texture.hpp
	utility/log.hpp utility/utility.hpp
)
set(SOURCES
	main.cpp
	game/game.cpp
	graphics/text.cpp graphics/texture.cpp
	utility/log.cpp
)
list(TRANSFORM HEADERS PREPEND src/)
list(TRANSFORM SOURCES PREPEND src/)

# Dependencies
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(dep/glm)

set(BUILD_SHARED_LIBS OFF)
set(SDL_TEST OFF)
add_subdirectory(dep/SDL)

set(BUILD_SHARED_LIBS OFF)
set(SDL2IMAGE_SAMPLES OFF)
set(SDL2IMAGE_VENDORED ON)
add_subdirectory(dep/SDL_image)

set(BUILD_SHARED_LIBS OFF)
set(SDL2TTF_SAMPLES OFF)
set(SDL2TTF_VENDORED ON)
add_subdirectory(dep/SDL_ttf)

# The executable
add_executable(Infecdead ${HEADERS} ${SOURCES})
target_include_directories(Infecdead PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(Infecdead SDL2::SDL2main SDL2::SDL2-static SDL2_image::SDL2_image-static SDL2_ttf::SDL2_ttf-static)
target_link_libraries(Infecdead glm)

# Copies resource files to the output directory
add_custom_command(TARGET Infecdead POST_BUILD COMMAND
				   ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:Infecdead>/res/)

# Compiler specific configuration
if (MSVC)
	set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Infecdead)
	set_target_properties(Infecdead PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

	target_compile_options(Infecdead PUBLIC /J)
else()
	target_compile_options(Infecdead PUBLIC -funsigned-char)
endif()
