CMAKE_MINIMUM_REQUIRED(VERSION 3.3 FATAL_ERROR)

PROJECT(geant4-thunderstorm)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ../proto/thunderstorm.proto ../proto/histogram.proto)

# protobuf_generate_python(PY_SOURCES thunderstorm.proto)
# add_custom_target(py_proto ALL DEPENDS ${PY_SOURCES})

FIND_PACKAGE(Geant4 REQUIRED ui_all vis_all)



IF(NOT Geant4_FOUND)
    MESSAGE(FATAL_ERROR "Geant4 not found!")
ENDIF(NOT Geant4_FOUND)


INCLUDE(${Geant4_USE_FILE})



INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../general/include)
FILE(GLOB general ${PROJECT_SOURCE_DIR}/../general/src/*.cc )

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
FILE(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)




ADD_EXECUTABLE(geant4-thunderstorm.exe main.cc
        ${sources} ${general}
        ${PROTO_SRCS}
        ${PROTO_HDRS})

target_include_directories(geant4-thunderstorm.exe
        PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        )

TARGET_LINK_LIBRARIES(geant4-thunderstorm.exe
        ${Geant4_LIBRARIES} ${Protobuf_LIBRARIES}
)