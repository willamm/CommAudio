#ifndef MEDIASERVER_H
#define MEDIASERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <vector>
#include <fstream>
#include <QFileInfo>
#include <QFile>

class MediaServer : public QObject
{
    Q_OBJECT
public:
    explicit MediaServer(QObject *parent = nullptr, int port = 0);
    std::vector<QTcpSocket*> getClients();
    std::ifstream inputFile;
    bool fileExists(QString path);

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
