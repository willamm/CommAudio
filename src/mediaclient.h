/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE: mediaclient.h
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
-- DESIGNER: Calvin Lai, Will Murphy, Matthew Shew
--
-- PROGRAMMER: Matthew Shew, Calvin Lai
--
-- NOTES:
-- This class handles all client side connections for the audio player. It is responsible for both UDP and TCP
-- connections with the server.
----------------------------------------------------------------------------------------------------------------------*/
#ifndef MEDIACLIENT_H
#define MEDIACLIENT_H

#include <fstream>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QNetworkDatagram>
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QUdpSocket>
#include <string.h>

class MediaClient : public QObject
{

    Q_OBJECT

public:
    explicit MediaClient(QObject *parent = nullptr);
    void stream();
    void processStream(QNetworkDatagram datagram);
    void joinGroup();
    void request();
    std::ofstream outputFile;
    std::ifstream inputFile;
    QString fileName;

public slots:
    void startStream();
    void connectToServer();
//    void getReqInfo();
    void readyRead();
    void closeFile();
    void readPendingDatagrams();

private:
    QTimer * timer;
    QTcpSocket m_client_sock;
    QUdpSocket m_client_usock;
    QString ipAddress;
    bool connected;
    bool uconnected;

};

#endif // MEDIACLIENT_H
