#ifndef MEDIASERVER_H
#define MEDIASERVER_H

#include <QObject>
#include <QTcpServer>
#include <QUdpSocket>
#include <QTcpSocket>
#include <vector>
#include <QFileInfo>
#include <QFile>


class MediaServer : public QObject
{
    Q_OBJECT
public:
    explicit MediaServer(QObject *parent = nullptr, int port = 0);
    std::vector<QTcpSocket*> getClients();
    bool fileExists(QString path);

signals:
    void updateMainWindow(QTcpSocket*);

public slots:
    void onNewConnection();
    void readyTcp();
    void readyUdp();

private:

    QTcpServer m_server_tcp;
    QUdpSocket m_server_udp;
    std::vector<QTcpSocket*> clients_tcp;
    std::vector<QUdpSocket*> clients_udp;
};

#endif // MEDIASERVER_H
