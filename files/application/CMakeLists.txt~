file(GLOB HEADER_FILES *.hpp)
file(GLOB SRC_FILES *.cpp)
file(GLOB_RECURSE SHADER_FILES *.glsl)

foreach(SRC_FILE ${SRC_FILES})
    get_filename_component(FILE ${SRC_FILE} NAME_WE)
    set(OUTPUT ${FILE})
    add_executable(${OUTPUT} ${SRC_FILE} ${HEADER_FILES} ${SHADER_FILES})
    target_link_libraries(${OUTPUT} ${ALL_LIBRARIES})
endforeach()

file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(GTEST_DIR ../third-party/gtest-1.7.0)
find_package(Threads)
include_directories(SYSTEM ${GTEST_DIR}/include)
include_directories(${GTEST_DIR})
add_library(gtest ${GTEST_DIR}/src/gtest-all.cc)
add_library(gtest_main ${GTEST_DIR}/src/gtest_main.cc)

file(GLOB ALL_TEST_FILES ../engine/test/*Test.cpp)
add_executable(runAllTests ${ALL_TEST_FILES})
target_link_libraries(runAllTests gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})
add_custom_command(TARGET runAllTests POST_BUILD COMMAND runAllTests)
