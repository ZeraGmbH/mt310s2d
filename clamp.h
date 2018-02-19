#ifndef CLAMP_H
#define CLAMP_H

#include <QList>
#include <QDataStream>
#include <QDateTime>

#include "adjflash.h"
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
        cmdStatAdjustment
    };

}


enum clamps { undefined, CL120A, CL300A, CL1000A , anzCL};

class cMT310S2dServer;
class cSenseRange;
class QDomDocument;

class cClamp: public cAdjFlash, public cSCPIConnection
{
public:
    cClamp(cMT310S2dServer *server, QString channelName);
    virtual ~cClamp();
    virtual quint8 getAdjustmentStatus();
    virtual void initSCPIConnection(QString);

protected slots:
    virtual void executeCommand(int cmdCode, cProtonetCommand* protoCmd);

protected:
    QList<cSenseRange*> m_RangeList;

    virtual void exportAdjData(QDataStream& stream);
    virtual bool importAdjData(QDataStream& stream);
    virtual QString exportXMLString();
    virtual bool importXMLDocument(QDomDocument* qdomdoc);

    virtual void setI2CMux();
    virtual quint8 readClampType();
    virtual void initClamp(quint8 type);
    void addSense();
    void addSenseInterface();
    void addSystAdjInterface();

private:
    cMT310S2dServer* m_pMyServer;
    QString m_sChannelName;
    QString m_sName;
    QString m_sSerial;
    QString m_sVersion;
    quint8 m_nType; // 0 is undefined type
    quint32 m_nFlags; // for future purpose
    QDateTime mDateTime;
    bool m_bSet;

    cSenseRange* getRange(QString name);

    QString m_ReadWriteSerial(QString &sInput);
    QString m_ReadWriteVersion(QString &sInput);
    QString m_ReadWriteType(QString &sInput);
    QString m_ReadWriteName(QString &sInput);
    QString m_WriteFlash(QString &sInput);
    QString m_ReadFlash(QString &sInput);
    QString m_ReadChksum(QString &sInput);
    QString m_ReadAdjStatus(QString &sInput);

};

#endif // CLAMP_H
