#include "voipserver.h"

VoipServer::VoipServer(const ushort port, const QAudioFormat& format, QObject *parent)
    : QObject(parent),
      m_server(new QTcpServer(this)),
      m_audioInput(new QAudioInput(format, this)),
      m_audioOutput(new QAudioOutput(format, this)),
      m_buffer(new QBuffer(this))
{
    if (!m_buffer->open(QIODevice::ReadWrite))
    {
        qDebug() << "Failed to open buffer";
        return;
    }
    m_buffer->setBuffer(&m_byteBuffer);
    connect(m_server, &QTcpServer::newConnection, this, &VoipServer::onNewConnection);
    connect(m_audioOutput, &QAudioOutput::stateChanged, this, &VoipServer::handleOutputStateChange);
    m_server->listen(QHostAddress::Any, port);
}

VoipServer::~VoipServer()
{
    m_audioInput->stop();
    m_audioOutput->stop();
    m_server->close();
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

    // Start reading from port
    m_audioOutput->start(m_client);
}

void VoipServer::onReadyRead()
{
    qDebug() << "server readyRead()";
    // append data to buffer?
    // TODO: fix popping noise when sound is played
    if (m_client->bytesAvailable())
    {
        //qDebug() << m_client->readAll();
        //m_byteBuffer.append(m_client->readAll());

    }
}

void VoipServer::handleOutputStateChange(QAudio::State state)
{
    qDebug() << "server output state: " << state;
}

void VoipServer::stopAllAudio()
{
    m_audioInput->stop();
    m_audioOutput->stop();
}
