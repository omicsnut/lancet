message(STATUS "Current CMake version: ${CMAKE_VERSION}")
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

option(LANCET_UNIT_TESTS "Build lancet unit tests" "OFF")

project(
  lancet
  LANGUAGES C CXX
  HOMEPAGE_URL https://github.com/omicsnut/lancet
  DESCRIPTION "Microassembly based somatic variant caller")

# https://gitlab.kitware.com/cmake/cmake/-/issues/16716
set(PROJECT_VERSION "2.0.0.alpha0")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "Please run cmake in a separate build directory")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to Release as none was specified.")
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                                               "MinSizeRel" "RelWithDebInfo")
endif()

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)
find_package(LibLZMA REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(defaults)
include(external)

add_executable(
  lancet
  "src/align.hh"
  "src/align.cc"
  "src/ContigLink.hh"
  "src/ContigLink.cc"
  "src/Edge.hh"
  "src/Edge.cc"
  "src/ErrorCorrector.hh"
  "src/FET.hh"
  "src/Graph.hh"
  "src/Graph.cc"
  "src/Lancet.hh"
  "src/Lancet.cc"
  "src/Mer.hh"
  "src/Microassembler.hh"
  "src/Microassembler.cc"
  "src/Node.hh"
  "src/Node.cc"
  "src/Path.hh"
  "src/Path.cc"
  "src/ReadInfo.hh"
  "src/ReadStart.hh"
  "src/Ref.hh"
  "src/Ref.cc"
  "src/sha256.hh"
  "src/sha256.cc"
  "src/Transcript.hh"
  "src/util.hh"
  "src/util.cc"
  "src/Variant.hh"
  "src/Variant.cc"
  "src/VariantDB.hh"
  "src/VariantDB.cc")

add_dependencies(lancet htslib bamtools)
target_include_directories(
  lancet
  PUBLIC src SYSTEM
  PRIVATE ${HTSLIB_INCLUDE_DIR} ${BAMTOOLS_INCLUDE_DIR})

target_link_libraries(
  lancet
  PRIVATE ${LIBHTS}
          ${LIBBAMTOOLS}
          ZLIB::ZLIB
          BZip2::BZip2
          CURL::libcurl
          OpenSSL::Crypto
          LibLZMA::LibLZMA)

set_target_properties(
  lancet
  PROPERTIES CXX_STANDARD 17
             CXX_STANDARD_REQUIRED ON
             POSITION_INDEPENDENT_CODE 1)
