cmake_minimum_required(VERSION 4.1)
project(kvadra_4)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wextra-semi -O3")

add_library(kvadra_4_lib STATIC)
target_sources(kvadra_4_lib
        PRIVATE
        include/finder.hpp
        libs/json.hpp
        include/formats.hpp
        include/all.hpp
        ######
        src/finder.cpp
        src/formats.cpp
)
target_include_directories(kvadra_4_lib PUBLIC include libs) # ?????

add_executable(kvadra_4 main.cpp)
target_link_libraries(kvadra_4 PRIVATE kvadra_4_lib)
