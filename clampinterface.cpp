#include "clampinterface.h"
#include "mt310s2d.h"
#include "atmel.h"
#include "scpi.h"
#include "clamp.h"
#include "senseinterface.h"
#include "protonetcommand.h"
#include <QDomDocument>
#include <QStringList>

cClampInterface::cClampInterface(cMT310S2dServer *server, cATMEL *controler)
    :m_pMyServer(server), m_pControler(controler)
{
    m_nClampStatus = 0;
    m_pSCPIInterface = m_pMyServer->getSCPIInterface();
}

void cClampInterface::initSCPIConnection(QString leadingNodes)
{
    if (leadingNodes != "")
        leadingNodes += ":";
    cSCPIDelegate* delegate;
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
    if ( m_pControler->readClampStatus(clStat) == ZeraMcontrollerBase::cmddone) {
        qInfo("Clamp status read: 0x%02X", clStat);
        quint16 clChange = clStat ^ m_nClampStatus; // now we know which clamps changed
        for (int i = 0; i < 16; i++) {
            quint16 bmask;
            bmask = 1 << i;
            if ((clChange & bmask) > 0) {
                QString channnelName = m_pMyServer->m_pSenseInterface->getChannelSystemName(i+1);
                if ((m_nClampStatus & bmask) == 0) {
                    // a clamp is connected perhaps it was actually connected
                    m_nClampStatus |= bmask;
                    m_clampHash[channnelName] = new cClamp(m_pMyServer, channnelName, i+1);
                    qInfo("Add clamp channel \"%s\"/%i", qPrintable(channnelName), i);
                    generateAndNotifyClampChannelList();
                }
                else {
                    // a clamp is not connected
                    if (m_clampHash.contains(channnelName)) {
                        // if we already have a clamp on this place it was actually disconnected
                        m_nClampStatus &= (~bmask);
                        cClamp* clamp;
                        clamp = m_clampHash.take(channnelName);
                        channnelName = clamp->getChannelName();
                        qInfo("Remove clamp channel \"%s\"/%i", qPrintable(channnelName), i);
                        generateAndNotifyClampChannelList();
                        delete clamp;
                    }
                    else {
                        qWarning("Clamp %i to remove not found!", i);
                    }
                }
            }
        }
    }
    else {
        qWarning("cClampInterface::actualizeClampStatus: readClampStatus failed");
    }
}

void cClampInterface::executeCommand(int cmdCode, cProtonetCommand *protoCmd)
{
    switch (cmdCode)
    {
    case ClampSystem::cmdClampChannelCat:
        protoCmd->m_sOutput = readClampChannelCatalog(protoCmd->m_sInput);
        break;
    case ClampSystem::cmdClampWrite:
        protoCmd->m_sOutput = writeAllClamps(protoCmd->m_sInput);
        break;
    case ClampSystem::cmdClampImportExport:
        protoCmd->m_sOutput = importExportAllClamps(protoCmd->m_sInput);
        break;
    }
    if (protoCmd->m_bwithOutput) {
        emit cmdExecutionDone(protoCmd);
    }
}

void cClampInterface::generateAndNotifyClampChannelList()
{
    QStringList clampList = m_clampHash.keys();
    m_notifierClampChannelList = clampList.join(";") + ";";
}

QString cClampInterface::readClampChannelCatalog(QString &sInput)
{
    cSCPICommand cmd = sInput;
    if (cmd.isQuery())
    {
        emit notifier(&m_notifierClampChannelList); // enable async notification on clamp catalog change
        generateAndNotifyClampChannelList();
        return m_notifierClampChannelList.getString();
    }
    else {
        return SCPI::scpiAnswer[SCPI::nak];
    }
}

QString cClampInterface::writeAllClamps(QString &sInput)
{
    cSCPICommand cmd = sInput;
    if (cmd.isCommand(1) && (cmd.getParam(0) == "")) {
        if (m_clampHash.count() > 0) {
            bool enable;
            if (pAtmel->getEEPROMAccessEnable(enable) == ZeraMcontrollerBase::cmddone) {
                if (enable) {
                    bool done = true;
                    for(auto clamp : m_clampHash) {
                        done = done && clamp->exportAdjFlash();
                    }
                    if (!done) {
                        return SCPI::scpiAnswer[SCPI::errexec];
                    }
                }
                else {
                    return SCPI::scpiAnswer[SCPI::erraut];
                }
            }
            else {
                return SCPI::scpiAnswer[SCPI::errexec];
            }
        }
        return SCPI::scpiAnswer[SCPI::ack]; // we return ack even in case there is no clamp because nothing went wrong
    }
    else {
        return SCPI::scpiAnswer[SCPI::nak];
    }
}

