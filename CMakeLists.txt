cmake_minimum_required(VERSION 3.20)
project(TrainProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Enable coverage with GCC" OFF)
option(BUILD_DOCS "Enable Doxygen documentation" OFF)

add_subdirectory(src)

add_executable(TrainApp dialogue.cpp)
target_link_libraries(TrainApp PRIVATE TrainLib)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

if(BUILD_DOCS)
    find_package(Doxygen REQUIRED)

    set(DOXYGEN_GENERATE_LATEX NO)
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/docs")

    doxygen_add_docs(docs
        "${PROJECT_SOURCE_DIR}/lib"
        "${PROJECT_SOURCE_DIR}/src"
    )
endif()