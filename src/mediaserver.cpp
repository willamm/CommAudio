#include "mediaserver.h"

#include <QTcpSocket>
#include <QErrorMessage>

MediaServer::MediaServer(QObject *parent, int port) : QObject(parent)
{
    if (!m_server.listen(QHostAddress::Any, port))
    {
        QErrorMessage errMsg;
        errMsg.showMessage(QString("Error while trying to listen on port %1").arg(port));
    }
    connect(&m_server, &QTcpServer::newConnection, this, &MediaServer::onNewConnection);
}

void MediaServer::onNewConnection()
{
    QTcpSocket* socket = m_server.nextPendingConnection();
}
