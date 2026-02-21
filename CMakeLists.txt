cmake_minimum_required(VERSION 3.10)
project(WebScraper)

set(CMAKE_CXX_STANDARD 17)

# 1. Find the libraries we just installed
find_package(CURL REQUIRED)
find_package(gumbo REQUIRED)

# 2. Add your source code
add_executable(${PROJECT_NAME} src/main.cpp)

# 3. Link the libraries to your executable
target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl gumbo)