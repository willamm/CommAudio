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
--      void readyStream();
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
-- INTERFACE: MediaServer (QObject, int)
--
-- RETURNS: N/A
--
-- NOTES:
-- The constructor for a media server object. Takes in a parent object and a port number.
----------------------------------------------------------------------------------------------------------------------*/
MediaServer::MediaServer(QObject *parent, int port) : QObject(parent)
{
    if (!m_server_tcp.listen(QHostAddress::Any, port))
    {
        QErrorMessage errMsg;
        errMsg.showMessage(QString("Error while trying to listen on port %1").arg(port));
    }
    connect(&m_server_tcp, &QTcpServer::newConnection, this, &MediaServer::onNewConnection);



    m_server_udp.bind(QHostAddress::Any, port + 1);
    connect(&m_server_udp, SIGNAL(readyRead()), this, SLOT(readyUdp()));

    qInfo() << "Server ready.\n";

}

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
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyStream()));
    emit updateMainWindow(socket->peerAddress(), socket->peerPort());
}

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
    QString filePath = "C:/Users/Matt/Music/";
    int size = (int) clients.size();
    for(int i = 0; i < size; i++) {
        QString temp = clients.at(i)->readAll();
        filePath.append(temp);
        qInfo() << filePath;

        if (fileExists(filePath)) {
            QFile file(filePath);

            file.open(QIODevice::ReadOnly);

            QByteArray byteArr;
            byteArr = file.readAll();
            clients_tcp.at(i)->write(byteArr);

            file.close();
            qInfo() << "file sent\n";
        }
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: readyUdp()
--
-- DATE: April 10, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Matthew Shew
--
-- PROGRAMMER: Matthew Shew
--
-- INTERFACE: bool readyUdp (void)
--
-- RETURNS: bool
--
-- NOTES:
-- Function is called when the UDP socket is ready for reading.
----------------------------------------------------------------------------------------------------------------------*/
void MediaServer::readyUdp()
{
    QByteArray buffer;
    buffer.resize(m_server_udp.pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    int currSize;
    currSize = m_server_udp.readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);
    //outputFile << buffer.toStdString();
    //qInfo() << buffer.toStdString();

//    if (currSize == 0)
//    {
//        stop();
//    }
}

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
-- INTERFACE: bool fileExists (QString)
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
