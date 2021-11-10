#ifndef STATUSINTERFACE_H
#define STATUSINTERFACE_H

#include <QObject>
#include <QList>
#include <scpi.h>

#include "mt310s2d.h"
#include "basestatusinterface.h"
#include "scpidelegate.h"


namespace StatusSystem
{

enum StatusCommands
{
    cmdDevice,
    cmdAdjustment,
    cmdAuthorization
};
}


class cStatusInterface: public cBaseStatusInterface
{
    Q_OBJECT

public:
    cStatusInterface(cMT310S2dServer *server);
    virtual void initSCPIConnection(QString leadingNodes);

protected:
    quint8 getDeviceStatus();
    quint8 getAuthorizationStatus();

protected slots:
    virtual void executeCommand(int cmdCode, cProtonetCommand* protoCmd);

private:
    cMT310S2dServer* m_pMyServer;
};

#endif // STATUSINTERFACE_H
