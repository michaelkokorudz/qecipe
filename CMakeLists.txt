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
    backend/src/fridge_inventory.cpp
    backend/database/database.cpp
    backend/src/mongoose.c
    backend/src/spice_pantry.cpp  # Ensure this file is listed if it exists
    backend/src/item.cpp    
    backend/src/date_utils.cpp
)

# Include directories for header files
include_directories(backend/include)

# Optional: Link SQLite3
find_package(SQLite3 REQUIRED)
if (SQLite3_FOUND)
    message(STATUS "SQLite3 include dirs: ${SQLite3_INCLUDE_DIRS}")
    target_include_directories(leftoverlab PRIVATE ${SQLite3_INCLUDE_DIRS})
    target_link_libraries(leftoverlab PRIVATE ${SQLite3_LIBRARIES})
else()
    message(FATAL_ERROR "SQLite3 not found. Please install SQLite3 to proceed.")
endif()

# Optional: Enable testing (if applicable)
enable_testing()