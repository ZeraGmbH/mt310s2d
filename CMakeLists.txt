cmake_minimum_required(VERSION 3.1)

project(mt310s2d LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(mt310s2d_VERSION_MAJOR "1")
set(mt310s2d_VERSION_MINOR "1")
set(mt310s2d_VERSION_PATCH "0")
set(PROJECT_VERSION "${mt310s2d_VERSION_MAJOR}.${mt310s2d_VERSION_MINOR}.${mt310s2d_VERSION_PATCH}")

include(FeatureSummary)
include(GNUInstallDirs)

option(useSystemd "Using libsystemd" ON)

#Find dependecies
find_package(Qt5 COMPONENTS Core Gui Xml Network CONFIG REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(GTest)
find_package(SCPI REQUIRED)
find_package(xiqnet REQUIRED)
find_package(zerai2c REQUIRED)
find_package(zeraproxy REQUIRED)
find_package(zeramisc REQUIRED)
find_package(zeraxmlconfig REQUIRED)
find_package(zeramath REQUIRED)
find_package(zeradev REQUIRED)
find_package(zerameaschannels REQUIRED)
find_package(VfProtobuf REQUIRED)
find_package(zeraprotobuf REQUIRED)

if(${useSystemd})
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(Systemd REQUIRED libsystemd)
    add_definitions(-DSYSTEMD_NOTIFICATION)
endif()

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
