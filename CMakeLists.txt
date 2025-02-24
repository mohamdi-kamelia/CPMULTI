cmake_minimum_required(VERSION 3.10)
project(CPMULTI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Spécifie le chemin de OpenCV si nécessaire
set(OpenCV_DIR "C:/opencv-4.11.0/opencv-4.11.0/build")

# Trouve OpenCV
find_package(OpenCV REQUIRED)

# Ajoute les dossiers include pour trouver les fichiers .hpp
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/traitement
    ${CMAKE_SOURCE_DIR}/multithreading
    ${CMAKE_SOURCE_DIR}/utilitaires
)

# Ajoute les fichiers sources
add_executable(CPMULTI 
    main.cpp 
    traitement/traitement.cpp 
    multithreading/multithreading.cpp 
    utilitaires/utils.cpp
)

# Ajoute les bibliothèques OpenCV
target_link_libraries(CPMULTI PRIVATE ${OpenCV_LIBS})

# Debug : Afficher les bibliothèques trouvées
message(STATUS "✅ OpenCV trouvé, bibliothèques utilisées: ${OpenCV_LIBS}")
