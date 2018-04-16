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

// CONSTRUCTOR/DESTRUCTOR
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
-- INTERFACE: MediaClient (QObject* parent)
--                         QObject* parent: the parent object
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

// PUBLIC SLOTS
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
        QMessageBox msgBox;
        msgBox.setText("Connected to Server!");
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

        QDir dir;
        qInfo() << dir.currentPath() << "\n" << dir.homePath() << "\n";
        QString filePath = dir.homePath() + "/Documents/";
        filePath.append(file);
        outputFile.open(filePath.toStdString(), std::ios_base::binary);
        qInfo() << "File saved as " << filePath << "\n";

        emit mediaLoaded(filePath);

    } else {
        QMessageBox msgBox;
        msgBox.setText("Please connect to server first.");
        msgBox.exec();
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

        QDir dir;
        QString filePath = dir.homePath() + "/Documents/streamFile.wav";
        outputFile.close();
        outputFile.open(filePath.toStdString(), std::ios_base::binary);

        streamOn = true;
        connect(&m_client_sock, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(timer, SIGNAL(timeout()), this, SLOT(closeFile()));

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

    if (streamOn) {
        outputFile << m_client_sock.readAll().toStdString();
        timer->start(2000);

    } else {
        outputFile << m_client_sock.readAll().toStdString();
        timer->start(2000);
    }

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
    QDir dir;
    if (streamOn) {
        outputFile.close();
        QAudioOutput* audioOutpu;

        QAudioFormat format;
        format.setSampleRate(44100);
        format.setChannelCount(2);
        format.setSampleSize(16);
        format.setCodec("audio/pcm");
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setSampleType(QAudioFormat::SignedInt);

        QString filePath = dir.homePath() + "/Documents/streamFile.wav";
        QFile file(filePath);
        file.open(QIODevice::ReadOnly);

        audioOutpu = new QAudioOutput(format);
        audioOutpu->setVolume(1.0);

        audioOutpu->start(&file);
        QEventLoop loop;
        QObject::connect(audioOutpu, SIGNAL(stateChanged(QAudio::State)), &loop, SLOT(quit()));
        do {
            loop.exec();
        } while(audioOutpu->state() == QAudio::ActiveState);
        file.remove();
        streamOn = false;

    } else {
        outputFile.close();
    }

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
//            QNetworkDatagram datagram = m_client_usock.receiveDatagram();
//            processStream(datagram);


            QByteArray data;
            data.resize(m_client_usock.pendingDatagramSize());
            //m_client_usock.readDatagram(data.data(), data.size());
            //device->write(data.data(), data.size());
            QNetworkDatagram datagram = m_client_usock.receiveDatagram();
            device->write(datagram.data(), datagram.data().size());

            //qInfo() << data.data();
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
    QByteArray audioPacket = datagram.data();
    QAudioFormat format;
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









