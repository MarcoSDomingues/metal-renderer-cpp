cmake_minimum_required(VERSION 3.12)
project(MetalEngine)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)

#find_package(glfw CONFIG REQUIRED)

find_library(METAL_FRAMEWORK Metal)
find_library(FOUNDATION_FRAMEWORK Foundation)

# Add the source files
file(GLOB SOURCES "MetalEngine/*.cpp")

# Add the executable
add_executable(MetalEngine ${SOURCES})

# Include directories for Metal-related code
target_include_directories(MetalEngine PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp-extensions"
)

# Link the libraries to the executable
target_link_libraries(MetalEngine PRIVATE ${METAL_FRAMEWORK} ${FOUNDATION_FRAMEWORK})