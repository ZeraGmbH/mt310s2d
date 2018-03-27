#ifndef HKEYSETTINGS_H
#define HKEYSETTINGS_H

#include <QObject>
#include <QList>

#include "xmlsettings.h"

namespace HKeySystem
{
enum configstate
{
    cfgHK0Alias,
    cfgHK0avail,
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


class cHKeySettings : public cXMLSettings
{
    Q_OBJECT

public:
    cHKeySettings(Zera::XMLConfig::cReader *xmlread);
    ~cHKeySettings();
    QList<HKeySystem::cChannelSettings*>& getChannelSettings();


public slots:
    virtual void configXMLInfo(QString key);

private:
    QList<HKeySystem::cChannelSettings*> m_ChannelSettingsList;
};

#endif // HKEYSETTINGS_H
