#ifndef MEDIASERVER_H
#define MEDIASERVER_H

class QTcpServer;

class MediaServer
{
public:
    MediaServer();
    ~MediaServer();

private:
    QTcpServer* server;
};

#endif // MEDIASERVER_H
