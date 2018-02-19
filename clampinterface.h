#ifndef CLAMPINTERFACE
#define CLAMPINTERFACE

#include <QHash>

// here we hold the clamps that are hotplugged to the system

class cMT310S2dServer;
class cATMEL;
class cClamp;

class cClampInterface
{

public:
    cClampInterface(cMT310S2dServer *server, cATMEL* controler);
    void actualizeClampStatus();

private:
    cMT310S2dServer *m_pMyServer;
    cATMEL *m_pControler;

    quint16 m_nClampStatus;
    QHash<int, cClamp*> clampHash;
};

#endif // CLAMPINTERFACE

