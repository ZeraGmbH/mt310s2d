#ifndef EMOBINTERFACE_H
#define EMOBINTERFACE_H

#include "mt310s2d.h"
#include "scpiconnection.h"
#include "notificationstring.h"


class EmobInterface: public cSCPIConnection
{
public:
    EmobInterface(cI2CSettings *i2cSettings);
    void initSCPIConnection(QString leadingNodes) override;
    void actualizeClampStatus(quint16 devConnectedMask);
protected slots:
    void executeCommand(int cmdCode, cProtonetCommand* protoCmd) override;
private:
    cI2CSettings *m_i2cSettings;
};

#endif // EMOBINTERFACE_H
