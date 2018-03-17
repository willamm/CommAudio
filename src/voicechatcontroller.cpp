#include "voicechatcontroller.h"
#include "ui_voicechatcontroller.h"

VoiceChatController::VoiceChatController(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoiceChatController)
{
    ui->setupUi(this);
}

VoiceChatController::~VoiceChatController()
{
    delete ui;
}
