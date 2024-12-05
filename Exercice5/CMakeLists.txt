# Version de cmake demandée.
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
 
# Chemin des répertoires contenant les fichiers entêtes.
INCLUDE_DIRECTORIES( src/include )

# Chemin du répertoire contenant les binaires.
SET ( EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE} )

# Activer OpenMP
FIND_PACKAGE(OpenMP REQUIRED)
if (OpenMP_CXX_FOUND)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# Création des exécutables.
ADD_EXECUTABLE( 
    Exercice5
    
    src/Metrics.cpp
    src/Exercice5Test.cpp )

# Lien avec OpenMP
TARGET_LINK_LIBRARIES(Exercice5 PRIVATE OpenMP::OpenMP_CXX)

# Faire parler le make.
set( CMAKE_VERBOSE_MAKEFILE off )