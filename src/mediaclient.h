#ifndef MEDIACLIENT_H
#define MEDIACLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <fstream>
#include <QInputDialog>
#include <QLineEdit>
#include <string.h>
#include <QTimer>

class MediaClient : public QObject
{

    Q_OBJECT

public:
    explicit MediaClient(QObject *parent = nullptr);
    std::ifstream inputFile;
    void stream();
    std::ofstream outputFile;
    QString fileName;

public slots:
    void connectClient();
    void getReqInfo();
    void readyRead();
    void closeFile();

private:
    QTimer * timer;
    QTcpSocket m_client_sock;
    QString ipAddress;
    bool connected;

};

#endif // MEDIACLIENT_H
