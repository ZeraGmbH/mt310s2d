#ifndef CLAMP_H
#define CLAMP_H

#include <QList>
#include <QDataStream>
#include <QDateTime>
#include <QDomDocument>
#include <QDomElement>

#include "adjflash.h"
#include "adjxml.h"
#include "scpiconnection.h"

namespace clamp
{
    enum Commands
    {
        cmdSerial,
        cmdVersion,
        cmdType,
        cmdName,
        cmdFlashWrite,
        cmdFlashRead,
        cmdChksum,
        cmdXMLWrite,
        cmdXMLRead,
        cmdStatAdjustment
    };
}

enum clamps
{
    undefined,
    CL120A,
    CL300A,
    CL1000A,
    EMOB32,
    EMOBDcDualTest,

    anzCL
};

class cMT310S2dServer;
class cSenseRange;
class QDomDocument;

class cClamp: public cAdjFlash, public cAdjXML, public cSCPIConnection
{
public:
    cClamp(){m_pMyServer = 0;}
    cClamp(cMT310S2dServer *server, QString channelName, quint8 ctrlChannel);
    virtual ~cClamp();
    virtual quint8 getAdjustmentStatus() override;
    virtual void initSCPIConnection(QString) override;
    QString getChannelName();
    QString getSerial();
    bool addSecondaryRanges(QString secondaryChannelName);
    virtual QString exportXMLString(int indent = 1) override;
    bool importXMLDocument(QDomDocument *qdomdoc, bool ignoreType);

protected slots:
    virtual void executeCommand(int cmdCode, cProtonetCommand* protoCmd) override;

protected:
    virtual void exportAdjData(QDataStream& stream) override;
    virtual bool importAdjData(QDataStream& stream) override;
    virtual bool importXMLDocument(QDomDocument* qdomdoc) override;

    virtual void setI2CMux() override;

private:
    void initClamp(quint8 type);
    void addSense();
    void addSenseInterface();
    void addSystAdjInterface();
    void addSystAdjInterfaceChannel(QString channelName);
    QString getClampName(quint8 type);
    void setI2CMuxClamp();
    cSenseRange* getRange(QString name);
    quint8 readClampType();
    void removeAllRanges();
    void exportRangeXml(QDomDocument &justqdom, QDomElement &typeTag, cSenseRange *range);

    QString handleScpiReadWriteSerial(QString &scpiCmdStr);
    QString handleScpiReadWriteVersion(QString &scpiCmdStr);
    QString handleScpiReadWriteType(QString &scpiCmdStr);
    QString handleScpiReadWriteName(QString &scpiCmdStr);
    QString handleScpiWriteFlash(QString &scpiCmdStr);
    QString handleScpiReadFlash(QString &scpiCmdStr);
    QString handleScpiReadChksum(QString &scpiCmdStr);
    QString handleScpiWriteXML(QString &scpiCmdStr);
    QString handleScpiReadXML(QString &scpiCmdStr);
    QString handleScpiReadAdjStatus(QString &scpiCmdStr);

    cMT310S2dServer* m_pMyServer;

    QList<cSenseRange*> m_RangeList;
    QString m_sChannelName;
    QList<cSenseRange*> m_RangeListSecondary;
    QString m_sChannelNameSecondary;

    quint8 m_nCtrlChannel;
    QString m_sClampTypeName;
    QString m_sSerial;
    QString m_sVersion;
    quint8 m_nType; // 0 is undefined type
    quint32 m_nFlags; // for future purpose
    QDateTime mDateTime;
};

#endif // CLAMP_H
