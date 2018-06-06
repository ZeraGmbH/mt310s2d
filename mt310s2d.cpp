#include <QStateMachine>
#include <QState>
#include <QFile>
#include <QFinalState>
#include <QStringList>
#include <QDebug>
#include <QSocketNotifier>
#include <QHostAddress>
#include <xmlconfigreader.h>
#include <QCoreApplication>
#include <protonetserver.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <signal.h>

#include "mt310s2dglobal.h"
#include "mt310s2d.h"
#include "pcbserver.h"
#include "debugsettings.h"
#include "ethsettings.h"
#include "i2csettings.h"
#include "fpgasettings.h"
#include "ctrlsettings.h"
#include "sensesettings.h"
#include "samplingsettings.h"
#include "sourcesettings.h"
#include "frqinputsettings.h"
#include "scheadsettings.h"
#include "hkeysettings.h"
#include "statusinterface.h"
#include "samplinginterface.h"
#include "systeminterface.h"
#include "systeminfo.h"
#include "senseinterface.h"
#include "sourceinterface.h"
#include "frqinputinterface.h"
#include "scheadinterface.h"
#include "hkeyinterface.h"
#include "clampinterface.h"
#include "atmel.h"
#include "atmelwatcher.h"
#include "adjustment.h"
#include "rmconnection.h"

#ifdef SYSTEMD_NOTIFICATION
#include <systemd/sd-daemon.h>
#endif


cMT310S2dServer* MTServer;
int pipeFD[2];
char pipeFDBuf[2] = "I";

void SigHandler(int)
{
    if (MTServer->m_pDebugSettings->getDebugLevel() & 2) syslog(LOG_INFO,"mt interrupt received\n");
        write(pipeFD[1], pipeFDBuf, 1);
}


struct sigaction mySigAction;
// sigset_t mySigmask, origSigmask;


cATMEL* pAtmel; // we take a static object for atmel connection

cMT310S2dServer::cMT310S2dServer(QObject *parent)
    :cPCBServer(parent)
{

    m_pDebugSettings = 0;
    m_pETHSettings = 0;
    m_pI2CSettings = 0;
    m_pFPGASettings = 0;
    m_pCtrlSettings  = 0;
    m_pSenseSettings = 0;
    pAtmel = 0;
    m_pAtmelWatcher = 0;
    m_pStatusInterface = 0;
    m_pSystemInterface = 0;
    m_pSenseInterface = 0;
    m_pClampInterface = 0;
    m_pSystemInfo = 0;
    m_pAdjHandler = 0;
    m_pRMConnection = 0;

    m_pInitializationMachine = new QStateMachine(this);

    QState* stateCONF = new QState(); // we start from here
    QFinalState* stateFINISH = new QFinalState(); // and here we finish

    stateCONF->addTransition(this, SIGNAL(abortInit()),stateFINISH); // from anywhere we arrive here if some error

    QState* statexmlConfiguration = new QState(stateCONF); // we configure our server with xml file
    QState* stateprogAtmel = new QState(stateCONF); // maybe we have to update the atmel
    QState* statewait4Atmel = new QState(stateCONF); // we synchronize on atmel running
    QState* statesetupServer = new QState(stateCONF); // we setup our server now
    stateconnect2RM = new QState(stateCONF); // we connect to resource manager
    stateconnect2RMError = new QState(stateCONF);
    stateSendRMIdentandRegister = new QState(stateCONF); // we send ident. to rm and register our resources

    stateCONF->setInitialState(statexmlConfiguration);

    statexmlConfiguration->addTransition(myXMLConfigReader, SIGNAL(finishedParsingXML(bool)), stateprogAtmel);
    stateprogAtmel->addTransition(this, SIGNAL(atmelProgrammed()), statewait4Atmel);
    statewait4Atmel->addTransition(this, SIGNAL(atmelRunning()), statesetupServer);
    statesetupServer->addTransition(this, SIGNAL(serverSetup()), stateconnect2RM);

    m_pInitializationMachine->addState(stateCONF);
    m_pInitializationMachine->addState(stateFINISH);
    m_pInitializationMachine->setInitialState(stateCONF);

    QObject::connect(statexmlConfiguration, SIGNAL(entered()), this, SLOT(doConfiguration()));
    QObject::connect(stateprogAtmel, SIGNAL(entered()), this, SLOT(programAtmelFlash()));
    QObject::connect(statewait4Atmel, SIGNAL(entered()), this, SLOT(doWait4Atmel()));
    QObject::connect(statesetupServer, SIGNAL(entered()), this, SLOT(doSetupServer()));
    QObject::connect(stateconnect2RM, SIGNAL(entered()), this, SLOT(doConnect2RM()));
    QObject::connect(stateconnect2RMError, SIGNAL(entered()), this, SLOT(connect2RMError()));
    QObject::connect(stateSendRMIdentandRegister, SIGNAL(entered()), this, SLOT(doIdentAndRegister()));
    QObject::connect(stateFINISH, SIGNAL(entered()), this, SLOT(doCloseServer()));

    m_pInitializationMachine->start();
}


