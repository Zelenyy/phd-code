CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

PROJECT(geant4-dfclient)

FIND_PACKAGE(Geant4 REQUIRED ui_all vis_all)


IF (NOT Geant4_FOUND)
    MESSAGE(FATAL_ERROR "Geant4 not found!")
ENDIF (NOT Geant4_FOUND)

INCLUDE(${Geant4_USE_FILE})

set(CMAKE_CONFIGURATION_TYPES Release)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ../proto/histogram.proto)


INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
FILE(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)


add_library(geant4-dfclient SHARED ${sources} ${PROTO_SRCS} ${PROTO_HDRS})

target_link_libraries(${PROJECT_NAME} ${Geant4_LIBRARIES} ${Protobuf_LIBRARIES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        )


INSTALL(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION .
        FILES_MATCHING PATTERN "*.hh")




ADD_EXECUTABLE(test.exe test.cc ${sources} ${headers} ${PROTO_SRCS}
        ${PROTO_HDRS})

target_include_directories(test.exe
        PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        )

TARGET_LINK_LIBRARIES(test.exe ${Geant4_LIBRARIES} ${Protobuf_LIBRARIES})

