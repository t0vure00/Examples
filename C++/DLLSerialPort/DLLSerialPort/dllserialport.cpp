/*
Part of a project that's objective was to make a banking app.
This interface and engine is meant for connetcting the RFID reader 
and then reading a tags (the bank card) ID, which was unique to a customer
and delivering it forward to the program for a check.
*/

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
