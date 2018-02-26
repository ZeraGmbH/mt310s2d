TEMPLATE	= app
LANGUAGE	= C++

include(mt310s2d.user.pri)

QMAKE_CXXFLAGS += -O0 -Wshadow

systemd_notification {
       DEFINES += SYSTEMD_NOTIFICATION
       LIBS +=  -lsystemd
}

LIBS +=  -lSCPI
LIBS +=  -lproto-net-qt
LIBS +=  -lzerai2c
LIBS +=  -lzeramisc
LIBS +=  -lzeraxmlconfig
LIBS +=  -lzeramath
LIBS +=  -lzeradev
LIBS +=  -lprotobuf
LIBS +=  -lzera-resourcemanager-protobuf

CONFIG	+= qt debug

HEADERS	+= \
	zeraglobal.h \
	justdata.h \
    mt310s2scpi.h \
    pcbserver.h \
    i2csettings.h \
    ethsettings.h \
    fpgasettings.h \
    justnode.h \
    scpidelegate.h \
    statusinterface.h \
    scpiconnection.h \
    systeminterface.h \
    atmel.h \
    xmlsettings.h \
    debugsettings.h \
    senseinterface.h \
    sensesettings.h \
    atmelwatcher.h \
    resource.h \
    samplerange.h \
    samplinginterface.h \
    samplingsettings.h \
    senserange.h \
    sensechannel.h \
    sourcesettings.h \
    sourceinterface.h \
    fpzchannel.h \
    adjflash.h \
    adjxml.h \
    adjustment.h \
    systeminfo.h \
    rmconnection.h \
    notificationstring.h \
    notificationdata.h \
    protonetcommand.h \
    fpzinchannel.h \
    frqinputinterface.h \
    frqinputsettings.h \
    scheadchannel.h \
    scheadsettings.h \
    scheadinterface.h \
    clampjustdata.h \
    clamp.h \
    ctrlsettings.h \
    clampinterface.h \
    mt310s2d.h \
    mt310s2dglobal.h \
    mt310s2dprotobufwrapper.h \
    mt310s2justdata.h

SOURCES	+= \
	main.cpp \
	justdata.cpp \
    pcbserver.cpp \
    i2csettings.cpp \
    ethsettings.cpp \
    fpgasettings.cpp \
    justnode.cpp \
    scpidelegate.cpp \
    statusinterface.cpp \
    scpiconnection.cpp \
    systeminterface.cpp \
    atmel.cpp \
    debugsettings.cpp \
    senseinterface.cpp \
    sensesettings.cpp \
    atmelwatcher.cpp \
    samplerange.cpp \
    samplinginterface.cpp \
    samplingsettings.cpp \
    senserange.cpp \
    sensechannel.cpp \
    sourcesettings.cpp \
    fpzchannel.cpp \
    sourceinterface.cpp \
    adjustment.cpp \
    systeminfo.cpp \
    resource.cpp \
    rmconnection.cpp \
    notificationstring.cpp \
    protonetcommand.cpp \
    fpzinchannel.cpp \
    frqinputinterface.cpp \
    frqinputsettings.cpp \
    scheadchannel.cpp \
    scheadsettings.cpp \
    scheadinterface.cpp \
    clampjustdata.cpp \
    adjflash.cpp \
    clamp.cpp \
    adjxml.cpp \
    ctrlsettings.cpp \
    clampinterface.cpp \
    mt310s2d.cpp \
    mt310s2dprotobufwrapper.cpp \
    mt310s2justdata.cpp

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

target.path = /usr/bin
INSTALLS += target

configxml.path = /etc/zera/mt310s2d
configxml.files = mt310s2d.xsd \
                  mt310s2d.xml

INSTALLS += configxml

QT += xml network

OTHER_FILES +=