cMT310S2dServer::~cMT310S2dServer()
{
    if (m_pDebugSettings) delete m_pDebugSettings;
    if (m_pETHSettings) delete m_pETHSettings;
    if (m_pI2CSettings) delete m_pI2CSettings;
    if (m_pFPGASettings) delete m_pFPGASettings;
    if (m_pCtrlSettings) delete m_pCtrlSettings;
    if (m_pSenseSettings) delete m_pSenseSettings;
    if (m_pSourceSettings) delete m_pSourceSettings;
    if (m_pFRQInputSettings) delete m_pFRQInputSettings;
    if (m_pSCHeadSettings) delete m_pSCHeadSettings;
    if (pAtmel) delete pAtmel;
    if (m_pAtmelWatcher) delete m_pAtmelWatcher;
    if (m_pStatusInterface) delete m_pStatusInterface;
    if (m_pSystemInterface) delete m_pSystemInterface;
    if (m_pSenseInterface) delete m_pSenseInterface;
    if (m_pClampInterface) delete m_pClampInterface;
    if (m_pSourceInterface) delete m_pSourceInterface;
    if (m_pFRQInputInterface) delete m_pFRQInputInterface;
    if (m_pSCHeadInterface) delete m_pSCHeadInterface;
    if (m_pHKeyInterface) delete m_pHKeyInterface;
    if (m_pSystemInfo) delete m_pSystemInfo;
    if (m_pAdjHandler) delete m_pAdjHandler;
    if (m_pRMConnection) delete m_pRMConnection;
}


void cMT310S2dServer::doConfiguration()
{
    QStringList args;

    args = QCoreApplication::instance()->arguments();
    if (args.count() != 2) // we want exactly 1 parameter
    {
        m_nerror = parameterError;
        emit abortInit();
    }
    else
    {
        if ( pipe(pipeFD) == -1)
        {
            m_nerror = pipeError;
            emit abortInit();
        }
        else
        {
            fcntl( pipeFD[1], F_SETFL, O_NONBLOCK);
            fcntl( pipeFD[0], F_SETFL, O_NONBLOCK);
            m_pNotifier = new QSocketNotifier(pipeFD[0], QSocketNotifier::Read, this);
            connect(m_pNotifier, SIGNAL(activated(int)), this, SLOT(MTIntHandler(int)));

            if (myXMLConfigReader->loadSchema(defaultXSDFile))
            {

                // we want to initialize all settings first
                m_pDebugSettings = new cDebugSettings(myXMLConfigReader);
                connect(myXMLConfigReader,SIGNAL(valueChanged(const QString&)),m_pDebugSettings,SLOT(configXMLInfo(const QString&)));
                m_pETHSettings = new cETHSettings(myXMLConfigReader);
                connect(myXMLConfigReader,SIGNAL(valueChanged(const QString&)),m_pETHSettings,SLOT(configXMLInfo(const QString&)));
                m_pI2CSettings = new cI2CSettings(myXMLConfigReader);
                connect(myXMLConfigReader,SIGNAL(valueChanged(const QString&)),m_pI2CSettings,SLOT(configXMLInfo(const QString&)));
                m_pFPGASettings = new cFPGASettings(myXMLConfigReader);
                connect(myXMLConfigReader,SIGNAL(valueChanged(const QString&)),m_pFPGASettings,SLOT(configXMLInfo(const QString&)));
                m_pCtrlSettings = new cCtrlSettings(myXMLConfigReader);
                connect(myXMLConfigReader,SIGNAL(valueChanged(const QString&)),m_pCtrlSettings,SLOT(configXMLInfo(const QString&)));
                m_pSenseSettings = new cSenseSettings(myXMLConfigReader);
                connect(myXMLConfigReader,SIGNAL(valueChanged(const QString&)),m_pSenseSettings,SLOT(configXMLInfo(const QString&)));
                m_pSourceSettings = new cSourceSettings(myXMLConfigReader);
                connect(myXMLConfigReader,SIGNAL(valueChanged(const QString&)),m_pSourceSettings,SLOT(configXMLInfo(const QString&)));
                m_pSamplingSettings = new cSamplingSettings(myXMLConfigReader);
                connect(myXMLConfigReader,SIGNAL(valueChanged(const QString&)),m_pSamplingSettings,SLOT(configXMLInfo(const QString&)));
                m_pFRQInputSettings = new cFRQInputSettings(myXMLConfigReader);
                connect(myXMLConfigReader,SIGNAL(valueChanged(const QString&)),m_pFRQInputSettings,SLOT(configXMLInfo(const QString&)));
                m_pSCHeadSettings = new cSCHeadSettings(myXMLConfigReader);
                connect(myXMLConfigReader,SIGNAL(valueChanged(const QString&)),m_pSCHeadSettings,SLOT(configXMLInfo(const QString&)));
                m_pHKeySettings = new cHKeySettings(myXMLConfigReader);
                connect(myXMLConfigReader,SIGNAL(valueChanged(const QString&)),m_pHKeySettings,SLOT(configXMLInfo(const QString&)));


                QString s = args.at(1);
                qDebug() << s;

                if (myXMLConfigReader->loadXML(s)) // the first parameter should be the filename
                {
                    // xmlfile ok -> nothing to do .. the configreader will emit all configuration
                    // signals and after this the finishedparsingXML signal
                }
                else
                {
                    m_nerror = xmlfileError;
                    emit abortInit();
                }
            }
            else
            {
                m_nerror = xsdfileError;
                emit abortInit();
            }
        }
    }
}


