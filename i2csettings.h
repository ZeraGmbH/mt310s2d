#ifndef I2CSETTINGS_H
#define I2CSETTINGS_H

#include "xmlsettings.h"
#include "i2cmuxer.h"
#include <xmlconfigreader.h>
#include <QObject>
#include <QString>

namespace i2cSettings
{
enum member
{
    atmelsys,
    atmel,
    flashmux,
    flash,
    clampflash
};

enum configstate
{
    SetDevNode,
    SetAtmelSysAdr,
    SetAtmelAdr,
    SetFlashMuxAdr,
    SetFlashAdr,
    SetClampFlashAdr
};
}

class cI2CSettings : public cXMLSettings
{
    Q_OBJECT
public:
    cI2CSettings(Zera::XMLConfig::cReader *xmlread);
    quint8 getI2CAdress(i2cSettings::member member);
    QString& getDeviceNode();
    I2cMuxer::Ptr createMuxer(quint8 ctrlChannel);
public slots:
    virtual void configXMLInfo(QString key);
private:
    QString m_sDeviceNode;
    quint8 m_nAtmelSysAdr, m_nAtmelAdr, m_nFlashMuxAdr, m_nFlashAdr, m_nClampFlashAdr;
};


#endif // I2CSETTINGS_H
