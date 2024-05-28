cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(compiler)
find_package(LLVM 16 REQUIRED CONFIG)
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

include(ExternalProject)

set(ANTLR4_ROOT ${CMAKE_CURRENT_BINARY_DIR}/antlr4_runtime/src/antlr4_runtime)
set(ANTLR4_INCLUDE_DIRS ${ANTLR4_ROOT}/runtime/Cpp/runtime/src)
set(ANTLR4_GIT_REPOSITORY https://github.com/antlr/antlr4.git)

if(NOT DEFINED ANTLR4_TAG)
  # Set to branch name to keep library updated at the cost of needing to rebuild after 'clean'
  # Set to commit hash to keep the build stable and does not need to rebuild after 'clean'
  set(ANTLR4_TAG 4.13.1)
endif()

set(ANTLR4_OUTPUT_DIR ${ANTLR4_ROOT}/runtime/Cpp/dist)

set(ANTLR4_STATIC_LIBRARIES ${ANTLR4_OUTPUT_DIR}/libantlr4-runtime.a)
set(ANTLR4_RUNTIME_LIBRARIES ${ANTLR4_OUTPUT_DIR}/libantlr4-runtime.so)

if(${CMAKE_GENERATOR} MATCHES ".* Makefiles")
  # This avoids
  # 'warning: jobserver unavailable: using -j1. Add '+' to parent make rule.'
  set(ANTLR4_BUILD_COMMAND $(MAKE))
else()
  set(ANTLR4_BUILD_COMMAND
      ${CMAKE_COMMAND}
          --build .
          --target)
endif()

if(NOT DEFINED ANTLR4_WITH_STATIC_CRT)
  set(ANTLR4_WITH_STATIC_CRT ON)
endif()

ExternalProject_Add(
      antlr4_runtime
      PREFIX antlr4_runtime
      GIT_REPOSITORY ${ANTLR4_GIT_REPOSITORY}
      GIT_TAG ${ANTLR4_TAG}
      DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}
      BUILD_COMMAND ""
      BUILD_IN_SOURCE 1
      SOURCE_DIR ${ANTLR4_ROOT}
      SOURCE_SUBDIR runtime/Cpp
      CMAKE_CACHE_ARGS
          -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
          -DWITH_STATIC_CRT:BOOL=${ANTLR4_WITH_STATIC_CRT}
      INSTALL_COMMAND ""
      EXCLUDE_FROM_ALL 1)

# Separate build step as rarely people want both
set(ANTLR4_BUILD_DIR ${ANTLR4_ROOT})
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14.0")
  # CMake 3.14 builds in above's SOURCE_SUBDIR when BUILD_IN_SOURCE is true
  set(ANTLR4_BUILD_DIR ${ANTLR4_ROOT}/runtime/Cpp)
endif()

ExternalProject_Add_Step(
    antlr4_runtime
    build_static
    COMMAND ${ANTLR4_BUILD_COMMAND} antlr4_static
    # Depend on target instead of step (a custom command)
    # to avoid running dependent steps concurrently
    DEPENDS antlr4_runtime
    BYPRODUCTS ${ANTLR4_STATIC_LIBRARIES}
    EXCLUDE_FROM_MAIN 1
    WORKING_DIRECTORY ${ANTLR4_BUILD_DIR})
ExternalProject_Add_StepTargets(antlr4_runtime build_static)

add_library(antlr4_static STATIC IMPORTED)
add_dependencies(antlr4_static antlr4_runtime-build_static)
set_target_properties(antlr4_static PROPERTIES
                      IMPORTED_LOCATION ${ANTLR4_STATIC_LIBRARIES})

message("LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS}")
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC include src ${ANTLR4_INCLUDE_DIRS})

llvm_map_components_to_libnames(llvm_libs 
    TextAPI
    OrcJIT
    JITLink
    ObjectYAML
    WindowsManifest
    Coverage
    TableGen
    LTO
    Passes
    ObjCARCOpts
    Coroutines
    LibDriver
    XRay
    MIRParser
    ipo
    Instrumentation
    Vectorize
    Linker
    IRReader
    AsmParser
    Symbolize
    DebugInfoPDB
    FuzzMutate
    MCA
    DebugInfoGSYM
    GlobalISel
    SelectionDAG
    AsmPrinter
    DebugInfoDWARF
    MCJIT
    Interpreter
    ExecutionEngine
    RuntimeDyld
    CodeGen
    Target
    ScalarOpts
    InstCombine
    AggressiveInstCombine
    TransformUtils
    BitWriter
    Analysis
    ProfileData
    DlltoolDriver
    Option
    Object
    MCParser
    MC
    DebugInfoCodeView
    DebugInfoMSF
    BitReader
    Core
    Remarks
    BinaryFormat
    BitstreamReader
    Support
    Demangle
)

include(clang-format.cmake)

target_link_libraries(${PROJECT_NAME} antlr4_static ${llvm_libs})

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libstdral ${CMAKE_BINARY_DIR}/libstdral)