void cMT310S2dServer::programAtmelFlash()
{
    pAtmel = new cATMEL(m_pI2CSettings->getDeviceNode(), m_pI2CSettings->getI2CAdress(i2cSettings::atmel), m_pDebugSettings->getDebugLevel());

    QFile atmelFile(atmelFlashfilePath);
    if (atmelFile.exists())
    {
        int fd;
        QString devNode;

        m_nerror = atmelProgError; // preset error

        devNode = m_pFPGASettings->getDeviceNode();
        syslog(LOG_INFO,"Starting programming atmel flash\n");

        if ( (fd = open(devNode.toLatin1().data(),O_RDWR)) < 0 )
        {
            syslog(LOG_ERR,"error opening fpga device: %s\n",devNode.toLatin1().data());
            emit abortInit();
        }
        else
        {
            ulong pcbTestReg;
            int r;
            if ( (r = lseek(fd,0xffc,0)) < 0 )
            {
                syslog(LOG_ERR,"error positioning fpga device: %s\n", devNode.toLatin1().data());
                syslog(LOG_ERR,"Programming atmel failed\n");
                close(fd);
                emit abortInit();
            }

            r = read(fd,(char*) &pcbTestReg,4);
            syslog(LOG_ERR,"reading fpga adr 0xffc =  %x\n", (unsigned int) pcbTestReg);
            if (r < 0 )
            {
                syslog(LOG_ERR,"error reading fpga device: %s\n", devNode.toLatin1().data());
                syslog(LOG_ERR,"Programming atmel failed\n");
                emit abortInit();
            }

            pcbTestReg |=  1 << (atmelResetBit-1); // set bit for atmel reset
            syslog(LOG_INFO,"writing fpga adr 0xffc =  %x\n", (unsigned int) pcbTestReg);
            r = write(fd, (char*) &pcbTestReg,4);

            if (r < 0 )
            {
                syslog(LOG_ERR,"error writing fpga device: %s\n", devNode.toLatin1().data());
                syslog(LOG_ERR,"Programming atmel failed\n");
                emit abortInit();
            }

            usleep(100); // give atmel some time for reset

            pcbTestReg &=  ~(1 << (atmelResetBit-1)); // reset bit for atmel reset
            syslog(LOG_INFO,"writing fpga adr 0xffc =  %x\n", (unsigned int) pcbTestReg);
            r = write(fd, (char*) &pcbTestReg,4);
            close(fd);

            if (r < 0 )
            {
                syslog(LOG_ERR,"error writing fpga device: %s\n", devNode.toLatin1().data());
                syslog(LOG_ERR,"Programming atmel failed\n");
                emit abortInit();
            }

            // atmel is reset
            usleep(100000); // now we wait for 100ms so bootloader is running definitely

            // and start writing flash
            cIntelHexFileIO IntelHexData;
            if (IntelHexData.ReadHexFile(atmelFlashfilePath))
            {
               if (pAtmel->loadFlash(IntelHexData) == cmddone)
               {
                   syslog(LOG_INFO,"Programming atmel passed\n");

                   // we must restart atmel now
                   if (pAtmel->startProgram() == cmddone)
                   {
                       syslog(LOG_ERR,"Restart atmel after programming done\n");
                       // once the job is done, we remove the file
                       atmelFile.remove();

                       emit atmelProgrammed();
                   }
                   else
                   {
                       syslog(LOG_ERR,"Restart atmel after programming failed\n");
                       emit abortInit();
                   }
               }
               else
               {
                   syslog(LOG_ERR,"error writing atmel flash\n");
                   syslog(LOG_ERR,"Programming atmel failed\n");
                   emit abortInit();
               }
            }
            else
            {
                syslog(LOG_ERR,"error reading hex file\n");
                syslog(LOG_ERR,"Programming atmel failed\n");
                emit abortInit();
            }
        }
    }

    else
        emit atmelProgrammed();
}


