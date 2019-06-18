#include "clampinterface.h"
#include "mt310s2d.h"
#include "atmel.h"
#include "scpi.h"
#include "clamp.h"
#include "senseinterface.h"
#include "protonetcommand.h"


cClampInterface::cClampInterface(cMT310S2dServer *server, cATMEL *controler)
    :m_pMyServer(server), m_pControler(controler)
{
    m_nClampStatus = 0;
    m_pSCPIInterface = m_pMyServer->getSCPIInterface();
}


void cClampInterface::initSCPIConnection(QString leadingNodes)
{
    cSCPIDelegate* delegate;

    if (leadingNodes != "")
        leadingNodes += ":";

    delegate = new cSCPIDelegate(QString("%1SYSTEM:CLAMP:CHANNEL").arg(leadingNodes),"CATALOG",SCPI::isQuery, m_pSCPIInterface, ClampSystem::cmdClampChannelCat);
    m_DelegateList.append(delegate);
    connect(delegate, SIGNAL(execute(int, cProtonetCommand*)), this, SLOT(executeCommand(int, cProtonetCommand*)));
    delegate = new cSCPIDelegate(QString("%1SYSTEM:CLAMP").arg(leadingNodes),"WRITE",SCPI::isCmd, m_pSCPIInterface, ClampSystem::cmdClampWrite);
    m_DelegateList.append(delegate);
    connect(delegate, SIGNAL(execute(int, cProtonetCommand*)), this, SLOT(executeCommand(int, cProtonetCommand*)));
    delegate = new cSCPIDelegate(QString("%1SYSTEM:ADJUSTMENT:CLAMP").arg(leadingNodes),"XML",SCPI::isQuery | SCPI::isCmdwP, m_pSCPIInterface, ClampSystem::cmdClampImportExport);
    m_DelegateList.append(delegate);
    connect(delegate, SIGNAL(execute(int, cProtonetCommand*)), this, SLOT(executeCommand(int, cProtonetCommand*)));
}


void cClampInterface::actualizeClampStatus()
{
    quint16 clStat;
    quint16 clChange;

    if ( m_pControler->readClampStatus(clStat) == cmddone)
    {
        clChange = clStat ^ m_nClampStatus; // now we know which clamps changed
        for (int i = 0; i < 16; i++)
        {
            quint16 bmask;
            bmask = 1 << i;
            if ((clChange & bmask) > 0)
            {
                if ((m_nClampStatus & bmask) == 0)
                {
                    // a clamp is connected perhaps it was actually connected
                    m_nClampStatus |= bmask;
                    QString s = m_pMyServer->m_pSenseInterface->getChannelSystemName(i+1);
                    clampHash[i] = new cClamp(m_pMyServer, s, i+1);
                    addChannel(s);
                }
                else
                {
                    // a clamp is not connected
                    if (clampHash.contains(i))
                    {   // if we already have a clamp on this place it was actually disconnected
                        m_nClampStatus &= (~bmask);
                        cClamp* clamp;
                        clamp = clampHash.take(i);
                        removeChannel(clamp->getChannelName());
                        delete clamp;
                    }
                }
            }
        }
    }
}


void cClampInterface::addChannel(QString channel)
{
    m_ClampChannelList.append(channel);
}


void cClampInterface::removeChannel(QString channel)
{
    m_ClampChannelList.removeAll(channel);
}

void cClampInterface::executeCommand(int cmdCode, cProtonetCommand *protoCmd)
{
    switch (cmdCode)
    {
    case ClampSystem::cmdClampChannelCat:
        protoCmd->m_sOutput = m_ReadClampChannelCatalog(protoCmd->m_sInput);
        break;
    case ClampSystem::cmdClampWrite:
        protoCmd->m_sOutput = m_WriteAllClamps(protoCmd->m_sInput);
        break;
    case ClampSystem::cmdClampImportExport:
        protoCmd->m_sOutput = m_ImportExportAllClamps(protoCmd->m_sInput);
        break;
    }

    if (protoCmd->m_bwithOutput)
        emit cmdExecutionDone(protoCmd);
}

QString cClampInterface::m_ReadClampChannelCatalog(QString &sInput)
{
    cSCPICommand cmd = sInput;

    if (cmd.isQuery())
    {
        QString s = "";
        int len;
        len = m_ClampChannelList.count();
        if (len > 0)
        {
            int i;
            for (i = 0; i < len-1; i++)
                s += m_ClampChannelList.at(i) + ";";
            s += m_ClampChannelList.at(i);
        }

        s += ";"; // no clamp present
        return s;
    }
    else
        return SCPI::scpiAnswer[SCPI::nak];
}


QString cClampInterface::m_WriteAllClamps(QString &sInput)
{
    cSCPICommand cmd = sInput;

    if (cmd.isCommand(0))
    {
        int n;
        n = clampHash.count();

        if (n > 0)
        {
            bool done;
            QList<int> keylist;

            keylist = clampHash.keys();
            done = true;

            for (int i = 0; i < n; n++)
            {
                cClamp* pClamp;
                pClamp = clampHash[keylist.at(i)];
                done = done && pClamp->exportAdjFlash();
            }

            if (!done)
                return SCPI::scpiAnswer[SCPI::errexec];
        }

        return SCPI::scpiAnswer[SCPI::ack]; // we return ack even in case there is no clamp because nothing went wrong
    }
    else
        return SCPI::scpiAnswer[SCPI::nak];
}


QString cClampInterface::m_ImportExportAllClamps(QString &sInput)
{
    cSCPICommand cmd = sInput;

    if (cmd.isQuery())
    {
        QString s;
        int n;
        n = clampHash.count();

        if (n > 0)
        {
            QList<int> keylist;
            cClamp* pClamp;

            keylist = clampHash.keys();
            for (int i = 0; i < n; i++)
            {
                pClamp = clampHash[keylist.at(i)];
                s.append(pClamp->exportXMLString(-1));
            }
        }

        return s;
    }
    else
    {

    }
}
