#include "qt-unittest-emobinterfacetest.h"
#include "main-unittest-qt.h"
#include "testserver.h"
#include "i2csettings.h"
#include "emobinterface.h"

static QObject* pEmobInterfaceTest = addTest(new EmobInterfaceTest);

void EmobInterfaceTest::init()
{
}

void EmobInterfaceTest::initialWorkingSCPIStack()
{
    TestServer srv;
    cI2CSettings i2cSettings(nullptr);
    EmobInterface emob(&i2cSettings, srv.getSCPIInterface());
    srv.addScpiInterface(&emob);
    srv.executeScpiCmd("SYSTEM:EMOB:CHANNEL:CATALOG?");
}
