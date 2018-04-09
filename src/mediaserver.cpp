#include "mediaserver.h"

#include <QTcpSocket>
#include <QErrorMessage>
#include <QInputDialog>

MediaServer::MediaServer(QObject *parent, int port) : QObject(parent)
{
    if (!m_server.listen(QHostAddress::Any, port))
    {
        QErrorMessage errMsg;
        errMsg.showMessage(QString("Error while trying to listen on port %1").arg(port));
    }
    connect(&m_server, &QTcpServer::newConnection, this, &MediaServer::onNewConnection);
    qInfo() << "Server ready.\n";
}

void MediaServer::onNewConnection()
{
    QTcpSocket* socket = m_server.nextPendingConnection();
    clients.push_back(socket);
    qInfo() << "New client successfully connected.\n";
    emit updateMainWindow(socket);

}

std::vector<QTcpSocket*> MediaServer::getClients() {
    return clients;
}

void MediaServer::readyStream()//QString fileName)
{
//   outputFile << m_server_client_sock->readAll().toHex().toStdString();
   //qInfo() << m_server_client_sock->readAll().toHex();
}


std::ifstream inputFile;

