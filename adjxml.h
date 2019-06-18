#ifndef ADJXML_H
#define ADJXML_H

#include <QString>

class QDomDocument;

class cAdjXML
{
public:
    cAdjXML(quint8 dlevel);

    virtual bool exportAdjXML(QString file);
    virtual bool importAdjXML(QString file);
    virtual bool importAdjXMLString(QString& xml);

protected:
    virtual QString exportXMLString(int indent = 1) = 0; // adjustment data xml export to string
    virtual bool importXMLDocument(QDomDocument* qdomdoc) = 0;

private:
    quint8 DebugLevel;
};

#endif // ADJXML_H
