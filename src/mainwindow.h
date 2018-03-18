#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMediaPlayer>

#include <memory>

#include "mediaserver.h"
#include "voicechatcontroller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private:
    Ui::MainWindow* m_ui;

    std::unique_ptr<MediaServer> m_server;
    std::unique_ptr<VoiceChatController> m_voiceChat;
    QString fileName;
};

#endif // MAINWINDOW_H
