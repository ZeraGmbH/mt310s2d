#include <scpi.h>
#include <scpicommand.h>

#include "mt310s2d.h"
#include "atmel.h"
#include "adjustment.h"
#include "scpidelegate.h"
#include "basesysteminfo.h"
#include "basesysteminterface.h"
#include "senseinterface.h"
#include "protonetcommand.h"


cBaseSystemInterface::cBaseSystemInterface(cPCBServer* server)
    :m_pMyServer(server)
{
    m_pSCPIInterface = m_pMyServer->getSCPIInterface();
}


void cBaseSystemInterface::initSCPIConnection(QString leadingNodes)
{
    cSCPIDelegate* delegate;

    if (leadingNodes != "")
        leadingNodes += ":";

    delegate = new cSCPIDelegate(QString("%1SYSTEM:VERSION").arg(leadingNodes),"SERVER", SCPI::isQuery, m_pSCPIInterface, SystemSystem::cmdVersionServer);
    m_DelegateList.append(delegate);
    connect(delegate, SIGNAL(execute(int, cProtonetCommand*)), this, SLOT(executeCommand(int, cProtonetCommand*)));
    delegate = new cSCPIDelegate(QString("%1SYSTEM:VERSION").arg(leadingNodes), "PCB", SCPI::isQuery | SCPI::isCmdwP, m_pSCPIInterface, SystemSystem::cmdVersionPCB);
    m_DelegateList.append(delegate);
    connect(delegate, SIGNAL(execute(int, cProtonetCommand*)), this, SLOT(executeCommand(int, cProtonetCommand*)));
    delegate = new cSCPIDelegate(QString("%1SYSTEM").arg(leadingNodes), "SERIAL", SCPI::isQuery | SCPI::isCmdwP , m_pSCPIInterface, SystemSystem::cmdSerialNumber);
    m_DelegateList.append(delegate);
    connect(delegate, SIGNAL(execute(int, cProtonetCommand*)), this, SLOT(executeCommand(int, cProtonetCommand*)));
    delegate = new cSCPIDelegate(QString("%1SYSTEM:INTERFACE").arg(leadingNodes), "READ", SCPI::isQuery, m_pSCPIInterface, SystemSystem::cmdInterfaceRead);
    m_DelegateList.append(delegate);
    connect(delegate, SIGNAL(execute(int, cProtonetCommand*)), this, SLOT(executeCommand(int, cProtonetCommand*)));
}


void cBaseSystemInterface::executeCommand(int cmdCode, cProtonetCommand *protoCmd)
{
    bool done = true;

    switch (cmdCode)
    {
    case SystemSystem::cmdVersionServer:
        protoCmd->m_sOutput = m_ReadServerVersion(protoCmd->m_sInput);
        break;
    case SystemSystem::cmdVersionPCB:
        protoCmd->m_sOutput = m_ReadWritePCBVersion(protoCmd->m_sInput);
        break;
    case SystemSystem::cmdSerialNumber:
        protoCmd->m_sOutput = m_ReadWriteSerialNumber(protoCmd->m_sInput);
        break;
    case SystemSystem::cmdInterfaceRead:
        protoCmd->m_sOutput = m_InterfaceRead(protoCmd->m_sInput);
        break;
    default:
        done = false;
    }

    if (protoCmd->m_bwithOutput && done)
        emit cmdExecutionDone(protoCmd);
}


QString cBaseSystemInterface::m_ReadServerVersion(QString &sInput)
{
    QString s;
    cSCPICommand cmd = sInput;

    if ( cmd.isQuery() )
    {
        s = m_pMyServer->getVersion();
    }
    else
        s = SCPI::scpiAnswer[SCPI::nak];

    return s;
}


QString cBaseSystemInterface::m_ReadDeviceName(QString& sInput)
{
    QString s;
    cSCPICommand cmd = sInput;

    if (cmd.isQuery())
    {
        return m_pMyServer->m_pSystemInfo->getDeviceName();
    }
    else
        return SCPI::scpiAnswer[SCPI::nak];
}


QString cBaseSystemInterface::m_ReadWritePCBVersion(QString &sInput)
{
    QString s;
    int ret = ZeraMcontrollerBase::cmdfault;
    cSCPICommand cmd = sInput;

    if (cmd.isQuery())
    {
        s = m_pMyServer->m_pSystemInfo->getPCBVersion();
    }
    else
    {
        if (cmd.isCommand(1))
        {
            QString Version = cmd.getParam(0);
            ret = pAtmel->writePCBVersion(Version);
            m_pMyServer->m_pSystemInfo->getSystemInfo(); // read back info
        }

        m_genAnswer(ret, s);
    }

    return s;
}


QString cBaseSystemInterface::m_ReadWriteSerialNumber(QString &sInput)
{
    ZeraMcontrollerBase::atmelRM ret = ZeraMcontrollerBase::cmdfault;
    QString s;
    cSCPICommand cmd = sInput;

    if (cmd.isQuery())
    {
        {
            s = m_pMyServer->m_pSystemInfo->getSerialNumber();
        }
    }
    else
    {
        if (cmd.isCommand(1))
        {
            QString Serial = cmd.getParam(0);
            ret = pAtmel->writeSerialNumber(Serial);
            m_pMyServer->m_pSystemInfo->getSystemInfo(); // read back info
        }

        m_genAnswer(ret, s);
    }

    return s;
}


QString cBaseSystemInterface::m_InterfaceRead(QString &sInput)
{
    cSCPICommand cmd = sInput;

    if (cmd.isQuery())
    {
        QString s;
        m_pMyServer->getSCPIInterface()->exportSCPIModelXML(s);
        return s;
    }
    else
        return SCPI::scpiAnswer[SCPI::nak];
}


void cBaseSystemInterface::m_genAnswer(int select, QString &answer)
{
    switch (select)
    {
    case ZeraMcontrollerBase::cmddone:
        answer = SCPI::scpiAnswer[SCPI::ack];
        break;
    case ZeraMcontrollerBase::cmdfault:
        answer = SCPI::scpiAnswer[SCPI::nak];
        break;
    case ZeraMcontrollerBase::cmdexecfault:
        answer = SCPI::scpiAnswer[SCPI::errexec];
        break;
    }
}



