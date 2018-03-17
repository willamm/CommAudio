#ifndef VOICECHATCONTROLLER_H
#define VOICECHATCONTROLLER_H

#include <QDialog>

namespace Ui {
class VoiceChatController;
}

class VoiceChatController : public QDialog
{
    Q_OBJECT

public:
    explicit VoiceChatController(QWidget *parent = 0);
    ~VoiceChatController();

private:
    Ui::VoiceChatController *ui;
};

#endif // VOICECHATCONTROLLER_H
