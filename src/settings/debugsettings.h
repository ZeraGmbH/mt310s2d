#ifndef DEBUGSETTINGS_H
#define DEBUGSETTINGS_H

#include "xmlsettings.h"
#include <xmlconfigreader.h>

namespace DebugSettings
{
enum debugconfigstate
{
    setdebuglevel
};
}

class cDebugSettings: public cXMLSettings
{
    Q_OBJECT
public:
    cDebugSettings(Zera::XMLConfig::cReader *xmlread);
    quint8 getDebugLevel();
public slots:
    virtual void configXMLInfo(QString key);
private:
    quint8 m_nDebugLevel;
};

#endif // DEBUGSETTINGS_H
