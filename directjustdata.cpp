// implemention cDirectJustData

#include <qdatastream.h>
#include <scpi.h>

#include "protonetcommand.h"
#include "atmel.h"
#include "justdata.h"
#include "scpidelegate.h"
#include "directjustdata.h"

extern cATMEL* pAtmel;

cDirectJustData::cDirectJustData(cSCPI *scpiinterface)
{
    m_pSCPIInterface = scpiinterface;

    m_pGainCorrection = new cJustData(m_pSCPIInterface, GainCorrOrder, 1.0);
    m_pPhaseCorrection = new cJustData(m_pSCPIInterface, PhaseCorrOrder, 0.0);
    m_pOffsetCorrection =  new cJustData(m_pSCPIInterface, OffsetCorrOrder, 0.0);
}


cDirectJustData::~cDirectJustData()
{
    delete m_pGainCorrection; 
    delete m_pPhaseCorrection;
    delete m_pOffsetCorrection;
}


void cDirectJustData::initSCPIConnection(QString leadingNodes)
{
    cSCPIDelegate* delegate;

    if (leadingNodes != "")
        leadingNodes += ":";

    delegate = new cSCPIDelegate(QString("%1CORRECTION").arg(leadingNodes), "GAIN", SCPI::CmdwP , m_pSCPIInterface, DirectJustGain);
    m_DelegateList.append(delegate);
    connect(delegate, SIGNAL(execute(int, cProtonetCommand*)), this, SLOT(executeCommand(int, cProtonetCommand*)));
    delegate = new cSCPIDelegate(QString("%1CORRECTION").arg(leadingNodes), "PHASE", SCPI::isCmdwP, m_pSCPIInterface, DirectJustPhase);
    m_DelegateList.append(delegate);
    connect(delegate, SIGNAL(execute(int, cProtonetCommand*)), this, SLOT(executeCommand(int, cProtonetCommand*)));
    delegate = new cSCPIDelegate(QString("%1CORRECTION").arg(leadingNodes), "OFFSET", SCPI::isCmdwP, m_pSCPIInterface, DirectJustOffset);
    m_DelegateList.append(delegate);
    connect(delegate, SIGNAL(execute(int, cProtonetCommand*)), this, SLOT(executeCommand(int, cProtonetCommand*)));
    delegate = new cSCPIDelegate(QString("%1CORRECTION").arg(leadingNodes), "STATUS", SCPI::isQuery, m_pSCPIInterface, DirectJustStatus);
    m_DelegateList.append(delegate);
    connect(delegate, SIGNAL(execute(int, cProtonetCommand*)), this, SLOT(executeCommand(int, cProtonetCommand*)));
    delegate = new cSCPIDelegate(QString("%1CORRECTION").arg(leadingNodes), "COMPUTE", SCPI::isCmdwP || SCPI::isQuery, m_pSCPIInterface, DirectJustCompute);
    m_DelegateList.append(delegate);
    connect(delegate, SIGNAL(execute(int, cProtonetCommand*)), this, SLOT(executeCommand(int, cProtonetCommand*)));

    connect(m_pGainCorrection, SIGNAL(cmdExecutionDone(cProtonetCommand*)), this, SIGNAL(cmdExecutionDone(cProtonetCommand*)));
    m_pGainCorrection->initSCPIConnection(QString("%1CORRECTION:GAIN").arg(leadingNodes));
    connect(m_pPhaseCorrection, SIGNAL(cmdExecutionDone(cProtonetCommand*)), this, SIGNAL(cmdExecutionDone(cProtonetCommand*)));
    m_pPhaseCorrection->initSCPIConnection(QString("%1CORRECTION:PHASE").arg(leadingNodes));
    connect(m_pOffsetCorrection, SIGNAL(cmdExecutionDone(cProtonetCommand*)), this, SIGNAL(cmdExecutionDone(cProtonetCommand*)));
    m_pOffsetCorrection->initSCPIConnection(QString("%1CORRECTION:OFFSET").arg(leadingNodes));
}
	    

