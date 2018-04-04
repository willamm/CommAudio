#include "client.h"


Client::Client(QObject *parent) : QObject(parent)
{

}


void Client::connectClient()
{
    QString ip = "127.0.0.1";

    m_client_sock.connectToHost(ip, 5150);

    //connect(&m_server_sock, &QTcpServer::newConnection, this, &Server::onNewConnection);
    char buffer[8192];
    QByteArray byteArr;
    byteArr.resize(4096);
    int packetSize = 4096;

    int i = 0;
    while (!inputFile.eof())
    {
        i++;

        memset(buffer, '\0', packetSize + 1);
        inputFile.read(buffer, packetSize);
        byteArr.append(buffer);
        m_client_sock.write(byteArr);
        qInfo() << "1 sent "  << byteArr << "\n";
        //qInfo() << "2 sent "  << byteArr.toHex() << "\n";
        byteArr.clear();


    }


    qInfo() << "done sending\n";
}




