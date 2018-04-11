#include "serverstream.h"

ServerStream::ServerStream(QObject *parent, int port) : QObject(parent)
{
    portNum = port;
}


ServerStream::~ServerStream()
{
}



void ServerStream::process() {

    m_server_udp = new QUdpSocket(this);
    QHostAddress groupAddress = QHostAddress("239.255.43.21");
    m_server_udp->bind(QHostAddress(QHostAddress::AnyIPv4), 0);

    m_server_udp->setSocketOption(QAbstractSocket::MulticastTtlOption, 1);

    const QByteArray datagram = "Multicast message ";

    while (1) {
        m_server_udp->writeDatagram(datagram.data(), datagram.size(), groupAddress, 45454);
    }
    qDebug("Hello World!");
    emit finished();
}

