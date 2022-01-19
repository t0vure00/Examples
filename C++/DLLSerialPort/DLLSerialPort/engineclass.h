#ifndef ENGINECLASS_H
#define ENGINECLASS_H

#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

class EngineClass : public QObject
{
    Q_OBJECT
public:
    EngineClass(QObject * parent = nullptr);
    ~EngineClass();
    void openSerialPort(QString);

signals:
    void sendSignalToInterface(QString);

public slots:
    void receiveSignalFromInterfaceAndGetRightDevice();
    void readID();

private:
    QSerialPort SerialPort;
    QSerialPortInfo serialPortInfos;
    QString portti;
};

#endif // ENGINECLASS_H
