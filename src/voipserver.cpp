#include "voipserver.h"

VoipServer::VoipServer(const ushort port, const QAudioFormat& format, QObject *parent)
    : QObject(parent),
      m_server(new QTcpServer(this)),
      m_audioInput(new QAudioInput(QAudioDeviceInfo::defaultInputDevice(), format, this)),
      m_audioOutput(new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), format, this))
{
    connect(m_server, &QTcpServer::newConnection, this, &VoipServer::onNewConnection);
    connect(m_audioOutput, &QAudioOutput::stateChanged, this, &VoipServer::handleOutputStateChange);
    connect(m_audioOutput, &QAudioOutput::notify, this, [this]() {
        qDebug() << "notify signal fired";


    });
    m_audioInput->setBufferSize(44100);
    m_audioOutput->setBufferSize(0);
    m_audioOutput->setNotifyInterval(128);
    m_server->listen(QHostAddress::Any, port);
}

void VoipServer::onNewConnection()
{
    qDebug() << "New connection";
    qDebug() << "Output buffer size: " << m_audioOutput->bufferSize();
    m_client = m_server->nextPendingConnection();
    m_client->setReadBufferSize(44100);
    connect(m_client, &QTcpSocket::disconnected, this, &QObject::deleteLater);
    connect(m_client, &QTcpSocket::readyRead, this, &VoipServer::onReadyRead);
    connect(m_client, &QTcpSocket::bytesWritten, this, [this](qint64 bytes){ qDebug() << "server bytes written: " << bytes;});
}

void VoipServer::onReadyRead()
{
    qDebug() << "server readyRead()";
    // append data to buffer?
    // TODO: fix popping noise when sound is played
    if (m_client->bytesAvailable())
    {
        //qDebug() << m_client->readAll();
        m_audioOutput->start(m_client);
    }
}

void VoipServer::handleOutputStateChange(QAudio::State state)
{
   qDebug() << "server output state: " << state;
}
