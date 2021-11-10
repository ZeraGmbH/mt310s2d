#ifndef BASESYSTEMINFO_H
#define BASESYSTEMINFO_H

#include <QString>

class cBaseSystemInfo
{
public:
    cBaseSystemInfo();
    virtual void getSystemInfo() = 0; // here we fetch all system info
    QString getDeviceName(); // all devices have a device name
    QString getPCBVersion(); // a pcb version
    QString getSerialNumber(); // a serial number

protected:
    bool m_bRead; // we keep in mind whether we could read the information
    QString m_sDeviceName;
    QString m_sPCBVersion;
    QString m_sSerialNumber;
};
#endif // BASESYSTEMINFO_H
