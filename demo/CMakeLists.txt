project(dverk-demo LANGUAGES CXX)

add_executable(
    dverk-demo
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
)
target_link_libraries(
    dverk-demo
    dverk::dverk
)

add_subdirectory(eigen)