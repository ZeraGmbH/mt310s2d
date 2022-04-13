#ifndef HKEYCHANNEL_H
#define HKEYCHANNEL_H

#include "scpiconnection.h"
#include "hkeysettings.h"
#include <QObject>
#include <scpi.h>

namespace HKEYChannel
{
enum Commands
{
    cmdAlias,
    cmdStatus
};
}

class cHKeyChannel : public cSCPIConnection
{
    Q_OBJECT

public:
    cHKeyChannel(cSCPI* scpiiinterface, QString description, quint8 nr, HKeySystem::cChannelSettings* cSettings);
    virtual void initSCPIConnection(QString leadingNodes);

    QString& getName();
    QString& getAlias();
    QString& getDescription();
    bool isAvail();

protected slots:
    virtual void executeCommand(int cmdCode, cProtonetCommand* protoCmd);

private:
    QString m_sName; // the channel's name
    QString m_sAlias;
    QString m_sDescription; // the channel's brief description
    bool m_bAvail; // is this channel available ?

    QString m_ReadAlias(QString& sInput);
    QString m_ReadChannelStatus(QString& sInput);
};

#endif // HKEYCHANNEL_H
