#include "clampinterface.h"
#include "mt310s2d.h"
#include "atmel.h"
#include "clamp.h"


cClampInterface::cClampInterface(cMT310S2dServer *server, cATMEL *controler)
    :m_pMyServer(server), m_pControler(controler)
{
    m_nClampStatus = 0;
    actualizeClampStatus(); // at start we look for already connected clamps
}


void cClampInterface::actualizeClampStatus()
{
    quint16 clStat;
    quint16 clChange;

    m_pControler->readClampStatus(clStat);
    clChange = clStat ^ m_nClampStatus; // now we know which clamps changed
    for (int i = 0; i < 16; i++)
    {
        quint16 bmask;
        bmask = 1 << i;
        if ((clChange & bmask) > 0)
        {
            if ((m_nClampStatus & bmask) == 0)
            {
                // a clamp was connected
                m_nClampStatus |= bmask;
                clampHash[i] = new cClamp(m_pMyServer,QString("m%1").arg(i));
            }
            else
            {
                // a clamp was disconnected
                cClamp* clamp;
                clamp = clampHash.take(i);
                delete clamp;
            }
        }
    }
}



