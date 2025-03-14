cmake_minimum_required(VERSION 3.10)
project(SFML_Project)

set(CMAKE_CXX_STANDARD 17)


find_package(SFML 3 COMPONENTS REQUIRED Audio Graphics Window Network System)

set(SOURCES 
    src/main.cpp
    src/AnimUtil.cpp
    src/TextureUtil.cpp
    src/Player.cpp
    src/Map.cpp
    src/Monster.cpp
    src/Character.cpp
    src/Projectile.cpp    
    src/Weapon.cpp
    src/Blood.cpp
    src/Footprint.cpp
    src/Hud.cpp
    src/MonsterFactory.cpp
    src/RandomUtil.cpp
    src/MuzzleFlash.cpp
    src/HitboxDebugger.cpp
    src/AK47.cpp
    src/Famas.cpp
    src/Barrett50.cpp
    src/Rpg.cpp
    src/Shell.cpp
    src/BatchRenderer.cpp
    src/RenderUtil.cpp
)
add_subdirectory(external/tinyxml2)

# Create executable
add_executable(sfml-app ${SOURCES})

#Tells the linker what specific library files to link, it will look in the directories you specified with link_directories
target_link_libraries(sfml-app SFML::Audio SFML::Graphics SFML::Window SFML::Network SFML::System)
target_link_libraries(sfml-app tinyxml2)