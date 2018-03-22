#ifndef SENSEINTERFACE_H
#define SENSEINTERFACE_H

#include <QObject>
#include <QList>
#include <QStateMachine>
#include <QHash>
#include <QState>
#include <QFinalState>

#include "adjflash.h"
#include "adjxml.h"
#include "resource.h"
#include "scpiconnection.h"
#include "sensechannel.h"
#include "notificationstring.h"

namespace SenseSystem
{

const QString Version = "V1.00";

enum Commands
{
    cmdVersion,
    cmdMMode,
    cmdMModeCat,
    cmdChannelCat,
    cmdGroupCat,
    initAdjData,
    computeAdjData,
    cmdStatAdjustment
};

enum MMode
{
    modeAC = 1,
    modeHF = 2,
    modeADJ = 4
};

const QString sVoltageChannelDescription = "Measuring channel 0..250V 50Hz/150kHz";
const QString sCurrentChannelDescription = "Measuring channel 0..1000A 50Hz/150kHz";
const QString sMeasuringModeDescription = "Measuring mode switch AC,HF,ADJ";
}

class cMT310S2dServer;
class cSenseSettings;
class cI2CSettings;
class cDebugSettings;
class QDataStream;


class cSenseInterface : public cResource, public cAdjFlash, public cAdjXML
{
    Q_OBJECT

public:
    cSenseInterface(cMT310S2dServer *server);
    ~cSenseInterface();
    virtual void initSCPIConnection(QString leadingNoMModedes);
    cSenseChannel* getChannel(QString& name);
    cSenseRange* getRange(QString channelName, QString rangeName);
    virtual quint8 getAdjustmentStatus(); // we return 0 if adj. otherwise  1 +2 +4
    virtual void registerResource(cRMConnection *rmConnection, quint16 port);
    virtual void unregisterResource(cRMConnection *rmConnection);

protected:
    virtual void exportAdjData(QDataStream& stream);
    virtual bool importAdjData(QDataStream& stream);
    virtual QString exportXMLString();
    virtual bool importXMLDocument(QDomDocument* qdomdoc);

    virtual void setI2CMux();

protected slots:
    virtual void executeCommand(int cmdCode, cProtonetCommand* protoCmd);

private:
    cMT310S2dServer* m_pMyServer;

    QList<cSenseChannel*> m_ChannelList;
    QString m_sVersion;
    QString m_sMMode;
    QHash<QString,quint8> m_MModeHash;

    quint8 m_nVersionStatus;
    qint8 m_nSerialStatus;

    QString m_ReadVersion(QString& sInput);
    void m_ReadWriteMMode(cProtonetCommand* protoCmd);
    QString m_ReadMModeCatalog(QString& sInput);
    QString m_ReadSenseChannelCatalog(QString& sInput);
    QString m_ReadSenseGroupCatalog(QString& sInput);
    QString m_InitSenseAdjData(QString& sInput);
    QString m_ComputeSenseAdjData(QString& sInput);
    QString m_ReadAdjStatus(QString& sInput);

    cNotificationString notifierSenseMMode;
    cNotificationString notifierSenseChannelCat;

    void setNotifierSenseMMode();
    void setNotifierSenseChannelCat();

    bool setSenseMode(QString sMode);
};

#endif // SENSEINTERFACE_H
