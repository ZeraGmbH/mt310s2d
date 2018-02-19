// header datei mt310touchd.h
// globale definitionen

#ifndef MT310S2D_H
#define MT310S2D_H

#include <QTimer>

#include "pcbserver.h"

class QStateMachine;
class QState;
class cStatusInterface;
class cSystemInterface;
class cSenseInterface;
class cSamplingInterface;
class cSourceInterface;
class cFRQInputInterface;
class cSCHeadInterface;
class cSystemInfo;
class cAdjustment;
class cAtmelWatcher;
class cRMConnection;
class QSocketNotifier;
class cClampInterface;

class cMT310S2dServer: public cPCBServer
{
    Q_OBJECT

public:
    explicit cMT310S2dServer(QObject* parent=0);
    ~cMT310S2dServer();

    cStatusInterface* m_pStatusInterface;
    cSystemInterface* m_pSystemInterface;
    cSenseInterface* m_pSenseInterface;
    cSamplingInterface* m_pSamplingInterface;
    cSourceInterface* m_pSourceInterface;
    cFRQInputInterface* m_pFRQInputInterface;
    cSCHeadInterface* m_pSCHeadInterface;
    cSystemInfo* m_pSystemInfo;
    cAdjustment* m_pAdjHandler;
    cRMConnection* m_pRMConnection;
    cClampInterface* m_pClampInterface;

    int DevFileDescriptor; // kerneltreiber wird nur 1x geöffnet und dann gehalten

signals:
    void abortInit();
    void confStarting();
    void confFinished();
    void atmelProgrammed();
    void atmelRunning();
    void serverSetup();

private:
    QStateMachine* m_pInitializationMachine;
    QState* stateconnect2RM;
    QState* stateconnect2RMError;
    QState* stateSendRMIdentandRegister;
    cAtmelWatcher* m_pAtmelWatcher;
    quint8 m_nerror;
    int m_nRetryRMConnect;
    QTimer m_retryTimer;
    QSocketNotifier* m_pNotifier;
    QString m_sCtrlDeviceNode;
    int CtrlDevOpen();
    void SetFASync();
    void enableClampInterrupt();

private slots:
    void MTIntHandler(int);
    void doConfiguration();
    void programAtmelFlash();
    void doWait4Atmel();
    void doSetupServer();
    void doCloseServer();
    void doConnect2RM();
    void connect2RMError();
    void doIdentAndRegister();
};


#endif
