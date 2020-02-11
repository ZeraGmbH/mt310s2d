cmake_minimum_required(VERSION 3.1)

#set project name and language
project(mt310s2d LANGUAGES CXX)

#Automoc and RCC ON. Needed in QT projects
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#C++ 11 Flag
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Set project Version
set(mt310s2d_VERSION_MAJOR "0")
set(mt310s2d_VERSION_MINOR "0")
set(mt310s2d_VERSION_PATCH "1")

set(PROJECT_VERSION "${mt310s2d_VERSION_MAJOR}.${mt310s2d_VERSION_MINOR}.${mt310s2d_VERSION_PATCH}")


option(useSystemd "Using libsystemd" ON)

#Adding cmake functions like feature_summary()
include(FeatureSummary)
include(GNUInstallDirs)

#Find dependecies
find_package(Qt5 COMPONENTS Xml Network CONFIG REQUIRED)
find_package(SCPI REQUIRED)
find_package(xiqnet REQUIRED)
find_package(zerai2c REQUIRED)
find_package(zeramisc REQUIRED)
find_package(zeraxmlconfig REQUIRED)
find_package(zeramath REQUIRED)
find_package(zeradev REQUIRED)
find_package(VfProtobuf REQUIRED)
find_package(zeraprotobuf REQUIRED)

if(${useSystemd})
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(Systemd REQUIRED libsystemd)
    add_definitions(-DSYSTEMD_NOTIFICATION)
endif()

#sum up project Files 
set(SOURCES
${PROJECT_SOURCE_DIR}/main.cpp
${PROJECT_SOURCE_DIR}/justdata.cpp
${PROJECT_SOURCE_DIR}/pcbserver.cpp
${PROJECT_SOURCE_DIR}/i2csettings.cpp
${PROJECT_SOURCE_DIR}/ethsettings.cpp
${PROJECT_SOURCE_DIR}/fpgasettings.cpp
${PROJECT_SOURCE_DIR}/justnode.cpp
${PROJECT_SOURCE_DIR}/scpidelegate.cpp
${PROJECT_SOURCE_DIR}/statusinterface.cpp
${PROJECT_SOURCE_DIR}/scpiconnection.cpp
${PROJECT_SOURCE_DIR}/systeminterface.cpp
${PROJECT_SOURCE_DIR}/atmel.cpp
${PROJECT_SOURCE_DIR}/debugsettings.cpp
${PROJECT_SOURCE_DIR}/senseinterface.cpp
${PROJECT_SOURCE_DIR}/sensesettings.cpp
${PROJECT_SOURCE_DIR}/atmelwatcher.cpp
${PROJECT_SOURCE_DIR}/samplerange.cpp
${PROJECT_SOURCE_DIR}/samplinginterface.cpp
${PROJECT_SOURCE_DIR}/samplingsettings.cpp
${PROJECT_SOURCE_DIR}/senserange.cpp
${PROJECT_SOURCE_DIR}/sensechannel.cpp
${PROJECT_SOURCE_DIR}/sourcesettings.cpp
${PROJECT_SOURCE_DIR}/fpzchannel.cpp
${PROJECT_SOURCE_DIR}/sourceinterface.cpp
${PROJECT_SOURCE_DIR}/adjustment.cpp
${PROJECT_SOURCE_DIR}/systeminfo.cpp
${PROJECT_SOURCE_DIR}/resource.cpp
${PROJECT_SOURCE_DIR}/rmconnection.cpp
${PROJECT_SOURCE_DIR}/notificationstring.cpp
${PROJECT_SOURCE_DIR}/protonetcommand.cpp
${PROJECT_SOURCE_DIR}/fpzinchannel.cpp
${PROJECT_SOURCE_DIR}/frqinputinterface.cpp
${PROJECT_SOURCE_DIR}/frqinputsettings.cpp
${PROJECT_SOURCE_DIR}/scheadchannel.cpp
${PROJECT_SOURCE_DIR}/scheadsettings.cpp
${PROJECT_SOURCE_DIR}/scheadinterface.cpp
${PROJECT_SOURCE_DIR}/clampjustdata.cpp
${PROJECT_SOURCE_DIR}/adjflash.cpp
${PROJECT_SOURCE_DIR}/clamp.cpp
${PROJECT_SOURCE_DIR}/adjxml.cpp
${PROJECT_SOURCE_DIR}/ctrlsettings.cpp
${PROJECT_SOURCE_DIR}/clampinterface.cpp
${PROJECT_SOURCE_DIR}/mt310s2d.cpp
${PROJECT_SOURCE_DIR}/mt310s2dprotobufwrapper.cpp
${PROJECT_SOURCE_DIR}/mt310s2justdata.cpp
${PROJECT_SOURCE_DIR}/hkeysettings.cpp
${PROJECT_SOURCE_DIR}/hkeyinterface.cpp
${PROJECT_SOURCE_DIR}/hkeychannel.cpp
    )


