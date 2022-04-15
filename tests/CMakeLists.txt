# unittests
find_package(GTest)
if(NOT CMAKE_CROSSCOMPILING AND ${GTest_FOUND})
    message(STATUS "unittest ${PROJECT_NAME}")
    enable_testing()


endif()
