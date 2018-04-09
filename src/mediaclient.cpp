#include "mediaclient.h"

MediaClient::MediaClient(QObject *parent) : QObject(parent)
  , connected(false)
{
    timer = new QTimer(this);
}

void MediaClient::connectClient()
{

    connect(&m_client_sock, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),this,
            [=](QAbstractSocket::SocketError socketError){
        qInfo() << m_client_sock.errorString(); });

    m_client_sock.connectToHost(ipAddress, 5150);
    connected = true;

    QByteArray ba = fileName.toLatin1();
    const char *temp = ba.data();
    m_client_sock.write(temp);

    connect(&m_client_sock, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(timer, SIGNAL(timeout()), this, SLOT(closeFile()));
    timer->start(2000);


}

void MediaClient::getReqInfo() {
    bool ok;
    QString ip = QInputDialog::getText(Q_NULLPTR, tr("Download from Server"), tr("Server IP Address:"), QLineEdit::Normal, "127.0.0.1", &ok);
    QString file = QInputDialog::getText(Q_NULLPTR, tr("Download from Server"), tr("Song Request:"), QLineEdit::Normal, "", &ok);
    if (ok && !ip.isEmpty() && !file.isEmpty()) {
        ipAddress = ip;
        fileName = file;
        connectClient();
    }

    QString filePath = "C:/Users/Matt/Music/Client_";
    filePath.append(file);
    outputFile.open(filePath.toStdString(), std::ios_base::binary);
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
























