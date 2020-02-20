CMAKE_MINIMUM_REQUIRED(VERSION 3.3 FATAL_ERROR)

PROJECT(test-hist)

include_directories(../../general/include/)

ADD_EXECUTABLE(testHist.exe testHist.cc)
