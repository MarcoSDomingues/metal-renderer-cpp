cmake_minimum_required(VERSION 3.12)
project(MetalEngine)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)

# Add the source files
file(GLOB SOURCES "MetalEngine/*.cpp")

# Add the executable
add_executable(MetalEngine ${SOURCES})

# Include directories for Metal-related code
target_include_directories(MetalEngine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/metal-cpp-extensions"
)

# Find the necessary frameworks
find_library(METAL_FRAMEWORK Metal)
find_library(FOUNDATION_FRAMEWORK Foundation)
find_library(METALKIT_FRAMEWORK MetalKit)

# Link frameworks to the executable
if(METAL_FRAMEWORK AND FOUNDATION_FRAMEWORK AND METALKIT_FRAMEWORK)
    target_link_libraries(MetalEngine PRIVATE
        ${METAL_FRAMEWORK}
        ${FOUNDATION_FRAMEWORK}
        ${METALKIT_FRAMEWORK}
    )
else()
    message(FATAL_ERROR "One or more frameworks not found")
endif()

# Specify the shaders directory
set(SHADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MetalEngine/shaders")
set(BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Compile Metal shaders
file(GLOB METAL_SHADERS "${SHADERS_DIR}/*.metal")

foreach(SHADER ${METAL_SHADERS})
    get_filename_component(SHADER_NAME ${SHADER} NAME_WE)
    set(SHADER_AIR "${BUILD_DIR}/${SHADER_NAME}.air")
    set(SHADER_METALLIB "${BUILD_DIR}/${SHADER_NAME}.metallib")
    
    add_custom_command(
        OUTPUT ${SHADER_AIR}
        COMMAND xcrun metal -c ${SHADER} -o ${SHADER_AIR}
        DEPENDS ${SHADER}
        COMMENT "Compiling Metal shader ${SHADER_NAME}.metal to ${SHADER_AIR}"
    )
    
    list(APPEND SHADER_AIRS ${SHADER_AIR})
endforeach()

add_custom_command(
    OUTPUT ${BUILD_DIR}/default.metallib
    COMMAND xcrun metallib ${SHADER_AIRS} -o ${BUILD_DIR}/default.metallib
    DEPENDS ${SHADER_AIRS}
    COMMENT "Creating Metal library: default.metallib"
)

add_custom_target(
    Shaders
    DEPENDS ${BUILD_DIR}/default.metallib
)

# Ensure shaders are compiled before the main executable
add_dependencies(MetalEngine Shaders)

# Copy compiled shader library to the executable directory
add_custom_command(
    TARGET MetalEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${BUILD_DIR}/default.metallib"
    "$<TARGET_FILE_DIR:MetalEngine>/default.metallib"
)