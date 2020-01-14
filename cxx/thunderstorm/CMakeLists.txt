CMAKE_MINIMUM_REQUIRED(VERSION 3.3 FATAL_ERROR)

PROJECT(geant4-thunderstorm)

FIND_PACKAGE(Geant4 REQUIRED ui_all vis_all)

IF(NOT Geant4_FOUND)
    MESSAGE(FATAL_ERROR "Geant4 not found!")
ENDIF(NOT Geant4_FOUND)


INCLUDE(${Geant4_USE_FILE})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../../g4npm/include)

find_library(geant4-dfclient NAMES geant4-dfclient         PATHS ${PROJECT_SOURCE_DIR}/../../../g4npm/lib)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
FILE(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)



ADD_EXECUTABLE(geant4-thunderstorm.exe main.cc
        ${sources}

)

TARGET_LINK_LIBRARIES(geant4-thunderstorm.exe
        ${Geant4_LIBRARIES}
        ${geant4-dfclient}
)
