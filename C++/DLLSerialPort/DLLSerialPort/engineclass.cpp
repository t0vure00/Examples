/*
This is the engine class for the RFID reader and does the actual operations.
For example read out the Port, Manufacturer ans Serial number.
*/

#include "engineclass.h"

EngineClass::EngineClass(QObject *parent):QObject(parent)
{
    qDebug() << "Ollaan RFIDDLL enginen constructorissa";

    QTextStream out(stdout);
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    const QString blankString = "N/A";

    QString description;
    QString manufacturer;
    QString serialNumber;
    QString vendorIdentefier;
    QString productIdentifier;

    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        description = serialPortInfo.description();
        manufacturer = serialPortInfo.manufacturer();
        serialNumber = serialPortInfo.serialNumber();
        vendorIdentefier = (serialPortInfo.hasVendorIdentifier() ? QByteArray::number(serialPortInfo.vendorIdentifier(), 16): blankString);
        productIdentifier = (serialPortInfo.hasProductIdentifier() ? QByteArray::number(serialPortInfo.productIdentifier(), 16) : blankString);
        out << "\nPort: " << serialPortInfo.portName()
            << "\nLocation: " << serialPortInfo.systemLocation()
            << "\nDescription: " << (!description.isEmpty() ? description : blankString)
            << "\nManufacturer: " << (!manufacturer.isEmpty() ? manufacturer : blankString)
            << "\nSerial number: " << (!serialNumber.isEmpty() ? serialNumber : blankString)
            << "\nVendor Identifier: " << (vendorIdentefier)
            << "\nProduct Identifier: " << (productIdentifier) << "\n";
        // Finding the right port whit the info inside "", should in theory connect to all RFID olimex readers
        // provided the description is "USB-sarjalaite" and vendor identefier is 15ba"
        if(description == "USB-sarjalaite" && vendorIdentefier == "15ba"){
            portti = serialPortInfo.portName();
            qDebug() << portti;
        }else{
            qDebug() << "Ei oikea portti";
        }
    }
}

EngineClass::~EngineClass()
{
    qDebug() << "Ollaan RFIDDLL enginen destructorissa";
    SerialPort.close();
}

void EngineClass::receiveSignalFromInterfaceAndGetRightDevice()
{
    qDebug() << "Ollaan RFIDDLL enginen getRightSerialPort";
    openSerialPort(portti);
}

void EngineClass::openSerialPort(QString a)
{
    qDebug() << "Ollaan saatu oikea portti/laite";
    SerialPort.setPortName(a);
    qDebug() << SerialPort.portName();
    SerialPort.setBaudRate(9600);                   //Setting up connection info
    SerialPort.setDataBits(QSerialPort::Data8);
    SerialPort.setParity(QSerialPort::NoParity);
    SerialPort.setStopBits(QSerialPort::OneStop);
    SerialPort.setFlowControl(QSerialPort::HardwareControl);

    if(!SerialPort.open(QIODevice::ReadWrite)){
        qDebug() << "ei aukea" << Qt::endl;
    }else{
        qDebug() << "portti aukesi" << Qt::endl;
        connect(&SerialPort,SIGNAL(readyRead()),
                this,SLOT(readID()));
    }
}

void EngineClass::readID()
{
    qDebug() << "Ollaan readID";
    QByteArray readData = SerialPort.readAll();
    QString muuttuja = readData.append(SerialPort.readAll());
    qDebug() << muuttuja;
    muuttuja.remove("-");
    muuttuja.remove(">");
    muuttuja.remove("\r\n");
    if(muuttuja.size() > 10){
        qDebug() << "Liikaa charactereita, yritä uudestaan";
        emit sendSignalToInterface("Yritä uudelleen");
    }else if(muuttuja.size() == 10){
        qDebug() << "ID saatiin ja sarjaportti suljetaan";
        emit sendSignalToInterface(muuttuja);
    }else{
        qDebug() << "Liian vähän charactereita";
        emit sendSignalToInterface("Yritä uudelleen");
    }
}
