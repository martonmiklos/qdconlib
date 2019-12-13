#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QtSerialPort>

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

private:
    Ui::MainWindow *ui;
    QDCON *m_dconHandler;
    QSerialPort *m_port;
};

#endif // MAINWINDOW_H
