#include "voicechatcontroller.h"
#include "ui_voicechatcontroller.h"

#define VOIP_PORT 7777

VoiceChatController::VoiceChatController(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoiceChatController),
    m_server(new QTcpServer(this))
{
    ui->setupUi(this);

    // for WAV audio
    m_format.setSampleRate(44100);
    m_format.setChannelCount(2);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::SignedInt);

    connect(ui->startSessionBtn, &QPushButton::clicked, this, &VoiceChatController::onSessionStart);
    connect(ui->joinSession, &QPushButton::clicked, this, &VoiceChatController::onSessionJoin);

    connect(ui->quit, &QPushButton::clicked, this, &VoiceChatController::quitSession);
    connect(ui->quitSession, &QPushButton::clicked, this, &VoiceChatController::quitSession);

    connect(m_server, &QTcpServer::newConnection, this, &VoiceChatController::onNewConnection);

    // Default client parameters for debugging purposes
    ui->iPAddressLineEdit->setText("127.0.0.1");
    ui->portLineEdit->setText("7777");
}

VoiceChatController::~VoiceChatController()
{
    delete ui;
}

void VoiceChatController::hostSession()
{
    ui->pages->setCurrentWidget(ui->hostPage);
    setWindowTitle("Host Session");
    QDialog::show();
}

void VoiceChatController::joinSession()
{
    ui->pages->setCurrentWidget(ui->joinPage);
    setWindowTitle("Join Session");
    QDialog::show();
}

// Private member functions
void VoiceChatController::onSessionStart()
{
    //m_server = new VoipServer(7777, *m_format, this);
    m_server->listen(QHostAddress::Any, VOIP_PORT);
}

void VoiceChatController::onSessionJoin()
{
    // Create a VoIP client with the IP address and port passed into the constructor
    QString ip = ui->iPAddressLineEdit->text();
    ushort port = ui->portLineEdit->text().toUShort();

    QHostAddress host(ip);
    quint32 ipToHost = host.toIPv4Address();

    QTcpSocket* socket = new QTcpSocket(this);

    socket->connectToHost(host, port);

    connect(socket, &QTcpSocket::readyRead, this, &VoiceChatController::onReadyRead);

    m_clients[ipToHost] = socket;
    m_inputs[ipToHost] = new QAudioInput(m_format, this);
    m_inputs[ipToHost]->start(socket);
}

void VoiceChatController::quitSession()
{

}

void VoiceChatController::onConnected()
{
    qDebug() << "connected";
}

void VoiceChatController::onNewConnection()
{
    qDebug() << "new connection";
    QTcpSocket* clientSocket = m_server->nextPendingConnection();
    quint32 clientAddress = clientSocket->peerAddress().toIPv4Address();

    if (m_clients.contains(clientAddress))
    {
        clientSocket->close();
        clientSocket->deleteLater();
        return;
    }

    connect(clientSocket, &QTcpSocket::readyRead, this, &VoiceChatController::onReadyRead);

    m_clients[clientAddress] = clientSocket;
    m_inputs[clientAddress] = new QAudioInput(m_format, this);
    m_inputs[clientAddress]->start(clientSocket);
}

void VoiceChatController::onReadyRead()
{
    QTcpSocket* sender = (QTcpSocket*) QObject::sender();
    quint32 senderAddress = sender->peerAddress().toIPv4Address();
    if (!m_clients.contains(senderAddress))
    {
        m_outputs[senderAddress] = new QAudioOutput(m_format, this);
        m_outputs[senderAddress]->start(m_clients[senderAddress]);
    }
}
void VoiceChatController::onAudioStateChange(QAudio::State state)
{
}
