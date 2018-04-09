#include "mediaclient.h"

MediaClient::MediaClient(QObject *parent) : QObject(parent)
  , connected(false)
{

}

void MediaClient::connectClient()
{
    connect(&m_client_sock, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),this,
            [=](QAbstractSocket::SocketError socketError){
        qInfo() << m_client_sock.errorString(); });
    m_client_sock.connectToHost(ipAddress, 5150);
    connected = true;

    /*
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
    */

}

void MediaClient::getIP() {
    bool ok;
    QString temp = QInputDialog::getText(Q_NULLPTR, tr("Stream from Server"), tr("Server IP Address:"), QLineEdit::Normal, "127.0.0.1", &ok);
    if (ok && !temp.isEmpty()) {
        ipAddress = temp;
        connectClient();
    }
}

void MediaClient::stream() {

}
