cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(MINGW)
    file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/bin)
elseif(MSVC)
    file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
endif()

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main src/main.cpp)

find_package(OpenMP REQUIRED)

target_compile_features(main PRIVATE cxx_std_17)

if(MINGW)
    target_link_libraries(main PRIVATE SFML::Graphics OpenMP::OpenMP_CXX gomp)
elseif(MSVC)
    target_link_libraries(main PRIVATE SFML::Graphics OpenMP::OpenMP_CXX)
endif()
