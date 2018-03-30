#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTime>

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
    void updateDurationInfo(double currentInfoD);

private slots:
    void durationChanged(qint64 position);
private:
    Ui::MainWindow* m_ui;

    MediaServer* m_server;
    VoiceChatController* m_voiceChat;
    QString fileName;
    quint64 duration;
    bool playing;
};

#endif // MAINWINDOW_H
