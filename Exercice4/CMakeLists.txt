cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/include)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})


# Packages requis.
FIND_PACKAGE( TBB ) 

# Création des exécutables.
add_executable(pearson src/pearson.cpp)

# Librairies avec lesquelles linker.
TARGET_LINK_LIBRARIES( pearson TBB::tbb )

# Faire parler le make.
set( CMAKE_VERBOSE_MAKEFILE off )

