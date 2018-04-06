#ifndef MEDIACLIENT_H
#define MEDIACLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <fstream>
#include <QInputDialog>
#include <QLineEdit>
#include <string.h>

class MediaClient : public QObject
{

    Q_OBJECT

public:
    explicit MediaClient(QObject *parent = nullptr);
    std::ofstream outputFile;
    QString fileName;

public slots:
    void connectClient();
    void getReqInfo();
    void readyRead();

private:
    QTcpSocket m_client_sock;
    QString ipAddress;

};

#endif // MEDIACLIENT_H
