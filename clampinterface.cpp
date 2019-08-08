#include <QDomDocument>

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

            for (int i = 0; i < n; i++)
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
                QString s2;
                pClamp = clampHash[keylist.at(i)];
                s2 = pClamp->exportXMLString(-1);
                s2.replace("\n","");
                s.append(s2);
            }
        }

        return s;
    }
    else
    {
        // here we got 1 to n concenated xml document's that we want distribute to connected clamps.
        // if we got more than 1 xml document we first check if we have the correct clamps connected
        // we do this using the serial numbers
        // if we only have 1 clamp and 1 xml document we accept this document and take the data for
        // initialzing. so this function can be used by testing field to set up new clamps :-)

        QStringList sl, sl2;
        QString allXML;
        QString answer;
        QString sep = "<!DOCTYPE";
        int anzXML, anzClamp;
        bool err;

        allXML = cmd.getParam(); // we fetch all input
        while (allXML[0] == QChar(' ')) // we remove all leading blanks
            allXML.remove(0,1);

        sl = allXML.split(sep);

        if (sl.count() > 0)
            for (int i = 0; i < sl.count(); i++)
                if (sl.at(i).length() > 0)
                    sl2.append(sl.at(i));

        anzXML = sl2.count();
        anzClamp = clampHash.count();

        if ( !((anzXML >0) && (anzClamp > 0)) )
        {
            err = true;
            answer = SCPI::scpiAnswer[SCPI::errxml];
        }

        int i;

        if (!err)
            for (i = 0; (i < anzXML) && (anzClamp > 0); i++)
            {
                QString XML;
                cClamp tmpClamp;
                QDomDocument justqdom( "TheDocument" );

                XML = sep + sl2.at(i);
                if ( !justqdom.setContent(XML) )
                {
                    err = true;
                    answer = SCPI::scpiAnswer[SCPI::errxml];
                    break;
                }

                if (tmpClamp.importXMLDocument(&justqdom,true))
                {
                    QList<int> keylist;
                    cClamp *pClamp, *pClamp4Use;
                    int anzClamps;
                    int anzSNR;

                    anzSNR = 0;
                    keylist = clampHash.keys();
                    anzClamps = keylist.count();

                    for (int j = 0; j < anzClamps; j++)
                    {
                        pClamp = clampHash[keylist.at(j)];
                        if (pClamp->getSerial() == tmpClamp.getSerial())
                        {
                            pClamp4Use = pClamp;
                            anzSNR++;
                        }
                    }

                    if ( (anzSNR == 1) /*|| ( (anzSNR == 0) && (anzXML == 1) && (anzClamps == 1))*/ )
                    // we have 1 matching serial number
                    {
                        anzClamp--;
                        pClamp4Use->importXMLDocument(&justqdom,false); // we let the found clamp import its xml data
                        m_pMyServer->m_pSenseInterface->m_ComputeSenseAdjData();
                        // then we let it compute its new adjustment coefficients... we simply call senseinterface's compute
                        // command. we compute a little bit to much but this doesn't matter at all
                        if (!pClamp4Use->exportAdjFlash()) // and then we program the clamp
                        {
                            err = true;
                            answer = SCPI::scpiAnswer[SCPI::errexec];
                            break;
                        }
                    }
                }
                else
                {
                    err = true;
                    answer = SCPI::scpiAnswer[SCPI::errxml];
                    break;
                }

            }

        if (!err)
            answer = SCPI::scpiAnswer[SCPI::ack];

        return answer;
    }
}
