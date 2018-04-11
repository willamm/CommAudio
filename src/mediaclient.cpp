/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: mediaclient.cpp
--
-- PROGRAM: Qtify Audio Player
--
-- FUNCTIONS
--  public:
--      explicit MediaClient(QObject *parent = nullptr);
--      void stream();
--      void processStream(QNetworkDatagram datagram);
--      void joinGroup();
--      void request();
--
--  public slots:
--      void startStream();
--      void connectToServer();
--      void getReqInfo();
--      void readyRead();
--      void closeFile();
--      void readPendingDatagrams();
--
-- DATE: April 10, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai, Matthew Shew
--
-- PROGRAMMER: Matthew Shew, Calvin Lai
--
-- NOTES:
-- This class handles all client side connections for the audio player. It is responsible for both UDP and TCP
-- connections with the server.
----------------------------------------------------------------------------------------------------------------------*/

#include "mediaclient.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: MediaClient
--
-- DATE: April 10, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai, Matthew Shew
--
-- PROGRAMMER: Calvin Lai, Matthew Shew
--
-- INTERFACE: MediaClient (QObject)
--
-- RETURNS: N/A
--
-- NOTES:
-- The constructor for a media server object. Takes in a parent object.
----------------------------------------------------------------------------------------------------------------------*/
MediaClient::MediaClient(QObject *parent) : QObject(parent)
  , connected(false)
{
    timer = new QTimer(this);

}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: connectToServer
--
-- DATE: April 10, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai, Matthew Shew
--
-- PROGRAMMER: Calvin Lai, Matthew Shew
--
-- INTERFACE: connectToServer (void)
--
-- RETURNS: void
--
-- NOTES:
-- Initializes the TCP socket and establishes a connection to a user specified server.
----------------------------------------------------------------------------------------------------------------------*/
void MediaClient::connectToServer() {
    bool ok;
    QString ip = QInputDialog::getText(Q_NULLPTR, tr("Connect to Server"), tr("Server IP Address:"), QLineEdit::Normal, "127.0.0.1", &ok);
    if (ok && !ip.isEmpty()) {
        ipAddress = ip;
        connect(&m_client_sock, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),this,
                [=](QAbstractSocket::SocketError socketError) {
            qInfo() << m_client_sock.errorString();
        });
        m_client_sock.connectToHost(ipAddress, 5150);
        connected = true;
    }

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: startStream
--
-- DATE: April 10, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai, Matthew Shew
--
-- PROGRAMMER: Calvin Lai, Matthew Shew
--
-- INTERFACE: startStream (void)
--
-- RETURNS: void
--
-- NOTES:
-- Initializes the UDP socket and sets it up to start receiving UDP sockets from server.
----------------------------------------------------------------------------------------------------------------------*/
void MediaClient::startStream()
{
    if (!ipAddress.isEmpty()) {
        QHostAddress serverIP(ipAddress);
        //m_client_usock.bind(serverIP, 7755);


        QHostAddress groupAddress = QHostAddress("239.255.43.21");
        m_client_usock.bind(QHostAddress::AnyIPv4, 45454, QUdpSocket::ShareAddress);
        m_client_usock.joinMulticastGroup(groupAddress);
        connect(&m_client_usock, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
        uconnected = true;
        emit streamMode();
    } else {
        QMessageBox msgBox;
        msgBox.setText("Please connect to server first.");
        msgBox.exec();
    }
}


void MediaClient::stream() {

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: readyRead
--
-- DATE: April 10, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Matthew Shew
--
-- PROGRAMMER: Matthew Shew
--
-- INTERFACE: readyRead (void)
--
-- RETURNS: void
--
-- NOTES:
-- Called when data is received by the TCP socket, saves data to specified output file. Starts a timer in case of
-- time out.
----------------------------------------------------------------------------------------------------------------------*/
void MediaClient::readyRead()
{
    outputFile << m_client_sock.readAll().toStdString();
    timer->start(2000);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: closeFile
--
-- DATE: April 10, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Matthew Shew
--
-- PROGRAMMER: Matthew Shew
--
-- INTERFACE: closeFile (void)
--
-- RETURNS: void
--
-- NOTES:
-- Closes the output file.
----------------------------------------------------------------------------------------------------------------------*/
void MediaClient::closeFile()
{
    outputFile.close();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: readPendingDatagrams
--
-- DATE: April 10, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai, Matthew Shew
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: readPendingDatagrams (void)
--
-- RETURNS: void
--
-- NOTES:
-- Receives pending datagrams and processes them.
----------------------------------------------------------------------------------------------------------------------*/
void MediaClient::readPendingDatagrams()
{
    if (uconnected) {
        while (m_client_usock.hasPendingDatagrams()) {
            QNetworkDatagram datagram = m_client_usock.receiveDatagram();
            processStream(datagram);
        }
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: processStream
--
-- DATE: April 10, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: processStream (void)
--
-- RETURNS: void
--
-- NOTES:
-- Processes incoming audio datagrams.
----------------------------------------------------------------------------------------------------------------------*/
void MediaClient::processStream(QNetworkDatagram datagram) {
    qInfo() << datagram.data().data();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: joinGroup
--
-- DATE: April 10, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: joinGroup (void)
--
-- RETURNS: void
--
-- NOTES:
-- Called to join a multicast group.
----------------------------------------------------------------------------------------------------------------------*/
void MediaClient::joinGroup() {
    if (ipAddress.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText("Please connect to server first.");
        msgBox.exec();
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: request
--
-- DATE: April 10, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai, Matthew Shew
--
-- PROGRAMMER: Calvin Lai, Matthew Shew
--
-- INTERFACE: request (void)
--
-- RETURNS: void
--
-- NOTES:
-- Requests a user specified audio file from the server. If file is found, the file is transfered and saved locally.
----------------------------------------------------------------------------------------------------------------------*/
void MediaClient::request() {
    if (!ipAddress.isEmpty()) {
        bool ok;
        QString file = QInputDialog::getText(Q_NULLPTR, tr("Download from Server"), tr("Song Request:"), QLineEdit::Normal, "01 Say My Name (Dabin Remix).mp3", &ok);
        if (ok && !file.isEmpty()) {
            fileName = file;

            QByteArray ba = fileName.toLatin1();
            const char *temp = ba.data();
            m_client_sock.write(temp);

            connect(&m_client_sock, SIGNAL(readyRead()), this, SLOT(readyRead()));
            connect(timer, SIGNAL(timeout()), this, SLOT(closeFile()));
            timer->start(2000);
        }
//        QString filePath = "C:/Users/Matt/Music/Client_";
//        QString filePath = "/Users/clai/Downloads/Client_";
        QDir dir;
        qInfo() << dir.currentPath() << "\n" << dir.homePath() << "\n";
        QString filePath = dir.homePath() + "/Documents/";
        filePath.append(file);
        outputFile.open(filePath.toStdString(), std::ios_base::binary);
        qInfo() << "File saved as " << filePath << "\n";

        //readyRead();
        emit mediaLoaded(filePath);

    } else {
        QMessageBox msgBox;
        msgBox.setText("Please connect to server first.");
        msgBox.exec();
    }
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: getUdpStatus
--
-- DATE: April 10, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: getUdpStatus (void)
--
-- RETURNS: bool
--
-- NOTES:
-- Returns the UDP connection status, true if connected, else false.
----------------------------------------------------------------------------------------------------------------------*/
bool MediaClient::getUdpStatus() {
    return uconnected;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: getTcpStatus
--
-- DATE: April 10, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: getTcpStatus (void)
--
-- RETURNS: bool
--
-- NOTES:
-- Returns the TCP connection status, true if connected, else false.
----------------------------------------------------------------------------------------------------------------------*/
bool MediaClient::getTcpStatus() {
    return connected;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: getIpAddress
--
-- DATE: April 10, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: getIpAddress (void)
--
-- RETURNS: bool
--
-- NOTES:
-- Returns the ipAddress status, true if connected, else false.
----------------------------------------------------------------------------------------------------------------------*/
QString MediaClient::getIpAddress() {
    if (!ipAddress.isEmpty()) {
        return ipAddress;
    } else {
        return NULL;
    }
}









