#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_port = new QSerialPort("/dev/ttyS0");
    if (!m_port->open(QSerialPort::ReadWrite)) {
        m_port = NULL;
        qWarning() << "Unable to open";
    }

    m_dconHandler = new QDCON(10, m_port, false);
    connect(m_dconHandler, SIGNAL(nameReceieved(QString)), ui->labelReceievedName, SLOT(setText(QString)));
    connect(m_dconHandler, SIGNAL(dataSent(QString)), ui->textEditSent, SLOT(append(QString)));
    connect(m_dconHandler, SIGNAL(dataReceieved(QString)), ui->textEditReceieved, SLOT(append(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonQueryName_clicked()
{
    m_dconHandler->getName();
}

void MainWindow::on_pushButtonA_toggled(bool checked)
{
   m_dconHandler->setDO(0, checked);
}

void MainWindow::on_pushButtonB_toggled(bool checked)
{
    m_dconHandler->setDO(1, checked);
}

void MainWindow::on_pushButtonC_toggled(bool checked)
{
    m_dconHandler->setDO(2, checked);
}

void MainWindow::on_pushButtonD_toggled(bool checked)
{
    m_dconHandler->setDO(3, checked);
}

void MainWindow::on_spinBoxAddress_valueChanged(int arg1)
{
    m_dconHandler->setAddress(arg1);
}
