#include "dllserialport.h"


DLLSerialPort::DLLSerialPort(QObject *parent):QObject(parent)
{
    qDebug() << "Ollaan RFIDDLL interfacen constructorissa";

    engineClass = new EngineClass(this);

    connect(this, SIGNAL(sendSignalToEngine()),
            engineClass,SLOT(receiveSignalFromInterfaceAndGetRightDevice()));

    connect(engineClass, SIGNAL(sendSignalToInterface(QString)),
            this,SLOT(receiveSignalFromEngine(QString)));
}

DLLSerialPort::~DLLSerialPort()
{
    qDebug() << "Ollaan RFIDDLL interfacen destructorissa";
    delete engineClass;
    engineClass = nullptr;
}

void DLLSerialPort::receiveSignalFromExe()
{
    qDebug() << "Ollaan saatu signaali Exe:ltä aloittaa";
    emit sendSignalToEngine();
}

void DLLSerialPort::receiveSignalFromEngine(QString a)
{
    qDebug() << "Ollaan saatu signaali engineltä mennä takaisin Exeen";
    emit sendSignalToExe(a);
}
