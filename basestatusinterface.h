#ifndef BASESTATUSINTERFACE_H
#define BASESTATUSINTERFACE_H

#include <QObject>
#include <QList>
#include <scpi.h>

#include "mt310s2d.h"
#include "scpiconnection.h"
#include "scpidelegate.h"


class cBaseStatusInterface: public cSCPIConnection
{
    Q_OBJECT

public:
    cBaseStatusInterface(){}

protected slots:
    virtual quint8 getDeviceStatus() = 0; // returns if device is present(>0) or not (=0) implementation is device specific

};

#endif // BASESTATUSINTERFACE_H
