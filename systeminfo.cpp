#include "scpi.h"
#include "atmelsysctrl.h"
#include "atmel.h"
#include "systeminfo.h"

cSystemInfo::cSystemInfo()
    :cBaseSystemInfo()
{
    m_sLCAVersion = m_sSysCTRLVersion = m_sCTRLVersion = "Unknown";
    getSystemInfo();
}


void cSystemInfo::getSystemInfo()
{
    int rm = ZeraMcontrollerBase::cmddone;
    rm |= pAtmel->readDeviceName(m_sDeviceName);
    rm |= pAtmel->readPCBVersion(m_sPCBVersion);
    rm |= pAtmel->readLCAVersion(m_sLCAVersion);
    rm |= pAtmelSys->readCTRLVersion(m_sSysCTRLVersion);
    rm |= pAtmel->readCTRLVersion(m_sCTRLVersion);
    rm |= pAtmel->readSerialNumber(m_sSerialNumber);
    m_bRead = (rm == ZeraMcontrollerBase::cmddone);
}


QString cSystemInfo::getDeviceVersion()
{
    QString s = QString ("DEVICE: %1;PCB: %2;LCA: %3;CTRL: %4")
            .arg(m_sDeviceName)
            .arg(m_sPCBVersion)
            .arg(m_sLCAVersion)
            .arg(m_sCTRLVersion);
    return s;
}


QString cSystemInfo::getLCAVersion()
{
    QString s;

    if (m_bRead)
        return m_sLCAVersion;
    else
        return s = SCPI::scpiAnswer[SCPI::errexec];
}


QString cSystemInfo::getSysCTRLVersion()
{
    QString s;

    if (m_bRead)
        return m_sSysCTRLVersion;
    else
        return s = SCPI::scpiAnswer[SCPI::errexec];
}


QString cSystemInfo::getCTRLVersion()
{
    QString s;

    if (m_bRead)
        return m_sCTRLVersion;
    else
        return s = SCPI::scpiAnswer[SCPI::errexec];
}







