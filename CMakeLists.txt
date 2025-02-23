cmake_minimum_required(VERSION 3.10)
project(CPMULTI)

# Spécifie le chemin de OpenCV
set(OpenCV_DIR "C:/opencv-4.11.0/opencv-4.11.0/build")

# Trouve OpenCV
find_package(OpenCV REQUIRED)

# Ajoute l'exécutable
add_executable(CPMULTI main.cpp)

# Ajoute les include de OpenCV
target_include_directories(CPMULTI PRIVATE ${OpenCV_INCLUDE_DIRS})

# Ajoute les bibliothèques OpenCV
target_link_libraries(CPMULTI PRIVATE ${OpenCV_LIBS})

# Debug : Afficher les bibliothèques trouvées
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")
