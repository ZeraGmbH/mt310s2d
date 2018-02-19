#ifndef SCHEADSETTINGS_H
#define SCHEADSETTINGS_H


#include <QObject>
#include <QList>

#include "xmlsettings.h"

namespace SCHeadSystem
{
enum configstate
{
    cfgSH0Alias,
    cfgSH0avail,
};


struct cChannelSettings // what we want to get configured
{
    QString m_sAlias; // the names channel
    bool avail; // is this channel available ?
};
}


class QString;

namespace Zera
{
namespace XMLConfig
{
    class cReader;
}
}


class cSCHeadSettings : public cXMLSettings
{
    Q_OBJECT

public:
    cSCHeadSettings(Zera::XMLConfig::cReader *xmlread);
    ~cSCHeadSettings();
    QList<SCHeadSystem::cChannelSettings*>& getChannelSettings();


public slots:
    virtual void configXMLInfo(QString key);

private:
    QList<SCHeadSystem::cChannelSettings*> m_ChannelSettingsList;
};



#endif // SCHEADSETTINGS_H
