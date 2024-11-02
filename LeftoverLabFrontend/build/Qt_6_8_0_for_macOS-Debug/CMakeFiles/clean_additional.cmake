# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/LeftoverLabFrontend_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/LeftoverLabFrontend_autogen.dir/ParseCache.txt"
  "LeftoverLabFrontend_autogen"
  )
endif()
