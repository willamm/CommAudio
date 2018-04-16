#include "voicechatcontroller.h"
#include "ui_voicechatcontroller.h"

VoiceChatController::VoiceChatController(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoiceChatController),
    m_server(new QTcpServer(this)),
    m_socket(new QTcpSocket(this))
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
    m_server->listen(QHostAddress::Any, 7777);
}

void VoiceChatController::onSessionJoin()
{
    // Create a VoIP client with the IP address and port passed into the constructor
    QString ip = ui->iPAddressLineEdit->text();
    ushort port = ui->portLineEdit->text().toUShort();
    //m_client = new VoipClient(ip, port, *m_format, this);

    m_socket->connectToHost(ip, port);

    connect(m_socket, &QTcpSocket::connected, this, &VoiceChatController::onConnected);
}

void VoiceChatController::quitSession()
{

}

void VoiceChatController::onConnected()
{
    QAudioInput* input = new QAudioInput(m_format, this);
    QAudioOutput* output = new QAudioOutput(m_format, this);

    input->start(m_socket);
    output->start(m_socket);
}

void VoiceChatController::onNewConnection()
{
    QAudioInput* input = new QAudioInput(m_format, this);
    QAudioOutput* output = new QAudioOutput(m_format, this);

    QTcpSocket* clientSocket = m_server->nextPendingConnection();

    input->start(clientSocket);
    output->start(clientSocket);
}

void VoiceChatController::onReadyRead()
{
}
void VoiceChatController::onAudioStateChange(QAudio::State state)
{
}
