#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <scpiconnection.h>
#include <notificationstring.h>

class TestServer: public cSCPIConnection
{
    Q_OBJECT
public:
    TestServer();
    cSCPI *getSCPIInterface();
    void addScpiInterface(cSCPIConnection* connection);
    void executeScpiCmd(QString cmd);

    // mandatory overrides - not used...
    void initSCPIConnection(QString leadingNodes) override;
    void executeCommand(int cmdCode, cProtonetCommand* protoCmd) override;
signals:
    void sigSendAnswer(QString answer);
private slots:
    void sendAnswer(cProtonetCommand* protoCmd);
    void establishNewNotifier(cNotificationString* notifier);
};

#endif // TESTSERVER_H
