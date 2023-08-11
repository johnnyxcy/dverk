project(dverk-demo-eigen LANGUAGES CXX)

find_package(Eigen3 REQUIRED)

add_executable(
    dverk-demo-eigen
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
)
target_link_libraries(
    dverk-demo-eigen
    dverk::dverk
    Eigen3::Eigen
)