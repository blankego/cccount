project(cccount)

add_executable(cccounttest test.cpp)

target_link_libraries(cccounttest cccountlib boost_unit_test_framework)