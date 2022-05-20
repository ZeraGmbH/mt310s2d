#ifndef TESTSERVER_H
#define TESTSERVER_H

#include "scpiconnection.h"

class TestServer: public cSCPIConnection
{
public:
    TestServer();
    cSCPI *getSCPIInterface();
    void executeScpiCmd(QString cmd);
    void initSCPIConnection(QString leadingNodes) override;
    void executeCommand(int cmdCode, cProtonetCommand* protoCmd) override;
};

#endif // TESTSERVER_H
