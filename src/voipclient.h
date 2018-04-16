#ifndef VOIPCLIENT_H
#define VOIPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAudioInput>
#include <QAudioOutput>


class VoipClient : public QObject
{
    Q_OBJECT
public:
    explicit VoipClient(const QString& serverIP, const short& portNumber, const QAudioFormat& format, QObject *parent = nullptr);

signals:

public slots:
    void onConnected();
    void onReadyRead();
    void handleInputStateChange(QAudio::State state);

private:
    char* buffer;

    QTcpSocket* m_socket;
    QString m_serverIp;
    ushort m_portNumber;

    QAudioFormat m_format;
    QAudioInput* m_audioInput;
    QAudioOutput* m_audioOutput;
};

#endif // VOIPCLIENT_H
