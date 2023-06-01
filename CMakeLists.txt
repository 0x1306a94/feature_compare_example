cmake_minimum_required(VERSION 3.20)
project(feature_compare_example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "$<IF:$<CONFIG:Debug>,dwarf,dwarf-with-dsym>")

if(APPLE)
    execute_process(
      COMMAND uname -m
      RESULT_VARIABLE result
      OUTPUT_VARIABLE OSX_NATIVE_ARCHITECTURE
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(${OSX_NATIVE_ARCHITECTURE} STREQUAL "x86_64")
        set(CMAKE_CXX_FLAGS "-mavx ${CMAKE_CXX_FLAGS}")
    endif()
elseif(UNIX)

endif()


add_executable(${PROJECT_NAME} main.cpp)