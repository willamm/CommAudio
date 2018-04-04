#ifndef MEDIASERVER_H
#define MEDIASERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <vector>

class MediaServer : public QObject
{
    Q_OBJECT
public:
    explicit MediaServer(QObject *parent = nullptr, int port = 0);
    std::vector<QTcpSocket*> getClients();

signals:
    void updateMainWindow(QTcpSocket*);

public slots:
    void onNewConnection();
    void readyStream();

private:
    QTcpServer m_server;
    std::vector<QTcpSocket*> clients;
};

#endif // MEDIASERVER_H
