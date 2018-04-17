/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: voicechatcontroller.h
--
-- PROGRAM: Qtify Audio Player
--
-- FUNCTIONS
--  public:
--      explicit VoiceChatController(QWidget *parent = nullptr);
--      ~VoiceChatController();
--
--  public slots:
--      void hostSession();
--      void joinSession();
--  private slots:
--      void onNewConnection();
--      void onReadyRead();
--      void onAudioInputStateChange(QAudio::State state);
--      void onSessionStart();
--      void onSessionJoin();
--  signals:
--      void sessionStarted();
--      void sessionJoined();
--
-- DATE: April 6, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: William Murphy, Matthew Shew, Calvin Lai
--
-- PROGRAMMER: William Murphy
--
-- NOTES:
-- Handles the voice chat use case.
----------------------------------------------------------------------------------------------------------------------*/
#ifndef VOICECHATCONTROLLER_H
#define VOICECHATCONTROLLER_H

#include <QAudioDeviceInfo>
#include <QDialog>
#include <QAudioInput>
#include <QAudioOutput>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QMap>

namespace Ui {
class VoiceChatController;
}

class VoiceChatController : public QDialog
{
    Q_OBJECT

public:
    explicit VoiceChatController(QWidget *parent = nullptr);
    ~VoiceChatController();

public slots:
    void hostSession();
    void joinSession();

private slots:
    void onNewConnection();
    void onReadyRead();
    void onSessionStart();
    void onSessionJoin();
    void quitSession();
    void onConnected();

private:
    Ui::VoiceChatController *ui;

    QAudioFormat m_format;

    QTcpServer* m_server;

    QMap<quint32, QTcpSocket*> m_clients;
    QMap<quint32, QAudioInput*> m_inputs;
    QMap<quint32, QAudioOutput*> m_outputs;
};


#endif // VOICECHATCONTROLLER_H
