#ifndef VOIPSERVER_H
#define VOIPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QAudioInput>
#include <QAudioOutput>
#include <QTcpSocket>
#include <QAudioFormat>
#include <QBuffer>

class VoipServer : public QObject
{
    Q_OBJECT
public:
    explicit VoipServer(const ushort port, const QAudioFormat &format, QObject *parent = nullptr);

signals:

public slots:
    void onNewConnection();
    void onReadyRead();
    void handleOutputStateChange(QAudio::State);

private:
    QTcpServer* m_server;
    QTcpSocket* m_client;

    QAudioInput* m_audioInput;
    QAudioOutput* m_audioOutput;
};

#endif // VOIPSERVER_H
