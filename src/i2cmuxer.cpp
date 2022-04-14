#include "i2cmuxer.h"
#include <i2cutils.h>

I2cMuxer::I2cMuxer(QString deviceNode, ushort i2cMuxAdress, uchar muxCode) :
    m_deviceNode(deviceNode),
    m_i2cMuxAdress(i2cMuxAdress),
    m_muxCode(muxCode)
{
}

void I2cMuxer::doMux()
{
     // 1 adr byte, 1 byte data = mux code
    struct i2c_msg i2cMsgs;
    i2cMsgs.addr = m_i2cMuxAdress;
    i2cMsgs.flags = 0;
    i2cMsgs.len = 1;
    i2cMsgs.buf = &m_muxCode;

    struct i2c_rdwr_ioctl_data i2cRdWrData;
    i2cRdWrData.msgs = &i2cMsgs;
    i2cRdWrData.nmsgs = 1;
    I2CTransfer(m_deviceNode, m_i2cMuxAdress, &i2cRdWrData);
}
