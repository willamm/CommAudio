#include "voicechatcontroller.h"
#include "ui_voicechatcontroller.h"

VoiceChatController::VoiceChatController(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoiceChatController)
{
    ui->setupUi(this);
    connect(ui->startSessionBtn, &QPushButton::clicked, this, &VoiceChatController::onSessionStart);
}

VoiceChatController::~VoiceChatController()
{
    delete ui;
    delete m_chatServer;
    delete m_input;
}

void VoiceChatController::hostSession()
{
    ui->pages->setCurrentWidget(ui->hostPage);
    QDialog::show();
}

void VoiceChatController::joinSession()
{
    ui->pages->setCurrentWidget(ui->joinPage);
    QDialog::show();
}

// Private member functions
void VoiceChatController::onSessionStart()
{

}
