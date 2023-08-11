cmake_minimum_required(VERSION 3.12)
set(DVERK_VERSION 0.1.0)
project(dverk LANGUAGES Fortran VERSION ${DVERK_VERSION})

find_package(LAPACK REQUIRED)

option(BUILDING_DEMO "Build demo" ON)

# include path
set(_DVERK_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
# src files
set(
    _DVERK_SRC
    src/dverk.f
)
add_library(dverk SHARED ${_DVERK_SRC})
target_compile_options(
    dverk
    PRIVATE
    -std=legacy
    -fpic
    -fno-underscoring
)
target_link_libraries(
    dverk
    PRIVATE
    LAPACK::LAPACK
)

target_include_directories(
    dverk
    PRIVATE
    $<BUILD_INTERFACE:${_DVERK_INCLUDE_DIR}>
)
set(DVERK_LIBRARIES dverk)

# interface
add_library(dverk::dverk ALIAS dverk)
set_property(
    TARGET 
    dverk 
    PROPERTY 
    INTERFACE_INCLUDE_DIRECTORIES 
    $<BUILD_INTERFACE:${_DVERK_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/dverk>
)
set_property(
    TARGET 
    dverk
    PROPERTY 
    INTERFACE_LINK_LIBRARIES
    dverk
)
unset(_DVERK_INCLUDE_DIR)

if (BUILDING_DEMO)
    add_subdirectory(demo)
endif()

# # Tests and benchmark
# if (BUILD_TESTING)
#     message(STATUS "[dverk] Building test")
#     add_subdirectory(test)
# endif()

# if (BUILD_BENCHMARK)
#     message(STATUS "[dverk] Building benchmark")
#     add_subdirectory(benchmark)
# endif()

# 将配置文件中的变量设置为实际值
configure_file(dverk-config.cmake.in dverk-config.cmake @ONLY)

# export dverk
export(TARGETS dverk NAMESPACE dverk:: FILE dverkTargets.cmake)

# install config
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/dverk-config.cmake"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/dverk")

# install target
install(EXPORT dverkTargets
        NAMESPACE dverk::
        DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/dverk")

# install header
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include
        DESTINATION "${CMAKE_INSTALL_PREFIX}"
        FILES_MATCHING PATTERN "*.h")

# install libraries
install(TARGETS dverk
        EXPORT dverkTargets
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        INCLUDES DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
