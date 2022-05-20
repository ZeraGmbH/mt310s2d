#ifndef EMOBINTERFACETEST_H
#define EMOBINTERFACETEST_H

#include <QObject>

class EmobInterfaceTest : public QObject
{
    Q_OBJECT
private slots:
    void init();

    void initialWorkingSCPIStack();
};

#endif // EMOBINTERFACETEST_H
