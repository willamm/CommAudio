#ifndef MEDIASERVER_H
#define MEDIASERVER_H

#include <QObject>
#include <QTcpServer>

class MediaServer : public QObject
{
    Q_OBJECT
public:
    explicit MediaServer(QObject *parent = nullptr, int port = 0);

signals:

public slots:
    void onNewConnection();

private:
    QTcpServer m_server;
};

#endif // MEDIASERVER_H
