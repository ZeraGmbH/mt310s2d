set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/micro-controller-io/atmel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/micro-controller-io/atmelsysctrl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/micro-controller-io/atmelwatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/clampinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/frqinputinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/hkeyinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/samplinginterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/scheadinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/senseinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/sourceinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/statusinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/systeminterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/ctrlsettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/debugsettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/ethsettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/fpgasettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/frqinputsettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/i2csettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/samplingsettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/scheadsettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/sensesettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/sourcesettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/hkeysettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/justdata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pcbserver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/justnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scpidelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scpiconnection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/samplerange.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/senserange.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sensechannel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fpzchannel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adjustment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/systeminfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/resource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rmconnection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/notificationstring.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/protonetcommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fpzinchannel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scheadchannel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/clampjustdata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adjflash.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/clamp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adjxml.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mt310s2d.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mt310s2justdata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hkeychannel.cpp
    )

set(HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/micro-controller-io/atmel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/micro-controller-io/atmelsysctrl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/micro-controller-io/atmelwatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/clampinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/frqinputinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/hkeyinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/samplinginterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/scheadinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/senseinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/sourceinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/statusinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scpi-interfaces/systeminterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/ctrlsettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/debugsettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/ethsettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/fpgasettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/frqinputsettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/hkeysettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/i2csettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/samplingsettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/scheadsettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/sensesettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/settings/sourcesettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/zeraglobal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/justdata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mt310s2dscpi.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pcbserver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/justnode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scpidelegate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scpiconnection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/resource.h
    ${CMAKE_CURRENT_SOURCE_DIR}/samplerange.h
    ${CMAKE_CURRENT_SOURCE_DIR}/senserange.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sensechannel.h
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
    ${CMAKE_CURRENT_SOURCE_DIR}/scheadchannel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/clampjustdata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/clamp.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mt310s2d.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mt310s2dglobal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mt310s2justdata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/hkeychannel.h
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


add_library(mt310s2d_lib STATIC
    ${HEADER}
    ${SOURCES}
    )

# announce headers - target perspective
target_include_directories(mt310s2d_lib
    PUBLIC
    $<BUILD_INTERFACE:${MY_LIBRARY_SOURCE_DIR}/include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

#link dependencies
if(${Systemd_FOUND})
    target_link_libraries(mt310s2d_lib
        PUBLIC
        systemd
    )
endif()

target_link_libraries(mt310s2d_lib
    PUBLIC
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

#create executable
add_executable(mt310s2d
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )

target_link_libraries(mt310s2d
    PUBLIC
    mt310s2d_lib
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
