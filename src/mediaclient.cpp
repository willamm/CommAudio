#include "mediaclient.h"

MediaClient::MediaClient(QObject *parent) : QObject(parent)
  , connected(false)
{
    timer = new QTimer(this);

}

void MediaClient::connectClient()
{
    if (!ipAddress.isEmpty()) {
        QHostAddress serverIP(ipAddress);
        m_client_usock.bind(serverIP, 7755);
        connect(&m_client_usock, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
        uconnected = true;
    } else {
        QMessageBox msgBox;
        msgBox.setText("Please connect to server first.");
        msgBox.exec();
    }
}

void MediaClient::connectToServer() {
    bool ok;
    QString ip = QInputDialog::getText(Q_NULLPTR, tr("Connect to Server"), tr("Server IP Address:"), QLineEdit::Normal, "127.0.0.1", &ok);
    if (ok && !ip.isEmpty()) {
        ipAddress = ip;
        connect(&m_client_sock, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),this,
                [=](QAbstractSocket::SocketError socketError) {
            qInfo() << m_client_sock.errorString();
        });
        m_client_sock.connectToHost(ipAddress, 5150);
        connected = true;
    }

}

void MediaClient::stream() {

}

void MediaClient::readyRead()
{
    outputFile << m_client_sock.readAll().toStdString();
    timer->start(2000);
}

void MediaClient::closeFile()
{
    outputFile.close();
}

void MediaClient::readPendingDatagrams()
{
    if (uconnected) {
        while (m_client_usock.hasPendingDatagrams()) {
            QNetworkDatagram datagram = m_client_usock.receiveDatagram();
            processStream(datagram);
        }
    }
}

void MediaClient::processStream(QNetworkDatagram datagram) {

}

void MediaClient::joinGroup() {

}

void MediaClient::request() {
    bool ok;
    QString file = QInputDialog::getText(Q_NULLPTR, tr("Download from Server"), tr("Song Request:"), QLineEdit::Normal, "", &ok);
    if (ok && !file.isEmpty()) {
        fileName = file;

        QByteArray ba = fileName.toLatin1();
        const char *temp = ba.data();
        m_client_sock.write(temp);

        connect(&m_client_sock, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(timer, SIGNAL(timeout()), this, SLOT(closeFile()));
        timer->start(2000);
    }
    QString filePath = "C:/Users/Matt/Music/Client_";
    filePath.append(file);
    outputFile.open(filePath.toStdString(), std::ios_base::binary);

}














