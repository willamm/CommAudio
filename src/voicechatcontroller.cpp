#include "voicechatcontroller.h"
#include "ui_voicechatcontroller.h"



VoiceChatController::VoiceChatController(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoiceChatController)
{
    ui->setupUi(this);
    QList<QAudioDeviceInfo> inputDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

    for (const QAudioDeviceInfo& deviceInfo : inputDevices)
    {
        ui->availableVoiceInputDevicesComboBox->addItem(deviceInfo.deviceName());

    }
    // does not work, causes runtime seg fault
    connect(ui->availableVoiceInputDevicesComboBox, QOverload<const QString&>::of(&QComboBox::highlighted), this, [this, &inputDevices] ()
    {
        for (const QAudioDeviceInfo& deviceInfo : inputDevices)
        {
            if (deviceInfo.deviceName() == ui->availableVoiceInputDevicesComboBox->currentText())
            {
                m_deviceInfo = deviceInfo;
            }
        }
    });
    connect(ui->startSessionBtn, &QPushButton::clicked, this, [this] () {
        m_input = new QAudioInput(m_deviceInfo);
        if (!m_chatServer->listen())
        {
            //idk
            return;
        }
        m_input->start();
    });
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