QString cClampInterface::importExportAllClamps(QString &sInput)
{
    cSCPICommand cmd = sInput;
    if (cmd.isQuery()) {
        QString xmlTotal;
        for(auto clamp : m_clampHash) {
            QString xmlClamp;
            xmlClamp = clamp->exportXMLString(-1);
            xmlClamp.replace("\n","");
            xmlTotal.append(xmlClamp);
        }
        return xmlTotal;
    }
    else {
        // here we got 1 to n concenated xml document's that we want distribute to connected clamps.
        // if we got more than 1 xml document we first check if we have the correct clamps connected
        // we do this using the serial numbers
        // if we only have 1 clamp and 1 xml document we accept this document and take the data for
        // initialzing. so this function can be used by testing field to set up new clamps :-)
        QString answer;
        bool err = false;
        bool enable;
        if (pAtmel->getEEPROMAccessEnable(enable) == ZeraMcontrollerBase::cmddone) {
            if (enable) {
                QString allXML = cmd.getParam(); // we fetch all input
                while (allXML[0] == QChar(' ')) { // we remove all leading blanks
                    allXML.remove(0,1);
                }
                QString sep = "<!DOCTYPE";
                QStringList sl = allXML.split(sep, Qt::SkipEmptyParts);
                int anzXML = sl.count();
                int anzClampTemp = m_clampHash.count();
                if ( !((anzXML > 0) && (anzClampTemp > 0)) ) {
                    err = true;
                    answer = SCPI::scpiAnswer[SCPI::errxml];
                }
                if (!err) {
                    for (int i = 0; (i < anzXML) && (anzClampTemp > 0); i++) {
                        QString XML;
                        cClamp tmpClamp;
                        QDomDocument justqdom( "TheDocument" );
                        XML = sep + sl.at(i);
                        if ( !justqdom.setContent(XML) ) {
                            err = true;
                            answer = SCPI::scpiAnswer[SCPI::errxml];
                            break;
                        }
                        if (tmpClamp.importXMLDocument(&justqdom,true)) {
                            cClamp *pClamp, *pClamp4Use;
                            int anzSNR = 0;
                            QList<QString> keylist = m_clampHash.keys();
                            int anzClamps = keylist.count();
                            for (int j = 0; j < anzClamps; j++) {
                                pClamp = m_clampHash[keylist.at(j)];
                                if (pClamp->getSerial() == tmpClamp.getSerial()) {
                                    pClamp4Use = pClamp;
                                    anzSNR++;
                                }
                            }
                            // From what René told me it seems that 'formatting' clamp's type is possible
                            // only if just one clamp is plugged - see comment above (still - what's going on here
                            // is not clear...)
                            if ( (anzSNR == 1) || ( (anzSNR == 0) && (anzXML == 1) && (anzClamps == 1)) ) {
                                // we have 1 matching serial number
                                anzClampTemp--;
                                if (anzSNR == 0) {
                                    pClamp4Use = pClamp;
                                    pClamp4Use->importXMLDocument(&justqdom, true); // if we only have 1 xml and 1 clamp we accept all information
                                }
                                else {
                                    pClamp4Use->importXMLDocument(&justqdom, false); // otherwise clamp type cannot be changed
                                }
                                m_pMyServer->m_pSenseInterface->m_ComputeSenseAdjData();
                                // then we let it compute its new adjustment coefficients... we simply call senseinterface's compute
                                // command. we compute a little bit to much but this doesn't matter at all
                                if (!pClamp4Use->exportAdjFlash()) {// and then we program the clamp
                                    err = true;
                                    answer = SCPI::scpiAnswer[SCPI::errexec];
                                    break;
                                }
                            }
                        }
                        else {
                            err = true;
                            answer = SCPI::scpiAnswer[SCPI::errxml];
                            break;
                        }
                    }
                }
            }
            else {
                err = true;
                answer = SCPI::scpiAnswer[SCPI::erraut];
            }
        }
        else {
            err = true;
            answer = SCPI::scpiAnswer[SCPI::errexec];
        }
        if (!err) {
            answer = SCPI::scpiAnswer[SCPI::ack];
        }
        return answer;
    }
}
