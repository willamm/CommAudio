#ifndef SERVER_H
#define SERVER_H


#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <fstream>

class Server : public QObject
{

    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

    std::ofstream outputFile;

public slots:
    void openServer();
    void onNewConnection();
    void readyRead();



private:
    QTcpServer m_server_sock;
    QTcpSocket* m_server_client_sock;


};

#endif // SERVER_H
