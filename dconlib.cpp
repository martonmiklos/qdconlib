#include "dconlib.h"
#include <QString>
#include <QDebug>

QString QDCONPacket::appendCheckSum(QString data)
{
    quint8 checkSum = 0;
    for (int i = 0; i<data.length(); i++) {
        checkSum += (quint8)data.at(i).toLatin1();
    }

    return data.append(QString::number(checkSum, 16).leftJustified(2, '0').toLatin1());
}

QDCON::QDCON(quint16 address, QSerialPort *port, bool checksumMode) :
    m_address(address),
    m_port(port),
    m_checksumMode(checksumMode),
    m_sentPacket(NULL),
    m_paused(false)
{
    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setInterval(100);
    m_timeoutTimer->setSingleShot(true);
    connect(m_timeoutTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    connect(port, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void QDCON::setPaused(bool paused)
{
    m_paused = paused;
    if (!paused) {
        sendNextPacket();
    }
}

void QDCON::setPort(QSerialPort *port)
{
    m_port = port;
    connect(port, SIGNAL(readyRead()), this, SLOT(readyRead()));
}


void QDCON::setDO(quint16 doNumber, bool state, quint16 address)
{
    QDCONSetDOPacket *packet = new QDCONSetDOPacket(doNumber, state, address, m_checksumMode);
    m_pendingPackets.append(packet);
    sendNextPacket();
}


void QDCON::readyRead()
{
    if (m_sentPacket != NULL) {
        QByteArray currentData = m_port->read(m_port->bytesAvailable());
        m_receievedBuffer.append(currentData);
        emit dataReceieved(QString(currentData));

        if (m_receievedBuffer.contains('\r')) {
            if (!m_sentPacket->checkAnswer(m_receievedBuffer)) {
                emit errorHappened(m_sentPacket->error());
            }
            delete m_sentPacket;
            m_sentPacket = NULL;
        }
    }
}

void QDCON::sendNextPacket()
{
    if (m_sentPacket == NULL && !m_paused) {
        if (m_pendingPackets.size()) {
            m_sentPacket = m_pendingPackets.takeFirst();
            m_port->readAll();
            clearReceieveBuffer();
            m_port->clear();
            m_port->write(m_sentPacket->serialize());
            m_timeoutTimer->start();
        }
    }
}

void QDCON::clearReceieveBuffer()
{
    m_receievedBuffer.clear();
}

void QDCON::timeout()
{
    if (m_port != NULL && m_sentPacket != NULL) {
        m_error = tr("Command answer did not receieved within the timeout limit (%1ms)").arg(m_timeoutTimer->interval());
        clearReceieveBuffer();
        emit errorHappened(m_error);

        delete m_sentPacket;
        m_sentPacket = NULL;
    }
}


QDCONSetDOPacket::QDCONSetDOPacket(quint8 doNumber, bool state, quint16 address, bool checkSumMode) :
    m_doNumber(doNumber),
    m_state(state),
    m_address(address),
    m_checksumMode(checkSumMode)
{

}

QByteArray QDCONSetDOPacket::serialize()
{
    /*
     * #AAAcDD
     * #AABcDD
     * Syntax:
     * #AAAcDD[CHKSUM](CR)
     * # Delimiter character
     * AA Address of the module to be set (00 to FF)
     * A/B Command to set a single digital output channel of the lower/upper eight channels
     * c Specifies the digital output channel to be set (0 to 7).
     * DD   00: set the digital output channel to off.
     *      01: set the digital output channel to on.
     */

    quint8 doNumber = m_doNumber;
    QChar halfSelector = 'A';
    if (m_doNumber > 7) {
        halfSelector = 'B';
        doNumber -= 8;
    }

    QString data = QString("#%1%2%3%4")
            .arg(m_address, 2, 16, QLatin1Char('0'))
            .arg(halfSelector)
            .arg(m_doNumber, 1, 10)
            .arg(m_state?"01":"00");
    data = data.toUpper();
    if (m_checksumMode)
        data = appendCheckSum(data);
    data.append('\r');
    return data.toLatin1();
}

bool QDCONSetDOPacket::checkAnswer(QByteArray data)
{
    if (data.at(0) == '>') {
        return true;
    } else if (data.at(0) == '?') {
        m_error = QObject::tr("Invalid command");
        return false;
    } else if (data.at(0) == '!') {
        m_error = QObject::tr("Ignored command");
        return false;
    }
    return false;
}
