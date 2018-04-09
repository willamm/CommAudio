#ifndef VOICECHATCONTROLLER_H
#define VOICECHATCONTROLLER_H

#include <QAudioDeviceInfo>
#include <QDialog>
#include <QAudioInput>
#include <QTcpServer>

#include "utilities.h"

namespace Ui {
class VoiceChatController;
}

class VoiceChatController : public QDialog
{
    Q_OBJECT

public:
    explicit VoiceChatController(QWidget *parent = 0);
    ~VoiceChatController();

public slots:
    void hostSession();
    void joinSession();

signals:
    void sessionStarted();
    void sessionJoined();

private:
    Ui::VoiceChatController *ui;
    QAudioInput* m_input;
    QAudioDeviceInfo m_deviceInfo;
    QTcpServer* m_chatServer;
};

#endif // VOICECHATCONTROLLER_H
