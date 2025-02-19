CMAKE_MINIMUM_REQUIRED(VERSION 3.7)

project(tipc)

# Required packages
find_package(PkgConfig REQUIRED)
#pkg_search_module(UUID REQUIRED uuid)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/externals/CMake-codecov/cmake")

# compiler must be 17
set(CMAKE_CXX_STANDARD 17)

# compile for debug if no other type is set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# cleanup the code with clang-tidy
#set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

######## Code Coverage rules ###########
add_library(coverage_config INTERFACE)

option (CODE_COVERAGE "Enable coverage reporting." ON)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  message(STATUS "Building with coverage reporting enabled")
  # Add required flags (GCC & LLVM/Clang)
  target_compile_options(coverage_config INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(coverage_config INTERFACE --coverage)
  else()
    target_link_libraries(coverage_config INTERFACE --coverage)
  endif()
endif()

######## ANTLR4 CPP target rules ###########
# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)

set(ANTLR4_WITH_STATIC_CRT OFF)
set(ANTLR4_TAG 4.8)
include(ExternalAntlr4Cpp)
include_directories(${ANTLR4_INCLUDE_DIRS})

# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR)
if(NOT ANTLR_FOUND)
  set(ANTLR_JAR antlr-4.8-complete.jar)
  set(ANTLR_JAR_URL https://www.antlr.org/download/${ANTLR_JAR})
  message(STATUS "Downloading antlr jar")
  file(DOWNLOAD ${ANTLR_JAR_URL} ${CMAKE_BINARY_DIR}/include/${ANTLR_JAR})
  message(STATUS "Downloading complete")
  list(APPEND CMAKE_PROGRAM_PATH "${CMAKE_BINARY_DIR}/include")
  find_package(ANTLR REQUIRED)
endif()

######## LLVM rules for installed libraries ###########
# We want to select the latest stable release even if others are installed
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  find_package(LLVM 11 REQUIRED CONFIG)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  find_package(LLVM 11.0.0 REQUIRED CONFIG)
else()
  message(FATAL_ERROR "${CMAKE_SYSTEM_NAME} is not supported")
endif()

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Logging.
add_library(loguru)
target_sources(loguru PUBLIC
        ${CMAKE_SOURCE_DIR}/externals/loguru/loguru.cpp
        ${CMAKE_SOURCE_DIR}/externals/loguru/loguru.hpp
        )
find_package(Threads REQUIRED)
target_link_libraries(loguru ${CMAKE_THREAD_LIBS_INIT})
if(NOT WIN32)
  target_link_libraries(loguru dl)
endif()
add_definitions(-DLOGURU_WITH_STREAMS=1 -DLOGURU_VERBOSE_SCOPE_ENDINGS=0)
include_directories(${CMAKE_SOURCE_DIR}/externals/loguru)

######## build target including unit tests ###########
add_subdirectory(src)

option (BUILD_TESTING "Build the testing tree." ON)
# Only build tests if we are the top-level project
# Allows this to be used by super projects with `add_subdirectory`
if (BUILD_TESTING AND (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
  enable_testing()
  add_subdirectory(test)
endif()
