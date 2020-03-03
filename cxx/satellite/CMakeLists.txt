CMAKE_MINIMUM_REQUIRED(VERSION 3.3 FATAL_ERROR)

PROJECT(geant4-satellite)


FIND_PACKAGE(Geant4 REQUIRED ui_all vis_all)


IF(NOT Geant4_FOUND)
    MESSAGE(FATAL_ERROR "Geant4 not found!")
ENDIF(NOT Geant4_FOUND)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS satellite.proto)

INCLUDE(${Geant4_USE_FILE})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../general/include)
#INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../dataforge/include)
FILE(GLOB general ${PROJECT_SOURCE_DIR}/../general/src/*.cc)
#FILE(GLOB dataforge ${PROJECT_SOURCE_DIR}/../dataforge/src/*.cc)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
FILE(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

ADD_EXECUTABLE(geant4-satellite.exe main.cc
        ${sources} ${general}
        )

TARGET_LINK_LIBRARIES(geant4-satellite.exe
        ${Geant4_LIBRARIES} ${Protobuf_LIBRARIES}
)
