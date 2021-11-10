#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include <QString>
#include "basesysteminfo.h"

class cSystemInfo: public cBaseSystemInfo
{
public:
    cSystemInfo();
    virtual void getSystemInfo(); // here we fetch all system info
    QString getDeviceVersion();
    QString getLCAVersion();
    QString getSysCTRLVersion();
    QString getCTRLVersion();

protected:
    QString m_sLCAVersion;
    QString m_sSysCTRLVersion;
    QString m_sCTRLVersion;
};
#endif // SYSTEMINFO_H
