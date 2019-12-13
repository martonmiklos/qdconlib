#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include <QtSerialPort/QtSerialPort>

#include <dconlib.h>
#include <serialportsettingsdialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonQueryName_clicked();

    void on_pushButtonA_toggled(bool checked);

    void on_pushButtonB_toggled(bool checked);

    void on_pushButtonC_toggled(bool checked);

    void on_pushButtonD_toggled(bool checked);

    void on_spinBoxAddress_valueChanged(int arg1);

    void on_actionSerial_port_settings_triggered();

private:
    Ui::MainWindow *ui;
    QDCON *m_dconHandler;
    QSerialPort *m_port;
    SerialPortSettingsDialog *serialDialog;
    QSettings m_settings;
};

#endif // MAINWINDOW_H
