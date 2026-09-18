cmake_minimum_required(VERSION 3.16)

project(MissingNo
    VERSION 0.1.0
    LANGUAGES C
)

add_library(MissingNo STATIC
    src/MissingNo.c
    src/emulator.c
    src/memory.c
    src/startup.c
)

add_library(MissingNo::MissingNo ALIAS MissingNo)

target_include_directories(MissingNo
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal
)

target_compile_features(MissingNo PUBLIC c_std_99)