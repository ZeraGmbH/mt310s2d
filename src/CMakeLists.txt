set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/justdata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pcbserver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/i2csettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ethsettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fpgasettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/justnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scpidelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statusinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scpiconnection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/systeminterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/atmel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/atmelsysctrl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/debugsettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/senseinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sensesettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/atmelwatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/samplerange.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/samplinginterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/samplingsettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/senserange.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sensechannel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sourcesettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fpzchannel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sourceinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adjustment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/systeminfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/resource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rmconnection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/notificationstring.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/protonetcommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fpzinchannel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/frqinputinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/frqinputsettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scheadchannel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scheadsettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scheadinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/clampjustdata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adjflash.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/clamp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adjxml.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ctrlsettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/clampinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mt310s2d.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mt310s2justdata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hkeysettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hkeyinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hkeychannel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/i2cmuxer.cpp
    )

set(HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/zeraglobal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/justdata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mt310s2dscpi.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pcbserver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/i2csettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ethsettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/fpgasettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/justnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scpidelegate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statusinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scpiconnection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/systeminterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/atmel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/atmelsysctrl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/xmlsettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/debugsettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/senseinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sensesettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/atmelwatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/resource.h
    ${CMAKE_CURRENT_SOURCE_DIR}/samplerange.h
    ${CMAKE_CURRENT_SOURCE_DIR}/samplinginterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/samplingsettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/senserange.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sensechannel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sourcesettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sourceinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/fpzchannel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/adjflash.h
    ${CMAKE_CURRENT_SOURCE_DIR}/adjxml.h
    ${CMAKE_CURRENT_SOURCE_DIR}/adjustment.h
    ${CMAKE_CURRENT_SOURCE_DIR}/systeminfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/rmconnection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/notificationstring.h
    ${CMAKE_CURRENT_SOURCE_DIR}/notificationdata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/protonetcommand.h
    ${CMAKE_CURRENT_SOURCE_DIR}/fpzinchannel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/frqinputinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/frqinputsettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scheadchannel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scheadsettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scheadinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/clampjustdata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/clamp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ctrlsettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/clampinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mt310s2d.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mt310s2dglobal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mt310s2justdata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/hkeychannel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/hkeyinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/hkeysettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/i2cmuxer.h
    )

file(GLOB RESOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc
    )

set(CONFIG_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/mt310s2d.xml
)
set(CONFIG_VALIDATOR
    ${CMAKE_CURRENT_SOURCE_DIR}/mt310s2d.xsd
)

# check for valid config files / display in qt-creator
add_custom_target(mt310s2-configs ALL
    DEPENDS ${CONFIG_VALIDATOR} ${CONFIG_FILES}
    SOURCES ${CONFIG_VALIDATOR} ${CONFIG_FILES}
    COMMAND xmllint --noout --schema ${CONFIG_VALIDATOR} ${CONFIG_FILES}
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
    Qt5::Gui
    Qt5::Xml
    Qt5::Network
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
    FILES ${CONFIG_FILES} ${CONFIG_VALIDATOR}
    DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/zera/mt310s2d
    )
