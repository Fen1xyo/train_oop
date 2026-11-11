add_library(TrainLib STATIC
    wagon.cpp
    train.cpp
)

target_include_directories(TrainLib PUBLIC
    "${PROJECT_SOURCE_DIR}/lib"
)

if(ENABLE_COVERAGE)
    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(FATAL_ERROR "Coverage requires GCC")
    endif()

    target_compile_options(TrainLib PRIVATE
        --coverage
        -O0
        -g
    )

    target_link_options(TrainLib INTERFACE
        --coverage
    )
endif()