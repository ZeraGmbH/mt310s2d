#ifndef ADJUSTMENT_H
#define ADJUSTMENT_H

namespace Adjustment
{
enum jDataStatus
{
    adjusted,
    notAdjusted,
    wrongVERS = 2,
    wrongSNR = 4
};
}

class QString;
class cMT310S2dServer;
class cAdjFlash;

class cAdjustment
{
public:
    cAdjustment(cMT310S2dServer* server);
    void addAdjFlashObject(cAdjFlash* obj);
    void removeAdjFlashObject(cAdjFlash* obj);
    quint8 getAdjustmentStatus();

private:
    cMT310S2dServer* m_pMyServer;
    QList<cAdjFlash*> m_AdjFlashList;
};

#endif // ADJUSTMENT_H
