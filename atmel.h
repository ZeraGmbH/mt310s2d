#ifndef ATMEL_H
#define ATMEL_H

#include <QString>
#include <intelhexfileio.h>
#include <crcutils.h>

enum hw_cmdcode
{
    hwGetSerialNr = 0x0001,
    hwGetDevName = 0x0002,
    hwGetCtrlVersion = 0x0003,
    hwGetLCAVersion = 0x0004,
    hwGetPCBVersion = 0x0005,
    hwSetSerialNr = 0x0006,
    hwSetPCBVersion = 0x0007,
    hwStartBootloader = 0x0008,

    hwGetInfStat  = 0x0100,

    hwGetCritStat = 0x0200,
    hwResetCritStat = 0x0201,
    hwSetIntMask = 0x0202,
    hwGetIntMask = 0x0203,
    hwGetClampStatus = 0x0204,

    hwSetSeqMask = 0x1000,
    hwGetSeqMask = 0x1001,
    hwSetPLLChannel = 0x1002,
    hwGetPLLChannel = 0x1003,
    hwSetMode = 0x1004,
    hwGetMode = 0x1005,
    hwSetFrequency = 0x1006,
    hwGetFrequency = 0x1007,
    hwSetSRate = 0x1008,
    hwGetSRate = 0x1009,
    hwGetPCBTemp = 0x100A,
    hwGetFlashWriteAccess=0x100B,

    hwSetRange = 0x1100,
    hwGetRange = 0x1101,
    hwGetOVRRange = 0x1102,
    hwGetStatus = 0x1103
};


struct hw_cmd {
    hw_cmdcode cmdcode;
    quint8 device;
    quint8* par;
    quint16 plen;
    quint16 cmdlen;
    quint8* cmddata;
    qint16 RM;
};


enum bl_cmdcode
{
    blReadInfo = 0x00,
    blStartProgram = 0x01,
    blReadFlashBlock = 0x10,
    blReadEEPromBlock = 0x11,
    blReadAdressPointer = 0x12,
    blWriteFlashBlock = 0x20,
    blWriteEEPromBlock = 0x21,
    blWriteAddressPointer = 0x22
};


struct bl_cmd
{
    bl_cmdcode cmdcode;
    quint8* par;
    quint16 plen;
    quint16 cmdlen;
    quint8* cmddata;
    qint16 RM;
};


enum enConfFlags { blAutoIncr = 1, blReadCommandsAvail = 2};


struct blInfo {
    quint16 ConfigurationFlags;
    quint16 MemPageSize;
    uchar AdressPointerSize;
};


enum atmelRM
{
    cmddone,
    cmdfault,
    cmdexecfault
};


class cATMEL
{
public:
    // atmel information about i2c device node, the controlers i2c adress and debuglevel
    cATMEL(QString devnode, quint8 adr, quint8 debuglevel);
    ~cATMEL();

    atmelRM readSerialNumber(QString& answer);
    atmelRM writeSerialNumber(QString &sNumber);
    atmelRM readDeviceName(QString& answer);
    atmelRM readPCBVersion(QString& answer);
    atmelRM writePCBVersion(QString& sVersion);
    atmelRM readCTRLVersion(QString& answer);
    atmelRM readLCAVersion(QString& answer);
    atmelRM startBootLoader();
    atmelRM startProgram();
    atmelRM loadFlash(cIntelHexFileIO& ihxFIO);
    atmelRM loadEEprom(cIntelHexFileIO& ihxFIO);
    atmelRM readChannelStatus(quint8 channel, quint8& stat);
    atmelRM readCriticalStatus(quint16& stat);
    atmelRM resetCriticalStatus(quint16 stat);
    atmelRM readClampStatus(quint16& stat);

    atmelRM writeIntMask(quint16 mask);
    atmelRM readIntMask(quint16& mask);

    atmelRM readRange(quint8 channel, quint8& range);
    atmelRM setRange(quint8 channel, quint8 range);
    atmelRM getEEPROMAccessEnable(bool& enable);
    atmelRM readSamplingRange(quint8& srange);
    atmelRM setSamplingRange(quint8 );
    atmelRM setMeasMode(quint8 mmode);
    atmelRM readMeasMode(quint8& mmode);
    atmelRM setPLLChannel(quint8 chn);
    atmelRM readPLLChannel(quint8& chn);

private:
    atmelRM mGetText(hw_cmdcode hwcmd,QString& answer);
    void GenCommand(hw_cmd* hc);
    void GenBootloaderCommand(bl_cmd* blc);
    qint16 writeCommand(hw_cmd* hc); // return -1  on error else length info of answer we can get
    qint16 writeBootloaderCommand(bl_cmd* blc); // return -1  on error else length info of answer we can get
    qint16 readOutput(char *data, quint16 dlen); // return -1  on error else length info
    quint8* GenAdressPointerParameter(quint8 adresspointerSize, quint32 adr);
    atmelRM loadMemory(bl_cmdcode blwriteCmd, cIntelHexFileIO& ihxFIO);

    cMaxim1WireCRC *m_pCRCGenerator;
    QString m_sI2CDevNode;
    quint8 m_nI2CAdr;
    quint8 m_nDebugLevel;
};

#endif // ATMEL_H
