# Spécifie la version minimale de CMake requise pour les fonctionnalités modernes
cmake_minimum_required(VERSION 3.10)

# Définit le nom de votre projet et les langues utilisées
project(MolecularDynamics CXX)

# --- Configuration du Standard C++ ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# --- Définition des Fichiers Sources (.cpp) ---
# Liste explicite de tous les fichiers sources pour assurer la robustesse

set(PROJECT_SOURCES
    main.cpp
    src/OutputManager/OutputManager.cpp
    src/OutputManager/DataWriter/LogWriter.cpp
    src/OutputManager/DataWriter/TrajectoryWriter.cpp
    src/MathUtils/minimumImageDistance.cpp
    src/Integrator/Verlet.cpp
    src/DynamicEngine/DynamicEngine.cpp
    src/SystemLoader/SystemLoader.cpp
    src/ForceCalculator/ForceCalculator.cpp
    src/ForceCalculator/Force/LennardJones.cpp
    src/ForceCalculator/Force/HarmonicBond.cpp
    src/Atom/Atom.cpp
    src/Topology/Topology.cpp
    src/Topology/Geometry/Dihedral.cpp
    src/Topology/Geometry/Bond.cpp
    src/Topology/Geometry/Angular.cpp
    src/System/System.cpp
)

# --- Configuration des Dépendances Externes ---

# Seule Eigen3 est requise
find_package(Eigen3 REQUIRED)

# --- Création de l'Exécutable ---

# Crée l'exécutable à partir de la liste complète des sources
add_executable(MD_Simulator ${PROJECT_SOURCES})

# --- Configuration des Chemins d'Inclusion (.hpp) et des Librairies ---

# 1. Chemins d'inclusion :
target_include_directories(MD_Simulator PUBLIC
    # 1. Inclure la racine du projet (pour main.cpp)
    ${CMAKE_SOURCE_DIR}
    
    # 2. Répertoire 'include'
    ${CMAKE_SOURCE_DIR}/include
    
    # 3. Répertoire 'src'
    ${CMAKE_SOURCE_DIR}/src
    
    # Chemins d'Eigen
    ${EIGEN3_INCLUDE_DIRS}
    
    # Les chemins GROMACS inutiles ont été supprimés ici.
)

# 2. Liens des Librairies :
# Correction du formatage pour éliminer l'erreur -lPRIVATE
target_link_libraries(MD_Simulator PRIVATE
    Eigen3::Eigen
    # La liaison à -lgromacs a été supprimée.
)
