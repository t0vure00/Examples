/*
This interface and engine are for the processing of the card PIN number.

*/

#include "pincodedll.h"

PinCodeDLL::PinCodeDLL(QObject *parent):QObject (parent)
{
    qDebug() << "Ollaan Pin DLL interfacen constructorissa";

    pEngineDialog = new EngineDialog;

    connect(pEngineDialog,SIGNAL(sendSignalToInterface(QString)),
            this,SLOT(getSignalFromEngine(QString)));
}

PinCodeDLL::~PinCodeDLL()
{
    delete pEngineDialog;
    pEngineDialog = nullptr;
}

void PinCodeDLL::getSignalFromExe()
{
    qDebug() << "Ollaan Pin DLL interfacessa saatu signaali Exeltä, mennään engineen";
    pEngineDialog->show();
}

void PinCodeDLL::getSignalFromEngine(QString a)
{
    qDebug() << "Ollaan Pin DLL enginessä saatu signaali ja pin: " << a << " , lähdetään Exeen";
    emit sendSignalToExe(a);
    pEngineDialog->close();
}
