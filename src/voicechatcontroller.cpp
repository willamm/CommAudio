#include "voicechatcontroller.h"
#include "ui_voicechatcontroller.h"

#include "voipclient.h"
#include "voipserver.h"

VoiceChatController::VoiceChatController(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoiceChatController),
    m_format(new QAudioFormat)
{
    ui->setupUi(this);
    // for WAV audio
    m_format->setSampleRate(44100);
    m_format->setChannelCount(2);
    m_format->setSampleSize(16);
    m_format->setCodec("audio/pcm");
    m_format->setByteOrder(QAudioFormat::LittleEndian);
    m_format->setSampleType(QAudioFormat::SignedInt);

    connect(ui->startSessionBtn, &QPushButton::clicked, this, &VoiceChatController::onSessionStart);
    connect(ui->joinSession, &QPushButton::clicked, this, &VoiceChatController::onSessionJoin);
    connect(ui->quit, &QPushButton::clicked, this, &VoiceChatController::quitSession);
    connect(ui->quitSession, &QPushButton::clicked, this, &VoiceChatController::quitSession);
}

VoiceChatController::~VoiceChatController()
{
    delete ui;
}

void VoiceChatController::hostSession()
{
    ui->pages->setCurrentWidget(ui->hostPage);
    setWindowTitle("Join Session");
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
    VoipServer* server = new VoipServer(7777, *m_format, this);
}

void VoiceChatController::onSessionJoin()
{
    // Create a VoIP client with the IP address and port passed into the constructor
    QString ip = ui->iPAddressLineEdit->text();
    ushort port = ui->portLineEdit->text().toUShort();
    VoipClient* client = new VoipClient(ip, port, *m_format, this);
}

void VoiceChatController::quitSession()
{
}
void VoiceChatController::onConnected()
{
}

void VoiceChatController::onNewConnection()
{
}

void VoiceChatController::onReadyRead()
{
}
void VoiceChatController::onAudioStateChange(QAudio::State state)
{
}
