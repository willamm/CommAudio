#ifndef SERVERSTREAM_H
#define SERVERSTREAM_H

#include <QUdpSocket>

class ServerStream : public QObject
{
      Q_OBJECT
public:
    explicit ServerStream(QObject *parent = nullptr, int port = 0);
    ~ServerStream();

public slots:
    void process();

signals:
    void finished();
    void error(QString err);

private:
    int portNum;
    QUdpSocket * m_server_udp;
};


#endif // SERVERSTREAM_H


