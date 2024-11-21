cmake_minimum_required(VERSION 3.10)
project(leftoverlab)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Debugging: Print important paths
message(STATUS "Project source directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "Project binary directory: ${CMAKE_BINARY_DIR}")

# Add the executable target
add_executable(leftoverlab
    backend/src/main.cpp
    backend/src/routes.cpp
    backend/src/inventory.cpp
    backend/src/spice_item.cpp
    backend/src/api_methods.cpp
    backend/src/fridge_inventory.cpp
    backend/src/database.cpp
    backend/src/mongoose.c
    backend/src/spice_pantry.cpp 
    backend/src/item.cpp    
    backend/src/date_utils.cpp
    backend/third_party/sqlite/sqlite3.c # SQLite source file
)

# Include directories for header files
include_directories(
    backend/include
    backend/third_party/sqlite # Include SQLite headers
)

# Link the SQLite library statically (no external dependency)
target_compile_definitions(leftoverlab PRIVATE SQLITE_OMIT_LOAD_EXTENSION SQLITE_THREADSAFE=1)

# Optional: Enable testing (if applicable)
enable_testing()
