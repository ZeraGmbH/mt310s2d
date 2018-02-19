#ifndef SENSERANGE_H
#define SENSERANGE_H

#include <QObject>
#include <scpi.h>

#include "scpiconnection.h"
#include "directjustdata.h"

namespace SenseRange
{
enum Type
{
    Phys, // we distinguish between physical
    Virt  // and virtual ranges
};

enum Commands
{
    cmdType,
    cmdAlias,
    cmdAvail,
    cmdValue,
    cmdRejection,
    cmdOVRejection
};
}

class cATMEL;
class cDirectJustData;
class cSCPI;

class cSenseRange:public cSCPIConnection
{
    Q_OBJECT

public:
    cSenseRange(cSCPI* scpiinterface, QString name, QString alias, bool avail, double rValue, double rejection, double ovrejection, quint8 rselcode, quint8 rspec, cDirectJustData* justdata);
    ~cSenseRange();
    virtual void initSCPIConnection(QString leadingNodes);
    quint8 getAdjustmentStatus();

    QString& getName();
    double getUrvalue();
    quint8 getSelCode();
    cDirectJustData* getJustData();
    bool getAvail();
    void setAvail(bool b);

    void initJustData();

protected slots:
    virtual void executeCommand(int cmdCode, cProtonetCommand* protoCmd);

protected:
    cATMEL* m_pATMEL;
    QString m_sName; // the range name
    QString m_sAlias; // the range alias name
    bool m_bAvail; // range io avail or not
    double m_fRValue; // upper range value
    double m_fRejection; // 100% rejection value
    double m_fOVRejection; // overload rejection value
    quint8 m_nSelCode; // selection code
    quint8 m_nRSpec; // range spec (phys. or virt. range)

    QString m_ReadRangeType(QString& sInput);
    QString m_ReadRangeAlias(QString& sInput);
    QString m_ReadRangeAvail(QString& sInput);
    QString m_ReadRangeValue(QString& sInput);
    QString m_ReadRangeRejection(QString& sInput);
    QString m_ReadRangeOVRejection(QString& sInput);

    cDirectJustData* m_pJustdata;

};


#endif // SENSERANGE_H
