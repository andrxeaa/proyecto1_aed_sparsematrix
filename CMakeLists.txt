cmake_minimum_required(VERSION 3.10)
project(HojaCalculoDispersa)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Definir los archivos fuente
set(SOURCES
    main.cpp
    Spreadsheet.cpp
    Node.cpp
    FormulaEvaluator.cpp
    GUI.cpp
)

# Intentar encontrar SFML
# Nota para CLion en Windows: Si usas MinGW, debes tener instalada la versión de SFML para MinGW.
# Apuntamos CMake a la carpeta SFML que acabo de descargar localmente
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Crear el ejecutable
add_executable(spreadsheet_app ${SOURCES})

# Enlazar las librerías de SFML
target_link_libraries(spreadsheet_app PRIVATE sfml-graphics sfml-window sfml-system)

# Copiar las DLLs necesarias al directorio del ejecutable
add_custom_command(TARGET spreadsheet_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/SFML/bin"
    $<TARGET_FILE_DIR:spreadsheet_app>
)

# Copiar la fuente al directorio del ejecutable
add_custom_command(TARGET spreadsheet_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/arial.ttf"
    $<TARGET_FILE_DIR:spreadsheet_app>
)
