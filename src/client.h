#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <fstream>

class Client : public QObject
{

    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    std::ifstream inputFile;


public slots:
    void connectClient();


private:
    QTcpSocket m_client_sock;


};

#endif // CLIENT_H
