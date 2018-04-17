/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: voicechatcontroller.cpp
--
-- PROGRAM: Qtify Audio Player
--
-- FUNCTIONS
--  public:
--      explicit VoiceChatController(QWidget *parent = nullptr);
--      ~VoiceChatController();
--
--  public slots:
--      void hostSession();
--      void joinSession();
--  private slots:
--      void onNewConnection();
--      void onReadyRead();
--      void onAudioInputStateChange(QAudio::State state);
--      void onSessionStart();
--      void onSessionJoin();
--  signals:
--      void sessionStarted();
--      void sessionJoined();
--
-- DATE: April 6, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: William Murphy, Matthew Shew, Calvin Lai
--
-- PROGRAMMER: William Murphy
--
-- NOTES:
-- Handles the voice chat use case.
----------------------------------------------------------------------------------------------------------------------*/
#include "voicechatcontroller.h"
#include "ui_voicechatcontroller.h"

#define VOIP_PORT 7777

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: VoiceChatController
--
-- PROGRAM: Qtify Audio Player
--
-- DATE: April 6, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: William Murphy, Matthew Shew, Calvin Lai
--
-- PROGRAMMER: William Murphy
--
-- INTERFACE: VoiceChatController(QWidget* parent)
--
-- NOTES:
-- Constructor for the VoiceChatController class.
----------------------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ~VoiceChatController
--
-- PROGRAM: Qtify Audio Player
--
-- DATE: April 6, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: William Murphy, Matthew Shew, Calvin Lai
--
-- PROGRAMMER: William Murphy
--
-- INTERFACE: ~VoiceChatController()
--
-- NOTES:
-- Destructor for the VoiceChatController class.
----------------------------------------------------------------------------------------------------------------------*/
VoiceChatController::~VoiceChatController()
{
    delete ui;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: hostSession
--
-- PROGRAM: Qtify Audio Player
--
-- DATE: April 6, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: William Murphy, Matthew Shew, Calvin Lai
--
-- PROGRAMMER: William Murphy
--
-- INTERFACE: hostSession()
--
-- NOTES:
-- Shows the dialog for hosting a VoIP session.
----------------------------------------------------------------------------------------------------------------------*/
void VoiceChatController::hostSession()
{
    ui->pages->setCurrentWidget(ui->hostPage);
    setWindowTitle("Host Session");
    QDialog::show();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: joinSession
--
-- PROGRAM: Qtify Audio Player
--
-- DATE: April 6, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: William Murphy, Matthew Shew, Calvin Lai
--
-- PROGRAMMER: William Murphy
--
-- INTERFACE: joinSession()
--
-- NOTES:
-- Shows the dialog for joining a VoIP session.
----------------------------------------------------------------------------------------------------------------------*/
void VoiceChatController::joinSession()
{
    ui->pages->setCurrentWidget(ui->joinPage);
    setWindowTitle("Join Session");
    QDialog::show();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: onSessionStart
--
-- PROGRAM: Qtify Audio Player
--
-- DATE: April 6, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: William Murphy, Matthew Shew, Calvin Lai
--
-- PROGRAMMER: William Murphy
--
-- INTERFACE: onSessionStart()
--
-- NOTES:
-- Starts the TCP server when the session start button is clicked.
----------------------------------------------------------------------------------------------------------------------*/
void VoiceChatController::onSessionStart()
{
    m_server->listen(QHostAddress::Any, VOIP_PORT);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: onSessionJoin
--
-- PROGRAM: Qtify Audio Player
--
-- DATE: April 6, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: William Murphy, Matthew Shew, Calvin Lai
--
-- PROGRAMMER: William Murphy
--
-- INTERFACE: onSessionJoin()
--
-- NOTES:
-- Joins a VoIP session when the join session button is clicked.
----------------------------------------------------------------------------------------------------------------------*/
void VoiceChatController::onSessionJoin()
{
    // Create a VoIP client with the IP address and port passed into the constructor
    QString ip = ui->iPAddressLineEdit->text();
    ushort port = ui->portLineEdit->text().toUShort();

    QHostAddress host(ip);
    quint32 ipToHost = host.toIPv4Address();

    QTcpSocket* socket = new QTcpSocket(this);

    if (m_clients.contains(ipToHost))
    {
        return;
    }

    socket->connectToHost(host, port);

    connect(socket, &QTcpSocket::readyRead, this, &VoiceChatController::onReadyRead);

    m_clients[ipToHost] = socket;
    m_inputs[ipToHost] = new QAudioInput(m_format, this);
    m_inputs[ipToHost]->start(socket);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: quitSession
--
-- PROGRAM: Qtify Audio Player
--
-- DATE: April 6, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: William Murphy, Matthew Shew, Calvin Lai
--
-- PROGRAMMER: William Murphy
--
-- INTERFACE: quitSession()
--
-- NOTES:
-- Closes the session when the Quit button is clicked.
----------------------------------------------------------------------------------------------------------------------*/
void VoiceChatController::quitSession()
{
    QList<quint32> connections = m_clients.keys();
    for (quint32 i : connections) {
        m_inputs[i]->stop();
        m_outputs[i]->stop();
        m_clients[i]->close();

    }
    m_clients.clear();
    m_outputs.clear();
    m_inputs.clear();
    m_server->close();
    QDialog::close();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: onConnected
--
-- PROGRAM: Qtify Audio Player
--
-- DATE: April 6, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: William Murphy, Matthew Shew, Calvin Lai
--
-- PROGRAMMER: William Murphy
--
-- INTERFACE: onConnected()
--
-- NOTES:
-- Handles socket connection events.
----------------------------------------------------------------------------------------------------------------------*/
void VoiceChatController::onConnected()
{
    qDebug() << "connected";
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: onNewConnection
--
-- PROGRAM: Qtify Audio Player
--
-- DATE: April 6, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: William Murphy, Matthew Shew, Calvin Lai
--
-- PROGRAMMER: William Murphy
--
-- INTERFACE: onNewConnection()
--
-- NOTES:
-- Handles socket connection events for the TCP server.
----------------------------------------------------------------------------------------------------------------------*/
void VoiceChatController::onNewConnection()
{
    qDebug() << "new connection";
    QTcpSocket* clientSocket = m_server->nextPendingConnection();
    quint32 clientAddress = clientSocket->peerAddress().toIPv4Address();

//    if (m_clients.contains(clientAddress))
//    {
//        clientSocket->close();
//        clientSocket->deleteLater();
//        return;
//    }

    connect(clientSocket, &QTcpSocket::readyRead, this, &VoiceChatController::onReadyRead);

    m_clients[clientAddress] = clientSocket;
    m_inputs[clientAddress] = new QAudioInput(m_format, this);
    m_inputs[clientAddress]->start(clientSocket);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: onReadyRead
--
-- PROGRAM: Qtify Audio Player
--
-- DATE: April 6, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: William Murphy, Matthew Shew, Calvin Lai
--
-- PROGRAMMER: William Murphy
--
-- INTERFACE: onReadyRead()
--
-- NOTES:
-- Handles the readyRead() event that is emitted by a TCP socket.
----------------------------------------------------------------------------------------------------------------------*/
void VoiceChatController::onReadyRead()
{
    QTcpSocket* sender = (QTcpSocket*) QObject::sender();
    quint32 senderAddress = sender->peerAddress().toIPv4Address();
    if (!m_outputs.contains(senderAddress))
    {
        m_outputs[senderAddress] = new QAudioOutput(m_format, this);
        m_outputs[senderAddress]->start(m_clients[senderAddress]);
    }
}
