#ifndef DLLSERIALPORT_H
#define DLLSERIALPORT_H

#include "DLLSerialPort_global.h"
#include "engineclass.h"
#include <QObject>
#include <QDebug>

class DLLSERIALPORT_EXPORT DLLSerialPort : public QObject
{
    Q_OBJECT
public:
    DLLSerialPort(QObject * parent = nullptr);
    ~DLLSerialPort();

signals:
    void sendSignalToEngine();
    void sendSignalToExe(QString);

public slots:
    void receiveSignalFromExe();
    void receiveSignalFromEngine(QString);

private:
    EngineClass *engineClass;

};

#endif // DLLSERIALPORT_H
