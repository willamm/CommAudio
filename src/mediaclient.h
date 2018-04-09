#ifndef MEDIACLIENT_H
#define MEDIACLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <fstream>
#include <QInputDialog>
#include <QLineEdit>

class MediaClient : public QObject
{

    Q_OBJECT

public:
    explicit MediaClient(QObject *parent = nullptr);
    std::ifstream inputFile;
    void stream();

public slots:
    void connectClient();
    void getIP();

private:
    QTcpSocket m_client_sock;
    QString ipAddress;
    bool connected;

};

#endif // MEDIACLIENT_H
