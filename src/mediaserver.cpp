#include "mediaserver.h"

#include <QTcpSocket>
#include <QErrorMessage>
#include <QInputDialog>

MediaServer::MediaServer(QObject *parent, int port) : QObject(parent)
{
    if (!m_server_tcp.listen(QHostAddress::Any, port))
    {
        QErrorMessage errMsg;
        errMsg.showMessage(QString("Error while trying to listen on port %1").arg(port));
    }
    connect(&m_server_tcp, &QTcpServer::newConnection, this, &MediaServer::onNewConnection);



    m_server_udp.bind(QHostAddress::Any, port + 1);
    connect(&m_server_udp, SIGNAL(readyRead()), this, SLOT(readyUdp()));

    qInfo() << "Server ready.\n";

}

void MediaServer::onNewConnection()
{

    QTcpSocket* socket = m_server_tcp.nextPendingConnection();
    clients_tcp.push_back(socket);
    qInfo() << "New client successfully connected.\n";
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyTcp()));
    emit updateMainWindow(socket);

}

std::vector<QTcpSocket*> MediaServer::getClients() {
    return clients_tcp;
}

void MediaServer::readyTcp()//QString fileName)
{
    QString filePath = "C:/Users/Matt/Music/";

    for(int i = 0; i < clients_tcp.size(); i++) {
        QString temp = clients_tcp.at(i)->readAll();
        filePath.append(temp);
        qInfo() << filePath;

        if (fileExists(filePath)) {
            QFile file(filePath);

            file.open(QIODevice::ReadOnly);

            QByteArray byteArr;
            byteArr = file.readAll();
            clients_tcp.at(i)->write(byteArr);

            file.close();
            qInfo() << "file sent\n";
        }
    }
}




void MediaServer::readyUdp()//QString fileName)
{
    QByteArray buffer;
    buffer.resize(m_server_udp.pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    int currSize;
    currSize = m_server_udp.readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);
    //outputFile << buffer.toStdString();
    //qInfo() << buffer.toStdString();

//    if (currSize == 0)
//    {
//        stop();
//    }
}





bool MediaServer::fileExists(QString path) {
    QFileInfo check_file(path);
    return check_file.exists() && check_file.isFile();
}


std::ifstream inputFile;

