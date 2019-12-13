#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_port = new QSerialPort();
    serialDialog = new SerialPortSettingsDialog(this);

    m_settings.beginGroup("serial");
    serialDialog->loadSettings(m_settings);
    m_settings.endGroup();

    serialDialog->setPort(m_port);
    serialDialog->applySettingsToPort(m_port);
    if (!m_port->open(QSerialPort::ReadWrite)) {
        m_port = NULL;
        qWarning() << "Unable to open";
    }

    m_dconHandler = new QDCON(1, m_port, false);
    connect(m_dconHandler, SIGNAL(nameReceieved(QString)), ui->labelReceievedName, SLOT(setText(QString)));
    connect(m_dconHandler, SIGNAL(dataSent(QString)), ui->textEditSent, SLOT(append(QString)));
    connect(m_dconHandler, SIGNAL(dataReceieved(QString)), ui->textEditReceieved, SLOT(append(QString)));
}

MainWindow::~MainWindow()
{
    serialDialog->saveSettings(m_settings);
    delete ui;
}

void MainWindow::on_pushButtonQueryName_clicked()
{
    m_dconHandler->getName();
}

void MainWindow::on_pushButtonA_toggled(bool checked)
{
   m_dconHandler->setDO(0, checked, ui->spinBoxAddress->value());
}

void MainWindow::on_pushButtonB_toggled(bool checked)
{
    m_dconHandler->setDO(1, checked, ui->spinBoxAddress->value());
}

void MainWindow::on_pushButtonC_toggled(bool checked)
{
    m_dconHandler->setDO(2, checked, ui->spinBoxAddress->value());
}

void MainWindow::on_pushButtonD_toggled(bool checked)
{
    m_dconHandler->setDO(3, checked, ui->spinBoxAddress->value());
}

void MainWindow::on_spinBoxAddress_valueChanged(int arg1)
{
    m_dconHandler->setAddress(arg1);
}

void MainWindow::on_actionSerial_port_settings_triggered()
{
    serialDialog->show();
}