void cDirectJustData::executeCommand(int cmdCode, cProtonetCommand *protoCmd)
{
    switch (cmdCode)
    {
    case DirectJustGain:
        protoCmd->m_sOutput = mReadGainCorrection(protoCmd->m_sInput);
        break;
    case DirectJustPhase:
        protoCmd->m_sOutput = mReadPhaseCorrection(protoCmd->m_sInput);
        break;
    case DirectJustOffset:
        protoCmd->m_sOutput = mReadOffsetCorrection(protoCmd->m_sInput);
        break;
    case DirectJustStatus:
        protoCmd->m_sOutput = m_ReadStatus(protoCmd->m_sInput);
        break;
    case DirectJustCompute:
        protoCmd->m_sOutput = m_ComputeJustData(protoCmd->m_sInput);
        break;
    }

    if (protoCmd->m_bwithOutput)
        emit cmdExecutionDone(protoCmd);
}


QString cDirectJustData::mReadGainCorrection(QString& sInput)
{
    bool ok;
    cSCPICommand cmd = sInput;

    if (cmd.isQuery(1))
    {
        QString spar = cmd.getParam(0);
        double par = spar.toDouble(&ok);
        if (ok)
            return QString("%1").arg(getGainCorrection(par));
        else
            return SCPI::scpiAnswer[SCPI::errval];
    }
    else
        return SCPI::scpiAnswer[SCPI::nak];
}


QString cDirectJustData::mReadPhaseCorrection(QString& sInput)
{
    bool ok;
    cSCPICommand cmd = sInput;

    if (cmd.isQuery(1))
    {
        QString spar = cmd.getParam(0);
        double par = spar.toDouble(&ok);
        if (ok)
            return QString("%1").arg(getPhaseCorrection(par));
        else
            return SCPI::scpiAnswer[SCPI::errval];
    }
    else
        return SCPI::scpiAnswer[SCPI::nak];

}


QString cDirectJustData::mReadOffsetCorrection(QString& sInput)
{
    bool ok;
    cSCPICommand cmd = sInput;

    if (cmd.isQuery(1))
    {
        QString spar = cmd.getParam(0);
        double par = spar.toDouble(&ok);
        if (ok)
            return QString("%1").arg(getOffsetCorrection(par));
        else
            return SCPI::scpiAnswer[SCPI::errval];
    }
    else
        return SCPI::scpiAnswer[SCPI::nak];

}


QString cDirectJustData::m_ReadStatus(QString& sInput)
{
    cSCPICommand cmd = sInput;

    if (cmd.isQuery())
    {
        return QString("%1").arg(getAdjustmentStatus());
    }
    else
        return SCPI::scpiAnswer[SCPI::nak];
}


QString cDirectJustData::m_ComputeJustData(QString& sInput)
{
    cSCPICommand cmd = sInput;

    if (cmd.isCommand(1) && (cmd.getParam(0) == ""))
    {
        bool enable;
        if (pAtmel->getEEPROMAccessEnable(enable) == cmddone)
        {
            if (enable)
            {
                m_pGainCorrection->cmpCoefficients();
                m_pPhaseCorrection->cmpCoefficients();
                m_pOffsetCorrection->cmpCoefficients();
                return SCPI::scpiAnswer[SCPI::ack];
            }
            else
                return SCPI::scpiAnswer[SCPI::erraut];
        }
        else
            return SCPI::scpiAnswer[SCPI::errexec];
    }
    else
        return SCPI::scpiAnswer[SCPI::nak];
}


double cDirectJustData::getGainCorrection(double par)
{
    return m_pGainCorrection->getCorrection(par);
}


double cDirectJustData::getPhaseCorrection(double par)
{
    return m_pPhaseCorrection->getCorrection(par);
}


double cDirectJustData::getOffsetCorrection(double par)
{
    return m_pOffsetCorrection->getCorrection(par);
}


void cDirectJustData::Serialize(QDataStream& qds)  // zum schreiben aller justagedaten in flashspeicher
{
    m_pGainCorrection->Serialize(qds); 
    m_pPhaseCorrection->Serialize(qds);
    m_pOffsetCorrection->Serialize(qds);
}
 

void cDirectJustData::Deserialize(QDataStream& qds) // zum lesen aller justagedaten aus flashspeicher
{
    m_pGainCorrection->Deserialize(qds); 
    m_pPhaseCorrection->Deserialize(qds);
    m_pOffsetCorrection->Deserialize(qds);
}


quint8 cDirectJustData::getAdjustmentStatus()
{
    return m_pGainCorrection->getStatus() & m_pPhaseCorrection->getStatus() & m_pOffsetCorrection->getStatus();

}


void cDirectJustData::initJustData()
{
    m_pGainCorrection->initJustData(1.0);
    m_pPhaseCorrection->initJustData(0.0);
    m_pOffsetCorrection->initJustData(0.0);
}






