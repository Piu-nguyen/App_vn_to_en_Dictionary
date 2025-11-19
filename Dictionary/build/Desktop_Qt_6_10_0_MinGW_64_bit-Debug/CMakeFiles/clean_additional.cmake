# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Dictionary_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Dictionary_autogen.dir\\ParseCache.txt"
  "Dictionary_autogen"
  )
endif()
