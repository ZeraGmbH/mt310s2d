#include <scpi.h>

#include "clampjustdata.h"
#include "justdata.h"

cClampJustData::cClampJustData(cSCPI *scpiinterface, cSenseRange *cascadedRange)
    :cDirectJustData(scpiinterface), m_pFirstStageRange(cascadedRange)
{   
}


double cClampJustData::getGainCorrection(double par)
{
    return m_pGainCorrection->getCorrection(par) * m_pFirstStageRange->getJustData()->m_pGainCorrection->getCorrection(par);
}


double cClampJustData::getPhaseCorrection(double par)
{
    return m_pPhaseCorrection->getCorrection(par) + m_pFirstStageRange->getJustData()->m_pPhaseCorrection->getCorrection(par);
}


double cClampJustData::getOffsetCorrection(double par)
{
    return m_pOffsetCorrection->getCorrection(par) + m_pFirstStageRange->getJustData()->m_pOffsetCorrection->getCorrection(par);
}


