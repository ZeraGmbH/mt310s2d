#include "scpi.h"
#include "atmelsysctrl.h"
#include "atmel.h"
#include "basesysteminfo.h"

cBaseSystemInfo::cBaseSystemInfo()
{
    m_sDeviceName = m_sPCBVersion = m_sSerialNumber = "Unknown";
}


QString cBaseSystemInfo::getDeviceName()
{
    QString s;

    if (m_bRead)
        return m_sDeviceName;
    else
        return s = SCPI::scpiAnswer[SCPI::errexec];
}


QString cBaseSystemInfo::getPCBVersion()
{
    QString s;

    if (m_bRead)
        return m_sPCBVersion;
    else
        return s = SCPI::scpiAnswer[SCPI::errexec];
}


QString cBaseSystemInfo::getSerialNumber()
{
    QString s;

    if (m_bRead)
        return m_sSerialNumber;
    else
        return s = SCPI::scpiAnswer[SCPI::errexec];
}







