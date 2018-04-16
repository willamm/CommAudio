#include "voipclient.h"

VoipClient::VoipClient(const QString &serverIP, const short &portNumber, const QAudioFormat &format, QObject *parent)
    : QObject(parent),
      m_audioInput(new QAudioInput(QAudioDeviceInfo::defaultInputDevice(), format, this)),
      m_audioOutput(new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), format, this)),
      m_socket(new QTcpSocket(this)),
      m_serverIp(serverIP),
      m_portNumber(portNumber)
{
    m_socket->setReadBufferSize(44100);
    connect(m_socket, &QTcpSocket::connected, this, &VoipClient::onConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &VoipClient::onReadyRead);
    connect(m_socket, &QTcpSocket::bytesWritten, this, [this](qint64 bytes) {qDebug() << "client bytes written: " << bytes;});
    connect(m_audioInput, &QAudioInput::stateChanged, this, &VoipClient::handleInputStateChange);
    connect(m_audioOutput, &QAudioOutput::stateChanged, this, [this](QAudio::State state){
        qDebug() << "client output state: " << state;
    });

    m_audioInput->setBufferSize(44100);
    m_audioOutput->setBufferSize(44100);

    m_socket->connectToHost(m_serverIp, m_portNumber);
}

void VoipClient::onConnected()
{
    qDebug() << "Connected";
    m_audioInput->start(m_socket);
}

void VoipClient::onReadyRead()
{
    qDebug() << "Ready read";
    // write data received to the audio output device
    m_audioOutput->start(m_socket);
}

void VoipClient::handleInputStateChange(QAudio::State state)
{
   qDebug() << "client input state: " << state;
}
