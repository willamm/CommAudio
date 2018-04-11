#include "serverstream.h"

ServerStream::ServerStream(QObject *parent, int port) : QObject(parent)
{
    portNum = port;
}


ServerStream::~ServerStream()
{
}

void ServerStream::process() {
    int arrsize;
    int pos = 0;
    int sizeInArray = 8192;

    QDir dir;
    // pick a file
    QString filePath = dir.homePath() + "/Downloads/Tritonal - Out My Mind.wav";


    QByteArray data;

    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    data = file.readAll();
    arrsize = data.size();
    QList<QByteArray> arrays;

    while(pos < arrsize){
        QByteArray arr = data.mid(pos, sizeInArray);
        arrays << arr;
        pos += arr.size();
    }

    m_server_udp = new QUdpSocket(this);
    QHostAddress groupAddress = QHostAddress("239.255.43.21");
    m_server_udp->bind(QHostAddress(QHostAddress::AnyIPv4), 0);
    m_server_udp->setSocketOption(QAbstractSocket::MulticastTtlOption, 1);
    qInfo() << "Multicasting\n";
    while (1) {
        for (int i = 0; i < arrays.size(); i++) {
            m_server_udp->writeDatagram(arrays.at(i).data(), arrays.at(i).size(), groupAddress, 45454);
        }
    }
//    const QByteArray datagram = "Multicast message ";

//    while (1) {
//        m_server_udp->writeDatagram(datagram.data(), datagram.size(), groupAddress, 45454);
//    }
//    qDebug("Hello World!");
    emit finished();
}

