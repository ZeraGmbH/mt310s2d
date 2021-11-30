#ifndef SYSTEMINTERFACE_H
#define SYSTEMINTERFACE_H

#include <QObject>
#include <QList>
#include <scpi.h>

#include "scpiconnection.h"
#include "basesysteminterface.h"

namespace SystemSystem
{

enum SystemCommands
{
    cmdVersionCTRL,
    cmdVersionFPGA,
    cmdVersionDevice,
    cmdUpdateControlerBootloader,
    cmdUpdateControlerProgram,
    cmdUpdateControlerFlash,
    cmdUpdateControlerEEprom,
    cmdAdjFlashWrite,
    cmdAdjFlashRead,
    cmdAdjXMLImportExport,
    cmdAdjXMLWrite,
    cmdAdjXMLRead,
    cmdAdjFlashChksum
};
}


class cMT310S2dServer;


class cSystemInterface: public cBaseSystemInterface
{
    Q_OBJECT

public:
    cSystemInterface(cMT310S2dServer* server);
    virtual void initSCPIConnection(QString leadingNodes);

protected slots:
    virtual void executeCommand(int cmdCode, cProtonetCommand* protoCmd);

private:
    QString m_ReadDeviceVersion(QString& sInput);
    QString m_ReadCTRLVersion(QString& sInput);
    QString m_ReadFPGAVersion(QString& sInput);
    QString m_StartControlerBootloader(QString& sInput);
    QString m_StartControlerProgram(QString& sInput);
    QString m_LoadFlash(QString& sInput);
    QString m_LoadEEProm(QString& sInput);
    QString m_AdjFlashWrite(QString& sInput);
    QString m_AdjFlashRead(QString& sInput);
    QString m_AdjXmlImportExport(QString& sInput);
    QString m_AdjXMLWrite(QString& sInput);
    QString m_AdjXMLRead(QString& sInput);
    QString m_AdjFlashChksum(QString& sInput);
};


#endif // SYSTEMINTERFACE_H
