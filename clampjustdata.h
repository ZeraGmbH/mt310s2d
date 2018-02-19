#ifndef CLAMPJUSTDATA
#define CLAMPJUSTDATA

#include <QObject>

#include "directjustdata.h"
#include "senserange.h"

// a clamp range consists of 2 stages . 1st the clamp itself and 2nd a voltage input range
// assigned to the clamp. so we need an interface for the clamp adjustment data (same as
// a normal senserange) but the adjustment data is the combination of the clamp's and the
// the voltage input range's adjustment data

class cSCPI;

class cClampJustData: public cDirectJustData
{
    Q_OBJECT

public:
    cClampJustData(cSCPI* scpiinterface, cSenseRange* cascadedRange);

protected:
    virtual double getGainCorrection(double par);
    virtual double getPhaseCorrection(double par);
    virtual double getOffsetCorrection(double par);

private:
    cSenseRange* m_pFirstStageRange; //
};

#endif // CLAMPJUSTDATA

