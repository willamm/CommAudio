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

#ifndef SERVERSTREAM_H
#define SERVERSTREAM_H

#include <QUdpSocket>
#include <QAudioInput>
#include <QAudioFormat>
#include <QFile>
#include <QDir>
#include <QTcpSocket>

class ServerStream : public QObject
{
      Q_OBJECT

public:
    explicit ServerStream(QObject *parent = nullptr, int port = 0);
    ~ServerStream();

public slots:
    void process();
    void tcpCast();
    void setClients(std::vector<QTcpSocket*>);

signals:
    void finished();
    void error(QString err);

private:
    int portNum;
    QUdpSocket * m_server_udp;
    std::vector<QTcpSocket*> clients_tcp;
};


#endif // SERVERSTREAM_H


