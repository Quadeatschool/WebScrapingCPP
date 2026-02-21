cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(WebScraper)

set(CMAKE_CXX_STANDARD 17)

# 1. Find CURL (This part is working now!)
find_package(CURL REQUIRED)

# 2. Find Gumbo (Using the "vcpkg-friendly" name)
# Some versions of gumbo in vcpkg use 'unofficial-gumbo'
find_package(unofficial-gumbo CONFIG REQUIRED)

# 3. Add your source code
add_executable(${PROJECT_NAME} src/main.cpp)

# 4. Link the libraries
# We use the unofficial namespace for gumbo here
target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl unofficial::gumbo::gumbo)