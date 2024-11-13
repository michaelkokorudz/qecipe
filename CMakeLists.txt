# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(InventoryBackend)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add Mongoose and other sources
set(SOURCES
    backend/src/main.cpp         # Ensure these paths are correct
    backend/src/routes.cpp
    backend/external/mongoose/mongoose.c  # Path to Mongoose source file
)

# Include paths for headers
include_directories(
    backend/include                    # Project headers
    backend/external/mongoose          # Mongoose headers
)

# Create executable
add_executable(inventory_backend ${SOURCES})

# Set output directory
set_target_properties(inventory_backend PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})