#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMediaPlayer>

#include <memory>

#include "mediaserver.h"
#include "voicechatcontroller.h"
#include "client.h"
#include "server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

public slots:
    void getFileInputName();
    void getFileOutputName();

private:
    Ui::MainWindow* m_ui;

    //MediaServer* m_server;
    Client* m_client;
    Server* m_server;
    VoiceChatController* m_voiceChat;
    QString fileName;
};

#endif // MAINWINDOW_H
