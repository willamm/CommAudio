#ifndef MEDIASERVER_H
#define MEDIASERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <vector>
<<<<<<< HEAD
#include <fstream>
=======
#include <QFileInfo>
#include <QFile>

>>>>>>> 42b45d56e3c29732a6e8b3ba43fac0f689538da4

class MediaServer : public QObject
{
    Q_OBJECT
public:
    explicit MediaServer(QObject *parent = nullptr, int port = 0);
    std::vector<QTcpSocket*> getClients();
<<<<<<< HEAD
    std::ifstream inputFile;
=======
    bool fileExists(QString path);
>>>>>>> 42b45d56e3c29732a6e8b3ba43fac0f689538da4

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
