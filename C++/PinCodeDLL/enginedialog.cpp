#include "enginedialog.h"
#include "ui_enginedialog.h"

EngineDialog::EngineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EngineDialog)
{
    ui->setupUi(this);

    ptimer = new QTimer;
    connect(ptimer,SIGNAL(timeout()),
            this,SLOT(timeri()));

    qDebug() << "Ollaan Pin DLL enginen constructorissa nyttt";

    this->setWindowTitle("PIN-ikkuna");

    ui->LogIn->setStyleSheet("background-color:green");
    ui->pyyhi->setStyleSheet("background-color:yellow");
    ui->Poista->setStyleSheet("background-color:red");

    napit.append(ui->B1);
    napit.append(ui->B2);
    napit.append(ui->B3);
    napit.append(ui->B4);
    napit.append(ui->B5);
    napit.append(ui->B6);
    napit.append(ui->B7);
    napit.append(ui->B8);
    napit.append(ui->B9);
    napit.append(ui->B0);

    this->setWindowFlags( (Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint) & ~Qt::WindowCloseButtonHint );

    ptimer->start(10000);
}



EngineDialog::~EngineDialog()
{
    qDebug() << "Ollaan Pin DLL enginen destructorissa";
    a = "";
    delete ui;
}

void EngineDialog::handleButton()
{
    qDebug() << "Ollaan handeButtonissa";
    ptimer->stop();
    QObject * sender = QObject::sender();//Tehdään QObject sender pointteri joka palauttaa pointterin sille objectille joka lähetti signaalin
    qDebug() << sender->objectName() << "kaytetty painike";//pointterilla osoitetaan objectNameen, mikä on painikkeen nimi joka lähetti signaalin/jota painettiin
    QString ruutu = sender->objectName();//-"- nimi annetaan QString tyyppiselle muuttujalle arvoksi

    foreach(QPushButton * ptr, napit){
        if (ptr->objectName() == ruutu){
            if(a.size() < 4){
                qDebug() << b;
                b = tallennus;
                a.append(b);
                ui->lineEdit->setText(a);
            }
        }
    }
    ptimer->start(10000);
}

void EngineDialog::timeri()
{
    qDebug() << "PinCode DLL meni 10 sekuntia ja yhteys suljetaan";
    ptimer->stop();
    this->close();
}


void EngineDialog::on_B1_clicked()
{
    qDebug() << "Ollaan Pin DLL enginessä Button 1 painettu";
    tallennus = ui->B1->text();
    handleButton();
}

void EngineDialog::on_B2_clicked()
{
    qDebug() << "Ollaan Pin DLL enginessä Button 2 painettu";
    tallennus = ui->B2->text();
    handleButton();
}

void EngineDialog::on_B3_clicked()
{
    qDebug() << "Ollaan Pin DLL enginessä Button 3 painettu";
    tallennus = ui->B3->text();
    handleButton();
}

void EngineDialog::on_B4_clicked()
{
    qDebug() << "Ollaan Pin DLL enginessä Button 4 painettu";
    tallennus = ui->B4->text();
    handleButton();
}

void EngineDialog::on_B5_clicked()
{
    qDebug() << "Ollaan Pin DLL enginessä Button 5 painettu";
    tallennus = ui->B5->text();
    handleButton();
}

void EngineDialog::on_B6_clicked()
{
    qDebug() << "Ollaan Pin DLL enginessä Button 6 painettu";
    tallennus = ui->B6->text();
    handleButton();
}

void EngineDialog::on_B7_clicked()
{
    qDebug() << "Ollaan Pin DLL enginessä Button 7 painettu";
    tallennus = ui->B7->text();
    handleButton();
}

void EngineDialog::on_B8_clicked()
{
    qDebug() << "Ollaan Pin DLL enginessä Button 8 painettu";
    tallennus = ui->B8->text();
    handleButton();
}

void EngineDialog::on_B9_clicked()
{
    qDebug() << "Ollaan Pin DLL enginessä Button 9 painettu";
    tallennus = ui->B9->text();
    handleButton();
}

void EngineDialog::on_B0_clicked()
{
    qDebug() << "Ollaan Pin DLL enginessä Button 0 painettu";
    tallennus = ui->B0->text();
    handleButton();
}

void EngineDialog::on_pyyhi_clicked()
{
    qDebug() << "Ollaan Pin DLL enginessä Button pyyhi painettu";
    QString muuttuja;
    muuttuja = ui->lineEdit->text();
    qDebug() << muuttuja;
    muuttuja.resize(muuttuja.size()-1);
    qDebug() << muuttuja;
    a = muuttuja;
    ui->lineEdit->setText(muuttuja);
}

void EngineDialog::on_Poista_clicked()
{
    qDebug() << "Ollaan Pin DLL enginessä Button Poista painettu";
    ui->lineEdit->setText("");
    a = "";
}

void EngineDialog::on_LogIn_clicked()
{
    qDebug() << "Ollaan Pin DLL enginessä Button Log In painettu";
    //tässä lähetettään interfaceen PINNI
    ui->lineEdit->setText("");
    emit sendSignalToInterface(a);
    a = "";
}

void EngineDialog::getSignalFromInterface()
{
    qDebug() << "Ollaan Pin DLL enginessä saatu signaali interfacesta";
}

void EngineDialog::on_BSulje_clicked()
{
    emit sendSignalToInterface("");
    this->close();
}

void EngineDialog::on_EngineDialog_rejected()
{
    emit sendSignalToInterface("");
    qDebug() << "Rejected";
}
