#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <syslog.h>

#include "adjxml.h"
#include "mt310s2dglobal.h"


cAdjXML::cAdjXML(quint8 dlevel)
    :DebugLevel(dlevel)
{
}


bool cAdjXML::exportAdjXML(QString file)
{
    QString filename = file + ".xml";

    QFile adjfile(filename);
    if ( !adjfile.open( QIODevice::WriteOnly ) )
    {
        if ((DebugLevel & 1) != 0) syslog(LOG_ERR,"justdata export, could not open xml file\n");
        return false;
    }

    QString sXML;

    sXML = exportXMLString();
    QTextStream stream( &adjfile );
    stream << sXML;
    adjfile.close();

    return true;
}


bool cAdjXML::importAdjXML(QString file)
{
    QString filename = file + ".xml";

    QFile adjfile(filename);
    if ( !adjfile.open( QIODevice::ReadOnly ) )
    {
        if ((DebugLevel & 1) != 0)
            syslog(LOG_ERR,"justdata import, could not open xml file\n");
        return false;
    }

    QDomDocument justqdom( "TheDocument" );
    if ( !justqdom.setContent( &adjfile ) )
    {
        adjfile.close();
        if ((DebugLevel & 1) != 0)
            syslog(LOG_ERR,"justdata import, format error in xml file\n");
        return false;
    }

    adjfile.close();

    return importXMLDocument(&justqdom);
}


bool cAdjXML::importAdjXMLString(QString &xml)
{
    QDomDocument justqdom( "TheDocument" );
    if ( !justqdom.setContent(xml) )
    {
        if ((DebugLevel & 1) != 0)
            syslog(LOG_ERR,"justdata import, format error in xml file\n");
        return false;
    }
    return importXMLDocument(&justqdom);
}


