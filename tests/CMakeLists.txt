cmake_minimum_required(VERSION 3.5.0)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(rayscene_tests TestSphere.cpp)

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
)

enable_testing()
add_test(NAME rayscene_tests COMMAND rayscene_tests)