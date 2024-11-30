# Version de cmake demandée.
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
 
# Chemin des répertoires contenant les fichiers entêtes.
INCLUDE_DIRECTORIES( src/include )

# Chemin du répertoire contenant les binaires.
SET ( EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE} )

# Création des exécutables.
ADD_EXECUTABLE( Question1 src/Question1Test.cpp )
ADD_EXECUTABLE( Question2 src/Question2Test.cpp )
ADD_EXECUTABLE( Question3 src/Question3Test.cpp )

# Faire parler le make.
set( CMAKE_VERBOSE_MAKEFILE off )