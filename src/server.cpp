#include "server.h"

#include <QTcpSocket>
#include <QErrorMessage>
#include <QFileDialog>

Server::Server(QObject *parent) : QObject(parent)
{

}


void Server::openServer()
{
    if (!m_server_sock.listen(QHostAddress::Any, 5150))
    {
        QErrorMessage errMsg;
        errMsg.showMessage(QString("Error while trying to listen on port %1").arg(5150));
    }
    connect(&m_server_sock, &QTcpServer::newConnection, this, &Server::onNewConnection);
    qInfo() << "new server\n";
}



void Server::onNewConnection()
{
    m_server_client_sock = m_server_sock.nextPendingConnection();
    qInfo() << "new clientsad\n";

    connect(m_server_client_sock, SIGNAL(readyRead()), this, SLOT(readyRead()));
    //sendFile();
}


void Server::readyRead()//QString fileName)
{
   outputFile << m_server_client_sock->readAll().toHex().toStdString();
   //qInfo() << m_server_client_sock->readAll().toHex();
}
