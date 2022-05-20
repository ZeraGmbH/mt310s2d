# unittests
if(NOT CMAKE_CROSSCOMPILING)
    message(STATUS "unittest qt")
    enable_testing()

    add_test(NAME    mt310s2d-unittest-qt
             COMMAND mt310s2d-unittest-qt)

    file(GLOB QTUNITTESTS
        qt-unittest-*.cpp
        qt-unittest-*.h
        testserver.h
        testserver.cpp
    )

    add_executable(mt310s2d-unittest-qt
        main-unittest-qt.h
        main-unittest-qt.cpp # must be first!!!
        ${QTUNITTESTS}
    )

    target_include_directories(mt310s2d-unittest-qt
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/settings
        ${PROJECT_SOURCE_DIR}/src/scpi-interfaces
        )

    target_link_libraries(mt310s2d-unittest-qt
        PUBLIC
        Qt5::Core
        Qt5::Test
        mt310s2d_lib
        )

endif()