set(HEADER
${PROJECT_SOURCE_DIR}/zeraglobal.h
${PROJECT_SOURCE_DIR}/justdata.h
${PROJECT_SOURCE_DIR}/mt310s2dscpi.h
${PROJECT_SOURCE_DIR}/pcbserver.h
${PROJECT_SOURCE_DIR}/i2csettings.h
${PROJECT_SOURCE_DIR}/ethsettings.h
${PROJECT_SOURCE_DIR}/fpgasettings.h
${PROJECT_SOURCE_DIR}/justnode.h
${PROJECT_SOURCE_DIR}/scpidelegate.h
${PROJECT_SOURCE_DIR}/statusinterface.h
${PROJECT_SOURCE_DIR}/scpiconnection.h
${PROJECT_SOURCE_DIR}/systeminterface.h
${PROJECT_SOURCE_DIR}/atmel.h
${PROJECT_SOURCE_DIR}/xmlsettings.h
${PROJECT_SOURCE_DIR}/debugsettings.h
${PROJECT_SOURCE_DIR}/senseinterface.h
${PROJECT_SOURCE_DIR}/sensesettings.h
${PROJECT_SOURCE_DIR}/atmelwatcher.h
${PROJECT_SOURCE_DIR}/resource.h
${PROJECT_SOURCE_DIR}/samplerange.h
${PROJECT_SOURCE_DIR}/samplinginterface.h
${PROJECT_SOURCE_DIR}/samplingsettings.h
${PROJECT_SOURCE_DIR}/senserange.h
${PROJECT_SOURCE_DIR}/sensechannel.h
${PROJECT_SOURCE_DIR}/sourcesettings.h
${PROJECT_SOURCE_DIR}/sourceinterface.h
${PROJECT_SOURCE_DIR}/fpzchannel.h
${PROJECT_SOURCE_DIR}/adjflash.h
${PROJECT_SOURCE_DIR}/adjxml.h
${PROJECT_SOURCE_DIR}/adjustment.h
${PROJECT_SOURCE_DIR}/systeminfo.h
${PROJECT_SOURCE_DIR}/rmconnection.h
${PROJECT_SOURCE_DIR}/notificationstring.h
${PROJECT_SOURCE_DIR}/notificationdata.h
${PROJECT_SOURCE_DIR}/protonetcommand.h
${PROJECT_SOURCE_DIR}/fpzinchannel.h
${PROJECT_SOURCE_DIR}/frqinputinterface.h
${PROJECT_SOURCE_DIR}/frqinputsettings.h
${PROJECT_SOURCE_DIR}/scheadchannel.h
${PROJECT_SOURCE_DIR}/scheadsettings.h
${PROJECT_SOURCE_DIR}/scheadinterface.h
${PROJECT_SOURCE_DIR}/clampjustdata.h
${PROJECT_SOURCE_DIR}/clamp.h
${PROJECT_SOURCE_DIR}/ctrlsettings.h
${PROJECT_SOURCE_DIR}/clampinterface.h
${PROJECT_SOURCE_DIR}/mt310s2d.h
${PROJECT_SOURCE_DIR}/mt310s2dglobal.h
${PROJECT_SOURCE_DIR}/mt310s2dprotobufwrapper.h
${PROJECT_SOURCE_DIR}/mt310s2justdata.h
${PROJECT_SOURCE_DIR}/hkeychannel.h
${PROJECT_SOURCE_DIR}/hkeyinterface.h
${PROJECT_SOURCE_DIR}/hkeysettings.h
    )

file(GLOB RESOURCES 
    ${PROJECT_SOURCE_DIR}/*.qrc
    )

set(OTHER_FILES 
    ${PROJECT_SOURCE_DIR}/mt310s2d.xsd
    ${PROJECT_SOURCE_DIR}/mt310s2d.xml
)

#create executable
add_executable(mt310s2d
    ${SOURCES}
    ${HEADER}
    ${RESOURCES}
    )

if(${Systemd_FOUND})
    target_link_libraries(mt310s2d
        PRIVATE
        systemd
    )
endif()

#link dependencies
target_link_libraries(mt310s2d
    PRIVATE
    Qt5::Core
    Qt5::Xml
    Qt5::Network
    Qt5::Quick
    Zera::SCPI
    VeinMeta::xiqnet
    ZeraClasses::zerai2c
    ZeraClasses::zeramisc
    ZeraClasses::zeraxmlconfig
    ZeraClasses::zeramath
    ZeraClasses::zeradev
    VeinMeta::VfProtobuf
    VeinMeta::zeraprotobuf
    )	





# announce headers - target perspective
target_include_directories(mt310s2d
    PUBLIC
    $<BUILD_INTERFACE:${MY_LIBRARY_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/mt310s2d>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    )



#install mt310s2dto bin 
install(TARGETS mt310s2d
    EXPORT mt310s2dExport
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

install(
    FILES ${OTHER_FILES}
    DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/zera/mt310s2d
    )


# spawn out some info on configuration
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)






