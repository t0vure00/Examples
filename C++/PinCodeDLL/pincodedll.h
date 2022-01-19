#ifndef PINCODEDLL_H
#define PINCODEDLL_H

#include "PinCodeDLL_global.h"
#include "enginedialog.h"
#include <QObject>
#include <QtDebug>

class PINCODEDLL_EXPORT PinCodeDLL : public QObject
{
    Q_OBJECT
public:
    PinCodeDLL(QObject *parent);
    ~PinCodeDLL();

signals:
    void sendSignalToEngine();
    void sendSignalToExe(QString);

public slots:
    void getSignalFromExe();
    void getSignalFromEngine(QString);

private:
    EngineDialog *pEngineDialog;

};

#endif // PINCODEDLL_H
