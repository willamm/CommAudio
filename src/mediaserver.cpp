/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: mediaserver.cpp
--
-- PROGRAM: Qtify Audio Player
--
-- FUNCTIONS
--  public:
--      explicit MediaServer(QObject *parent = nullptr, int port = 0);
--      std::vector<QTcpSocket*> getClients();
--      std::ifstream inputFile;
--      bool fileExists(QString path);
--
--  signals:
--      void updateMainWindow(QHostAddress, quint16);
--
--  public slots:
--      void onNewConnection();
--      void readyTcp();
--      void broadcast();
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai, Will Murphy, Matthew Shew
--
-- PROGRAMMER: Matthew Shew, Calvin Lai
--
-- NOTES:
-- This class handles all server side connections for the audio player. It is responsible for both UDP and TCP
-- connections with the client as well as all file transfers.
----------------------------------------------------------------------------------------------------------------------*/
#include "mediaserver.h"

// CONSTRUCTOR/DESTRUCTOR
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: MediaServer
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai, Matthew Shew, William Murphy
--
-- PROGRAMMER: Calvin Lai, Matthew Shew, William Murphy
--
-- INTERFACE: MediaServer (QObject* parent, int port)
--                         QObject* parent: the parent object
--                         int port: the port number
--
-- RETURNS: N/A
--
-- NOTES:
-- The constructor for a media server object. Takes in a parent object and a port number.
----------------------------------------------------------------------------------------------------------------------*/
MediaServer::MediaServer(QObject *parent, int port) : QObject(parent)
{
    portNum = port;
    if (!m_server_tcp.listen(QHostAddress::Any, portNum))
    {
        QErrorMessage errMsg;
        errMsg.showMessage(QString("Error while trying to listen on port %1").arg(portNum));
    }
    connect(&m_server_tcp, &QTcpServer::newConnection, this, &MediaServer::onNewConnection);
    qInfo() << "Server ready.\n";

}

// PUBLIC SLOTS
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: onNewConnection
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai, Matthew Shew
--
-- PROGRAMMER: Calvin Lai, Matthew Shew
--
-- INTERFACE: void   (void)
--
-- RETURNS: void.
--
-- NOTES:
-- This function is called when a new connection is established. Adds the new client to a vector of clients.
----------------------------------------------------------------------------------------------------------------------*/
void MediaServer::onNewConnection()
{
    QTcpSocket* socket = m_server_tcp.nextPendingConnection();
    clients_tcp.push_back(socket);
    qInfo() << "New client successfully connected.\n";
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyTcp()));
    emit updateMainWindow(socket->peerAddress(), socket->peerPort());
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: readyTcp
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai, Matthew Shew
--
-- PROGRAMMER: Calvin Lai, Matthew Shew
--
-- INTERFACE: void readyTcp (void)
--
-- RETURNS: void.
--
-- NOTES:
-- This function is called when the TCP socket is ready for streaming.
----------------------------------------------------------------------------------------------------------------------*/
void MediaServer::readyTcp()
{
    QDir dir;
    QString filePath = dir.homePath() + "/Documents/";
    int size = (int) clients_tcp.size();
    for(int i = 0; i < size; i++) {
        QString temp = clients_tcp.at(i)->readAll();
        filePath.append(temp);
        qInfo() << filePath;
        if (fileExists(filePath)) {
            QFile file(filePath);
            file.open(QIODevice::ReadOnly);
            QByteArray byteArr;
            byteArr = file.readAll();
            clients_tcp.at(i)->write(byteArr);
            file.close();
            qInfo() << "File successfully sent to " << clients_tcp.at(i)->peerAddress().toString() << "\n";
        } else {
            QMessageBox msgBox;
            msgBox.setText("File does not exist, please request another file.");
            msgBox.exec();
        }
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: broadcast
--
-- DATE: April 11, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Matthew Shew
--
-- PROGRAMMER: Matthew Shew
--
-- INTERFACE: void broadcast (void)
--
-- RETURNS: void
--
-- NOTES:
-- Starts a thread to broadcast to all connected clients.
----------------------------------------------------------------------------------------------------------------------*/
void MediaServer::broadcast() {
    QDir dir;
    QString filePath = dir.homePath() + "/Documents/";
    QString fileName = QFileDialog::getOpenFileName(NULL, tr("Open file"), filePath, tr("Audio Files (*.mp3 *.wav)") );
    QByteArray data;

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    data = file.readAll();

    int size = (int) clients_tcp.size();
    for(int i = 0; i < size; i++) {
        qInfo() << "sent";
        clients_tcp.at(i)->write(data);
    }
    file.close();
}

// PUBLIC FUNCTIONS
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: fileExists
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Matthew Shew
--
-- PROGRAMMER: Matthew Shew
--
-- INTERFACE: bool fileExists (QString path)
--                             QString path: the file path of the file to check
--
-- RETURNS: bool
--
-- NOTES:
-- Checks the specified file path if the file exists.
----------------------------------------------------------------------------------------------------------------------*/
bool MediaServer::fileExists(QString path) {
    QFileInfo check_file(path);
    return check_file.exists() && check_file.isFile();
}

// GETTERS/SETTERS
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: getClients
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: std::vector<QTcpSocket*> getClients (void)
--
-- RETURNS: std::vector<QTcpSocket*>
--
-- NOTES:
-- Returns the list of currently connected clients.
----------------------------------------------------------------------------------------------------------------------*/
std::vector<QTcpSocket*> MediaServer::getClients() {
    return clients_tcp;
}
