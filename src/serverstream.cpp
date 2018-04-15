/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: serverstream.cpp
--
-- PROGRAM: Qtify Audio Player
--
-- FUNCTIONS
--  public:
--      explicit ServerStream(QObject *parent = nullptr, int port = 0);
--      ~ServerStream();
--
--  public slots:
--      void process();
--
--  signals:
--      void finished();
--      void error(QString err);
--
-- DATE: April 11, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai, Matthew Shew
--
-- PROGRAMMER: Matthew Shew, Calvin Lai
--
-- NOTES:
-- This class handles server sided UDP multicasting to clients.
----------------------------------------------------------------------------------------------------------------------*/

#include "serverstream.h"

//// CONSTRUCTOR/DESTRUCTOR
///*------------------------------------------------------------------------------------------------------------------
//-- FUNCTION: ServerStream
//--
//-- DATE: April 11, 2018
//--
//-- REVISIONS: (Date and Description)
//--
//-- DESIGNER: Matthew Shew
//--
//-- PROGRAMMER: Matthew Shew
//--
//-- INTERFACE: ServerStream (QObject* parent, int port)
//--                          QObject* parent: the parent widget
//--                          int port: the port number for UDP socket
//--
//-- RETURNS: void
//--
//-- NOTES:
//-- Constructor for the class.
//----------------------------------------------------------------------------------------------------------------------*/
//ServerStream::ServerStream(QObject *parent, std::vector<QTcpSocket*> clients) : QObject(parent)
//{
//    clients_tcp = clients;
//    portNum = port;
//}

// CONSTRUCTOR/DESTRUCTOR
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ServerStream
--
-- DATE: April 11, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Matthew Shew
--
-- PROGRAMMER: Matthew Shew
--
-- INTERFACE: ServerStream (QObject* parent, int port)
--                          QObject* parent: the parent widget
--                          int port: the port number for UDP socket
--
-- RETURNS: void
--
-- NOTES:
-- Constructor for the class.
----------------------------------------------------------------------------------------------------------------------*/
ServerStream::ServerStream(QObject *parent, int port) : QObject(parent)
{
    portNum = port;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ~ServerStream
--
-- DATE: April 11, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Matthew Shew
--
-- PROGRAMMER: Matthew Shew
--
-- INTERFACE: ~ServerStream()
--
-- RETURNS: void
--
-- NOTES:
-- Constructor for the class.
----------------------------------------------------------------------------------------------------------------------*/
ServerStream::~ServerStream()
{
}

// FUNCTIONS
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: process
--
-- DATE: April 11, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Matthew Shew, Calvin Lai
--
-- PROGRAMMER: Matthew Shew, Calvin Lai
--
-- INTERFACE: process()
--
-- RETURNS: void
--
-- NOTES:
-- Broadcasts a file using UDP multicasting to all connected clients.
----------------------------------------------------------------------------------------------------------------------*/
void ServerStream::setClients(std::vector<QTcpSocket*> clients) {
    clients_tcp = clients;
}

// PUBLIC SLOTS
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: process
--
-- DATE: April 11, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Matthew Shew, Calvin Lai
--
-- PROGRAMMER: Matthew Shew, Calvin Lai
--
-- INTERFACE: process()
--
-- RETURNS: void
--
-- NOTES:
-- Broadcasts a file using UDP multicasting to all connected clients.
----------------------------------------------------------------------------------------------------------------------*/
void ServerStream::process() {
    int arrsize;
    int pos = 0;
    int sizeInArray = 8192;
    QDir dir;

//    QString filePath = QFileDialog::getOpenFileName(, tr("Select a file to Broadcast"), filePath, tr("Audio Files (*.mp3 *.wav)") );
    QString filePath = dir.homePath() + "/Downloads/Tritonal Out My Mind.wav";
    QByteArray data;

    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    data = file.readAll();
    arrsize = data.size();
    QList<QByteArray> arrays;

    while(pos < arrsize) {
        QByteArray arr = data.mid(pos, sizeInArray);
        arrays << arr;
        pos += arr.size();
    }

    m_server_udp = new QUdpSocket(this);
    QHostAddress groupAddress = QHostAddress("239.255.43.21");
    m_server_udp->bind(QHostAddress(QHostAddress::AnyIPv4), 0);
    m_server_udp->setSocketOption(QAbstractSocket::MulticastTtlOption, 1);
    qInfo() << "Multicasting\n";
    while (1) {
        for (int i = 0; i < arrays.size(); i++) {
//            m_server_udp->writeDatagram(data.data(), data.size(), groupAddress, 45454);
                        m_server_udp->writeDatagram(arrays.at(i).data(), arrays.at(i).size(), groupAddress, 45454);

        }
    }
//    emit finished();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: tcpCast
--
-- DATE: April 14, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Matthew Shew, Calvin Lai
--
-- PROGRAMMER: , Calvin Lai
--
-- INTERFACE: tcpCast(std::vector<QTcpSocket*> clients)
--                    std::vector<QTcpSocket*> clients: a vector of all connected clients
--
-- RETURNS: void
--
-- NOTES:
-- Broadcasts a file using TCP to several clients.
----------------------------------------------------------------------------------------------------------------------*/
void ServerStream::tcpCast() {
    int arrsize;
    int pos = 0;
    int sizeInArray = 8192;
    QDir dir;

//    QString filePath = QFileDialog::getOpenFileName(, tr("Select a file to Broadcast"), filePath, tr("Audio Files (*.mp3 *.wav)") );
    QString filePath = dir.homePath() + "/Downloads/Tritonal - Out My Mind.wav";
    QByteArray data;

    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    data = file.readAll();
    arrsize = data.size();
    QList<QByteArray> arrays;

    while(pos < arrsize) {
        QByteArray arr = data.mid(pos, sizeInArray);
        arrays << arr;
        pos += arr.size();
    }

    for (int i = 0; i < clients_tcp.size(); i++) {
        clients_tcp.at(i)->write(data);
    }
}
