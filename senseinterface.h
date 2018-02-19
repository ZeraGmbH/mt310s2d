#ifndef SENSEINTERFACE_H
#define SENSEINTERFACE_H

#include <QObject>
#include <QList>
#include <QStateMachine>
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
    cmdStatAdjustment
};

enum MMode
{
    modeAC,
    modeHF,
    modeAnz
};

const QString sVoltageChannelDescription = "Measuring channel 0..480V 50Hz/150kHz";
const QString sCurrentChannelDescription = "Measuring channel 0..160A 50Hz/150kHz";
const QString sMeasuringModeDescription = "Measuring mode switch AC,HF";
const QString sMMode[2] = {"AC", "HF"};
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
    virtual void initSCPIConnection(QString leadingNodes);
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
    quint8 m_nMMode;

    quint8 m_nVersionStatus;
    qint8 m_nSerialStatus;

    QString m_ReadVersion(QString& sInput);
    void m_ReadWriteMModeVersion(cProtonetCommand* protoCmd);
    QString m_ReadMModeCatalog(QString& sInput);
    QString m_ReadSenseChannelCatalog(QString& sInput);
    QString m_ReadSenseGroupCatalog(QString& sInput);
    QString m_InitSenseAdjData(QString& sInput);
    QString m_ReadAdjStatus(QString& sInput);

    cNotificationString notifierSenseMMode;
    cNotificationString notifierSenseChannelCat;

    void setNotifierSenseMMode();
    void setNotifierSenseChannelCat();

    void changeSenseMode();
};

#endif // SENSEINTERFACE_H