void cMT310S2dServer::doWait4Atmel()
{
    m_pAtmelWatcher = new cAtmelWatcher(m_pDebugSettings->getDebugLevel(), m_pCtrlSettings->getDeviceNode(), 10000, 100);

    m_nerror = atmelError; // we preset error
    connect(m_pAtmelWatcher,SIGNAL(timeout()),this,SIGNAL(abortInit()));
    connect(m_pAtmelWatcher,SIGNAL(running()),this,SIGNAL(atmelRunning()));
    m_pAtmelWatcher->start();
}


void cMT310S2dServer::doSetupServer()
{
    m_sCtrlDeviceNode = m_pCtrlSettings->getDeviceNode(); // we try to open the ctrl device
    m_sMessageDeviceNode = m_pFPGASettings->getDeviceNode();

    MTServer = this;

    if (CtrlDevOpen() < 0)
    {
        m_nerror = ctrlDeviceError; // and finish if not possible
        emit abortInit();
    }
    else
        if (MessageDevOpen() < 0)
        {
            m_nerror = fpgaDeviceError;
            emit abortInit();
        }
        else
        {
            pAtmel->setPLLChannel(1); // default channel m0 for pll control
            m_pSystemInfo = new cSystemInfo();
            m_pAdjHandler = new cAdjustment(this);

            //m_pSystemInfo, m_pI2CSettings->getDeviceNode(), m_pDebugSettings->getDebugLevel(), m_pI2CSettings->getI2CAdress(i2cSettings::flash)

            cPCBServer::setupServer(); // here our scpi interface gets instanciated, we need this for further steps

            scpiConnectionList.append(this); // the server itself has some commands
            scpiConnectionList.append(m_pStatusInterface = new cStatusInterface(this));
            scpiConnectionList.append(m_pSystemInterface = new cSystemInterface(this));
            scpiConnectionList.append(m_pSenseInterface = new cSenseInterface(this));
            scpiConnectionList.append(m_pSamplingInterface = new cSamplingInterface(this));
            scpiConnectionList.append(m_pSourceInterface = new cSourceInterface(this));
            scpiConnectionList.append(m_pFRQInputInterface = new cFRQInputInterface(this));
            scpiConnectionList.append(m_pSCHeadInterface = new cSCHeadInterface(this));
            scpiConnectionList.append(m_pHKeyInterface = new cHKeyInterface(this));
            scpiConnectionList.append(m_pClampInterface = new cClampInterface(this, pAtmel));

            resourceList.append(m_pSenseInterface); // all our resources
            resourceList.append(m_pSamplingInterface);
            resourceList.append(m_pSourceInterface);
            resourceList.append(m_pFRQInputInterface);
            resourceList.append(m_pSCHeadInterface);
            resourceList.append(m_pHKeyInterface);


            m_pAdjHandler->addAdjFlashObject(m_pSenseInterface);
            m_pSenseInterface->importAdjFlash(); // we read adjustmentdata at least once

            initSCPIConnections();

            myServer->startServer(m_pETHSettings->getPort(protobufserver)); // and can start the server now
            m_pSCPIServer->listen(QHostAddress::AnyIPv4, m_pETHSettings->getPort(scpiserver));

            mySigAction.sa_handler = &SigHandler; // signal handler einrichten
            sigemptyset(&mySigAction.sa_mask);
            mySigAction. sa_flags = SA_RESTART;
            mySigAction.sa_restorer = NULL;
            sigaction(SIGIO, &mySigAction, NULL); // handler für sigio definieren

            SetFASync();
            enableClampInterrupt();

            // our resource mananager connection must be opened after configuration is done
            m_pRMConnection = new cRMConnection(m_pETHSettings->getRMIPadr(), m_pETHSettings->getPort(resourcemanager), m_pDebugSettings->getDebugLevel());
            //connect(m_pRMConnection, SIGNAL(connectionRMError()), this, SIGNAL(abortInit()));
            // so we must complete our state machine here
            m_nRetryRMConnect = 100;
            m_retryTimer.setSingleShot(true);
            connect(&m_retryTimer, SIGNAL(timeout()), this, SIGNAL(serverSetup()));

            stateconnect2RM->addTransition(m_pRMConnection, SIGNAL(connected()), stateSendRMIdentandRegister);
            stateconnect2RM->addTransition(m_pRMConnection, SIGNAL(connectionRMError()), stateconnect2RMError);
            stateconnect2RMError->addTransition(this, SIGNAL(serverSetup()), stateconnect2RM);

            emit serverSetup(); // so we enter state machine's next state
        }
}


