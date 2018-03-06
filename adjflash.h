#ifndef ADJFLASH_H
#define ADJFLASH_H

#include <QByteArray>
#include <QString>

class QDataStream;

class cAdjFlash
{
public:
    cAdjFlash(QString devnode, quint8 dlevel, quint8 i2cadr);
    virtual bool exportAdjFlash();
    virtual bool importAdjFlash();

    virtual quint8 getAdjustmentStatus() = 0;
    quint16 getChecksum();

protected:
    quint8 m_nDebugLevel;
    quint16 m_nChecksum;
    virtual void exportAdjData(QDataStream& stream) = 0; // the derived class exports adjdata to qdatastream
    virtual bool importAdjData(QDataStream& stream) = 0; // same for import

    bool readFlash(QByteArray& ba);
    bool writeFlash(QByteArray& ba);
    virtual void setI2CMux() = 0; // default we do nothing here but if necessary it can be overwritten

private:
    QString m_sDeviceNode;
    quint8 m_nI2CAdr;

    void setAdjCountChecksum(QByteArray& ba);
};

#endif // ADJFLASH_H
