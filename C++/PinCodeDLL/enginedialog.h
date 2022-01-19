#ifndef ENGINEDIALOG_H
#define ENGINEDIALOG_H

#include <QDialog>
#include <QVector>
#include <QDebug>
#include <QObject>
#include <QPushButton>
#include <QTimer>
#include <QTime>

namespace Ui {
class EngineDialog;
}

class EngineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EngineDialog(QWidget *parent = nullptr);
    ~EngineDialog();
    void handleButton();

private slots:
    void on_B1_clicked();
    void on_B2_clicked();
    void on_B3_clicked();
    void on_B4_clicked();
    void on_B5_clicked();
    void on_B6_clicked();
    void on_B7_clicked();
    void on_B8_clicked();
    void on_B9_clicked();
    void on_B0_clicked();
    void on_pyyhi_clicked();
    void on_Poista_clicked();
    void on_LogIn_clicked();
    void on_BSulje_clicked();
    void on_EngineDialog_rejected();
    void timeri();

public slots:
    void getSignalFromInterface();

signals:
    void sendSignalToInterface(QString);

private:
    Ui::EngineDialog *ui;
    QVector<QPushButton *> napit;
    QString tallennus;
    QString a, b = "";
    QTimer *ptimer;

};

#endif // ENGINEDIALOG_H