void cMT310S2dServer::doCloseServer()
{
    QCoreApplication::instance()->exit(m_nerror);
}


void cMT310S2dServer::doConnect2RM()
{
    m_nerror = rmConnectionError; // preset error condition
    m_pRMConnection->connect2RM();
}


void cMT310S2dServer::connect2RMError()
{
    m_nRetryRMConnect--;
    if (m_nRetryRMConnect == 0)
        emit abortInit();
    else
        m_retryTimer.start(200);
}


void cMT310S2dServer::doIdentAndRegister()
{
    m_pRMConnection->SendIdent(getName());

    for (int i = 0; i < resourceList.count(); i++)
    {
        cResource *res = resourceList.at(i);
        connect(m_pRMConnection, SIGNAL(rmAck(quint32)), res, SLOT(resourceManagerAck(quint32)) );
        res->registerResource(m_pRMConnection, m_pETHSettings->getPort(protobufserver));
    }
#ifdef SYSTEMD_NOTIFICATION
    sd_notify(0, "READY=1");
#endif
}


int cMT310S2dServer::CtrlDevOpen()
{
    if ( (DevFileDescriptorCtrl = open(m_sCtrlDeviceNode.toLatin1().data(), O_RDWR)) < 0 )
    {
        if (m_pDebugSettings->getDebugLevel() & 1)  syslog(LOG_ERR,"error opening ctrl device: %s\n",m_pCtrlSettings->getDeviceNode().toLatin1().data());
    }
    return DevFileDescriptorCtrl;
}


int cMT310S2dServer::MessageDevOpen()
{
    if ( (DevFileDescriptorMsg = open(m_sMessageDeviceNode.toLatin1().data(), O_RDWR)) < 0 )
    {
        if (m_pDebugSettings->getDebugLevel() & 1)  syslog(LOG_ERR,"error opening ctrl device: %s\n",m_pFPGASettings->getDeviceNode().toLatin1().data());
    }
    return DevFileDescriptorMsg;
}


void cMT310S2dServer::SetFASync()
{
    fcntl(DevFileDescriptorMsg, F_SETOWN, getpid()); // wir sind "besitzer" des device
    int oflags = fcntl(DevFileDescriptorMsg, F_GETFL);
    fcntl(DevFileDescriptorMsg, F_SETFL, oflags | FASYNC); // async. benachrichtung (sigio) einschalten

    fcntl(DevFileDescriptorCtrl, F_SETOWN, getpid());
    oflags = fcntl(DevFileDescriptorCtrl, F_GETFL);
    fcntl(DevFileDescriptorCtrl, F_SETFL, oflags | FASYNC);
}


void cMT310S2dServer::enableClampInterrupt()
{
    quint16 imask;

    pAtmel->readIntMask(imask);
    pAtmel->writeIntMask(imask | (1 << clampstatusInterrupt));
}


void cMT310S2dServer::MTIntHandler(int)
{// handles clamp interrupt sent by the controler

    char buf[2];
    quint16 stat;

    read(pipeFD[0], buf, 1); // first we read the pipe

    pAtmel->readCriticalStatus(stat);
    if ((stat & (1 << clampstatusInterrupt)) > 0)
    {
        m_pClampInterface->actualizeClampStatus();
        pAtmel->resetCriticalStatus(stat & (1 << clampstatusInterrupt));
    }

    // here we must add the handling for message interrupts sent by fpga device
    if (false)
    {
        QString message;
        message = QString("Error");
        qFatal(message.toLatin1());
        qWarning(message.toLatin1());
    }
}


