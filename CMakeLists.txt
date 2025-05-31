cmake_minimum_required(VERSION 3.10)
project(SFML_Project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

find_package(SFML 3 COMPONENTS REQUIRED Audio Graphics Window Network System)
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

# Create executable
add_executable(sfml-app ${SOURCES})

#Tells the linker what specific library files to link, it will look in the directories you specified with link_directories
target_link_libraries(sfml-app SFML::Audio SFML::Graphics SFML::Window SFML::Network SFML::System)