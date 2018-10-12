#include <scpi.h>

#include "clampjustdata.h"
#include "justdata.h"

cClampJustData::cClampJustData(cSCPI *scpiinterface, cSenseRange *cascadedRange)
    :cMT310S2JustData(scpiinterface), m_pFirstStageRange(cascadedRange)
{   
}


double cClampJustData::getGainCorrection(double par)
{
    return m_pGainCorrection->getCorrection(par) * m_pFirstStageRange->getJustData()->m_pGainCorrection->getCorrection(par);
}


double cClampJustData::getJustGainCorrection(double par)
{
    return m_pGainCorrection->getCorrection(par);
}


double cClampJustData::getPhaseCorrection(double par)
{
    return m_pPhaseCorrection->getCorrection(par) + m_pFirstStageRange->getJustData()->m_pPhaseCorrection->getCorrection(par);
}


double cClampJustData::getJustPhaseCorrection(double par)
{
    return m_pPhaseCorrection->getCorrection(par);
}


double cClampJustData::getOffsetCorrection(double par)
{
    return m_pOffsetCorrection->getCorrection(par) + m_pFirstStageRange->getJustData()->m_pOffsetCorrection->getCorrection(par);
}


double cClampJustData::getJustOffsetCorrection(double par)
{
    return m_pOffsetCorrection->getCorrection(par);
}


