project(cccount)
add_subdirectory(cccounttest)


add_library(cccountlib STATIC utf8iterator.cpp cccounter.cpp utils.cpp)
add_executable(cccount main.cpp cccountcli.cpp)
target_link_libraries(cccount cccountlib boost_program_options)

