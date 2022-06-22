cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(oslib VERSION 0.1.0 LANGUAGES CXX)

add_library(oslib
  "src/dl.cpp" src/dl.cpp)
# library file becomes libos.a on POSIX-like OSes.
if (UNIX)
  set_target_properties(oslib PROPERTIES
    OUTPUT_NAME "os"
  )
endif()
target_include_directories(oslib PUBLIC
  "${PROJECT_SOURCE_DIR}/include"
)
target_compile_features(oslib PUBLIC cxx_std_20)
target_link_libraries(oslib PUBLIC ${CMAKE_DL_LIBS})

add_executable(test0
  "test/test0.cpp"
)
target_link_libraries(test0 PUBLIC oslib)