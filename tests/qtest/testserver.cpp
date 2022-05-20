#include <scpi.h>
#include "testserver.h"
#include "protonetcommand.h"

TestServer::TestServer()
{
    m_pSCPIInterface = new cSCPI("TestServer");
}

cSCPI *TestServer::getSCPIInterface()
{
    return m_pSCPIInterface;
}

void TestServer::addScpiInterface(cSCPIConnection *connection)
{
    connection->initSCPIConnection("");
    connect(connection, &cSCPIConnection::notifier, this, &TestServer::establishNewNotifier);
    connect(connection, &cSCPIConnection::cmdExecutionDone, this, &TestServer::sendAnswer);
}

void TestServer::executeScpiCmd(QString cmd)
{
    // This is socket-less copy of cPCBServer::SCPIInput()
    cmd.remove('\r'); // we remove cr lf
    cmd.remove('\n');

    QByteArray clientId = QByteArray(); // we set an empty byte array
    cProtonetCommand* protoCmd = new cProtonetCommand(0, false, true, clientId, 0, cmd);
    // peer = 0 means we are working on the scpi socket ....

    cSCPIObject* scpiObject;
    QString dummy;
    if ( (scpiObject =  m_pSCPIInterface->getSCPIObject(cmd, dummy)) != 0) {
        cSCPIDelegate* scpiDelegate = static_cast<cSCPIDelegate*>(scpiObject);
        if (!scpiDelegate->executeSCPI(protoCmd)) {
            protoCmd->m_sOutput = SCPI::scpiAnswer[SCPI::nak];
            emit cmdExecutionDone(protoCmd);
        }
    }
    else {
        protoCmd->m_sOutput = SCPI::scpiAnswer[SCPI::nak];
        emit cmdExecutionDone(protoCmd);
    }
}

void TestServer::initSCPIConnection(QString leadingNodes)
{
    Q_UNUSED(leadingNodes);
}

void TestServer::executeCommand(int cmdCode, cProtonetCommand *protoCmd)
{
    Q_UNUSED(cmdCode);
    Q_UNUSED(protoCmd);
}

void TestServer::sendAnswer(cProtonetCommand *protoCmd)
{
    // This is cPCBServer::sendAnswer / if (protoCmd->m_pPeer == 0)
    // but we publish by signal
    QString answer = protoCmd->m_sOutput+"\n";
    emit sigSendAnswer(answer);
}

void TestServer::establishNewNotifier(cNotificationString *notifier)
{

}
