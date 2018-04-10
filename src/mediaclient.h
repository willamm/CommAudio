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
    void connectClient();
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
