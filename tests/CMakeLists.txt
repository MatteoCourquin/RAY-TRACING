cmake_minimum_required(VERSION 3.5.0)

option(ENABLE_THREADING "Enable multi-threading support" ON)
if(ENABLE_THREADING)
    add_compile_definitions(THREADING_ENABLED)
    message(STATUS "Threading support: ENABLED")
else()
    message(STATUS "Threading support: DISABLED")
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(
    rayscene_tests 
    TestImage.cpp 
)

target_include_directories(rayscene_tests PUBLIC
    "${PROJECT_SOURCE_DIR}/src/raymath"
    "${PROJECT_SOURCE_DIR}/src/rayimage"
    "${PROJECT_SOURCE_DIR}/src/rayscene"
    "${PROJECT_SOURCE_DIR}/src/lodepng"
)

target_link_libraries(rayscene_tests 
    PUBLIC 
    gtest_main
    rayscene
    raymath
    rayimage
    lodepng
    OpenSSL::Crypto
    Threads::Threads
)

enable_testing()
add_test(NAME rayscene_tests COMMAND rayscene_tests)