#ifndef I2CMUXER_H
#define I2CMUXER_H

#include <QString>
#include <QSharedPointer>

class I2cMuxer
{
public:
    typedef QSharedPointer<I2cMuxer> Ptr;
    I2cMuxer(QString deviceNode, ushort i2cMuxAdress, uchar muxCode);
    void doMux();
private:
    QString m_deviceNode;
    ushort m_i2cMuxAdress;
    uchar m_muxCode;
};

#endif // I2CMUXER_H
