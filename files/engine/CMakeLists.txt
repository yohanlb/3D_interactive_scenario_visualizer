include_directories(include)
file(GLOB_RECURSE SRC_FILES ./src/*.cpp ./include/*.hpp)
add_library(engine ${SRC_FILES})
