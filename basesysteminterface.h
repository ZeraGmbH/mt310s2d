#ifndef BASESYSTEMINTERFACE_H
#define BASESYSTEMINTERFACE_H

#include <QObject>
#include <QList>
#include <scpi.h>

#include "scpiconnection.h"

namespace BaseSystemSystem
{

enum BaseSystemCommands
{
    cmdVersionServer,
    cmdVersionPCB,
    cmdSerialNumber,
    cmdInterfaceRead
};
}


class cPCBServer;


class cBaseSystemInterface: public cSCPIConnection
{
    Q_OBJECT

public:
    cBaseSystemInterface(cPCBServer* server);
    virtual void initSCPIConnection(QString leadingNodes);

protected:
    cPCBServer* m_pMyServer;
    QString m_ReadServerVersion(QString& sInput);
    QString m_ReadDeviceName(QString& sInput);
    QString m_ReadWritePCBVersion(QString& sInput);
    QString m_ReadWriteSerialNumber(QString& sInput);
    QString m_InterfaceRead(QString& sInput);
    void m_genAnswer(int select, QString& answer);

protected slots:
    virtual void executeCommand(int cmdCode, cProtonetCommand* protoCmd);
};


#endif // BASESYSTEMINTERFACE_H
