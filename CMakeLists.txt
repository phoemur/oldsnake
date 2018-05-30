cmake_minimum_required(VERSION 2.8.11)

project(oldsnake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
include_directories(include)

# Sources
set(SOURCES
    src/main.cpp
    src/field.cpp
    src/snake.cpp
)

# Compile
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

# Link
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})

#Install
install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
