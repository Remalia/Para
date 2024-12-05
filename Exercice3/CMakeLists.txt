# Version de cmake demandée.
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
 
# Chemin des répertoires contenant les fichiers entêtes.
INCLUDE_DIRECTORIES( src/include )

# Packages requis.
FIND_PACKAGE( TBB ) 

# Chemin du répertoire contenant les binaires.
SET ( EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE} )

# Création des exécutables.
ADD_EXECUTABLE(Exercice3
               src/Metrics.cpp
               src/Exercice3Test.cpp)

# Librairies avec lesquelles linker.
TARGET_LINK_LIBRARIES( Exercice3 TBB::tbb )

# Faire parler le make.
set( CMAKE_VERBOSE_MAKEFILE off )
