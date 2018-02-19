// header datei com5003justdata.h

#ifndef DIRECTJUSTDATA_H
#define DIRECTJUSTDATA_H

#include <QObject>
#include "scpiconnection.h"

enum DirectJustCommands
{
    DirectJustGain,
    DirectJustPhase,
    DirectJustOffset,
    DirectJustStatus,
    DirectJustCompute
};


const int GainCorrOrder = 3; // ax^3 + bx^2 + cx + d
const int PhaseCorrOrder  = 3;
const int OffsetCorrOrder = 3;


class QDataStream;
class cJustData;


class cDirectJustData: public cSCPIConnection  // alle korrekturdaten für einen bereich + status
{
    Q_OBJECT

public:
    cDirectJustData(cSCPI* scpiinterface);
    ~cDirectJustData();
    virtual void initSCPIConnection(QString leadingNodes);

    cJustData* m_pGainCorrection;
    cJustData* m_pPhaseCorrection; 
    cJustData* m_pOffsetCorrection;
    
    void Serialize(QDataStream&); // zum schreiben aller justagedaten in flashspeicher
    void Deserialize(QDataStream&); // zum lesen aller justagedaten aus flashspeicher
    quint8 getAdjustmentStatus();
    void initJustData();

protected slots:
    virtual void executeCommand(int cmdCode, cProtonetCommand* protoCmd);

protected:
    QString mReadGainCorrection(QString&sInput);
    QString mReadPhaseCorrection(QString&sInput);
    QString mReadOffsetCorrection(QString&sInput);
    QString m_ReadStatus(QString& sInput);
    QString m_ComputeJustData(QString& sInput);

    virtual double getGainCorrection(double par);
    virtual double getPhaseCorrection(double par);
    virtual double getOffsetCorrection(double par);
};


#endif

