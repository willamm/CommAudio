/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE: mediaserver.h
--
-- PROGRAM: Qtify Audio Player
--
-- FUNCTIONS
--  public:
--      explicit MediaServer(QObject *parent = nullptr, int port = 0);
--      std::vector<QTcpSocket*> getClients();
--      bool fileExists(QString path);
--
--  signals:
--      void updateMainWindow(QHostAddress);
--
--  public slots:
--      void onNewConnection();
--      void readyTcp();
--      void broadcast();
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
-- This class handles all server side connections for the audio player. It is responsible for both UDP and TCP
-- connections with the client as well as all file transfers.
----------------------------------------------------------------------------------------------------------------------*/
#ifndef MEDIASERVER_H
#define MEDIASERVER_H
#include <QtCore>
#include <fstream>
#include <QDir>
#include <QErrorMessage>
#include <QFileInfo>
#include <QFile>
#include <QInputDialog>
#include <QMessageBox>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAudioOutput>

#include <vector>

#include "serverstream.h"

class MediaServer : public QObject
{
    Q_OBJECT
public:
    explicit MediaServer(QObject *parent = nullptr, int port = 0);
    std::vector<QTcpSocket*> getClients();
    std::ifstream inputFile;
    bool fileExists(QString path);

signals:
    void updateMainWindow(QHostAddress, quint16);

public slots:
    void onNewConnection();
    void readyTcp();
    void broadcast();

private:
    int portNum;
    QTcpServer m_server_tcp;
    std::vector<QTcpSocket*> clients_tcp;
};

#endif // MEDIASERVER_H
