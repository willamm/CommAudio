/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE: mainwindow.h
--
-- PROGRAM: Qtify Audio Player
--
-- FUNCTIONS:
--  public:
--      explicit MainWindow(QWidget *parent = 0);
--      virtual ~MainWindow();
--      void updateDurationInfo(double currentInfoD);
--
--  private slots:
--      void durationChanged(qint64 position);
--      void positionChanged(qint64 progress);
--      void setVolume(int value);
--      void seek(int value);
--      updateWindowTitle (void);
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai, Will Murphy, Matthew Shew
--
-- PROGRAMMER: Will Murphy, Calvin Lai, Matthew Shew
--
-- NOTES:
-- The the main window for the media player. This file contains the gui and all user controls such as seek, volume
-- control, playlist display, play/pause, etc. All media control signals and slots are connected in this file.
----------------------------------------------------------------------------------------------------------------------*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define SLIDER_DIVISOR 10
#include <QDir>
#include <QFileDialog>
#include <QMainWindow>
#include <QMediaMetaData>
#include <QEventLoop>
#include <QTime>
#include <memory>

#include "mplayer.h"
#include "mediaserver.h"
#include "voicechatcontroller.h"
#include "mediaclient.h"
#include "ui_mainwindow.h"
#include "mplayer.h"

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
    void getFileInputName();

signals:
    void addedMedia();

private slots:
    void durationChanged(qint64 position);
    void positionChanged(qint64 progress);
    void setVolume(int value);
    void seek(int value);
    void previous();
    bool exit(bool clicked);
    void updateClientList(QHostAddress ip, quint16 port);
    void updatePlayList();
    void updateWindowTitle();
    void playRequest(QString filePath);
    void playStream(QByteArray);

private:
    Ui::MainWindow* m_ui;
    mPlayer* player;
    MediaServer* m_server;
    MediaClient* m_client;
    VoiceChatController* m_voiceChat;
    QString fileName;
    quint64 duration;
    bool playing;
};

#endif // MAINWINDOW_H
