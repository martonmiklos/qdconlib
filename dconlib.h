#ifndef DCONLIB_H
#define DCONLIB_H

#include "dconlib_global.h"

#include <QtSerialPort/QtSerialPort>
#include <QTimer>

class QDCON;

class QDCONPacket
{
public:
    virtual ~QDCONPacket() {}
    enum Command {
        CommandGetName,
        CommandSetDO,
    };

    virtual QByteArray serialize() = 0;
    virtual bool checkAnswer(QByteArray data) = 0;
    QString error() const {return m_error;}
protected:
    Command command;
    QString appendCheckSum(QString data);
    QString m_error;
};

class QDCONSetDOPacket : public QDCONPacket
{
public:
    QDCONSetDOPacket(quint8 doNumber, bool state, quint16 address, bool checkSumMode = false);
    ~QDCONSetDOPacket() {}
    QByteArray serialize();
    bool checkAnswer(QByteArray data) ;

private:
    quint8 m_doNumber;
    bool m_state;
    quint16 m_address;
    bool m_checksumMode;
};

class DCONLIBSHARED_EXPORT QDCON : public QObject
{
    Q_OBJECT
public:
    QDCON(quint16 address, QSerialPort *port = NULL, bool checksumMode = false);

    bool paused() const {return m_paused;}
    void setPaused(bool paused);

    QSerialPort* port() const {return m_port;}
    void setPort(QSerialPort* port);

    quint16 address() const {return m_address;}
    void setAddress(quint16 address) {m_address = address;}

    void setDO(quint16 dioNumber, bool state, quint16 address = 0);

    void getName() {}

    bool checksumMode() const {return m_checksumMode;}
    void setChecksumMode(bool checksumMode) {m_checksumMode = checksumMode;}

    QString error() const {return m_error;}

    /**
     * @brief If a command is sent an no valid response is receieved within the timeout interval
     * the errorHappened(); signal will be emitted.
     * @return Returns with the communication timeout in milliseconds.
     */
    int timeoutInMs() {return m_timeoutTimer->interval();}
    /**
     * @brief Sets the communication timeout in milliseconds.
     * @param The communication timeout in milliseconds.
     */
    void setTimeOutInMs(int ms) {m_timeoutTimer->setInterval(ms);}

private:
    quint16 m_address;
    QSerialPort* m_port;
    bool m_checksumMode;
    QByteArray m_receievedBuffer;

    QTimer *m_timeoutTimer;

    QString m_error;

    QDCONPacket *m_sentPacket;
    QList<QDCONPacket*> m_pendingPackets;

    bool m_paused;

    void processReceievedData();

    void sendNextPacket();

private slots:
    void readyRead();
    void clearReceieveBuffer();
    void timeout();

signals:
    void nameReceieved(QString name);
    void dataSent(QString data);
    void dataReceieved(QString data);
    void errorHappened(QString error);
};

#endif // DCONLIB_H
