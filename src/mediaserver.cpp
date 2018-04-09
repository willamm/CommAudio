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
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyStream()));
    emit updateMainWindow(socket);

}

std::vector<QTcpSocket*> MediaServer::getClients() {
    return clients;
}

void MediaServer::readyStream()//QString fileName)
{
    QString filePath = "C:/Users/Matt/Music/";

    for(int i = 0; i < clients.size(); i++) {
        QString temp = clients.at(i)->readAll();
        filePath.append(temp);
        qInfo() << filePath;

        if (fileExists(filePath)) {
            QFile file(filePath);

            file.open(QIODevice::ReadOnly);

            QByteArray byteArr;
            byteArr = file.readAll();
            clients.at(i)->write(byteArr);

            file.close();
            qInfo() << "file sent\n";
        }
    }
}




bool MediaServer::fileExists(QString path) {
    QFileInfo check_file(path);
    return check_file.exists() && check_file.isFile();
}


std::ifstream inputFile;

