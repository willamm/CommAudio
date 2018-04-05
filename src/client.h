#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <fstream>
#include <QFile>

class Client : public QObject
{

    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    //std::ifstream inputFile;
    QString fileName;


public slots:
    void connectClient();


private:
    QTcpSocket m_client_sock;


};

#endif // CLIENT_H
