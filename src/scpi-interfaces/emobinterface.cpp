#include "emobinterface.h"

EmobInterface::EmobInterface(cI2CSettings *i2cSettings) :
    m_i2cSettings(i2cSettings)
{

}

void EmobInterface::initSCPIConnection(QString leadingNodes)
{

}

void EmobInterface::actualizeClampStatus(quint16 devConnectedMask)
{

}

void EmobInterface::executeCommand(int cmdCode, cProtonetCommand *protoCmd)
{

}
