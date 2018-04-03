/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: mainwindow.cpp
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
--  private:
--      Ui::MainWindow* m_ui;
--      mPlayer* player;
--      MediaServer* m_server;
--      VoiceChatController* m_voiceChat;
--      QString fileName;
--      quint64 duration;
--      bool playing;
--
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai, Will Murphy, Matthew Shew
--
-- PROGRAMMER: Will Murphy, Calvin Lai
--
-- NOTES:
-- The the main window for the media player. This file contains the gui and all user controls such as seek, volume
-- control, playlist display, play/pause, etc. All media control signals and slots are connected in this file.
----------------------------------------------------------------------------------------------------------------------*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mplayer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_server(new MediaServer(this, 5150))
    , m_voiceChat(new VoiceChatController(this))
{
    player = new mPlayer;
    m_ui->setupUi(this);
    m_ui->horizontalSlider->setTickInterval(QSlider::TicksBothSides);


    connect(m_ui->actionStart_session, &QAction::triggered, m_voiceChat, &VoiceChatController::hostSession);
    connect(m_ui->actionJoin_session, &QAction::triggered, m_voiceChat, &VoiceChatController::joinSession);

    // Mediaplayer setup
    connect(m_ui->actionAdd_file, &QAction::triggered, this, [this] (){
       fileName = QFileDialog::getOpenFileName(this, tr("Open file"));
       player->addToQueue(QUrl::fromLocalFile(fileName));
    });
    connect(m_ui->playPauseBtn, SIGNAL(clicked(bool)), player, SLOT(play()));
    connect(m_ui->fastForwardBtn, SIGNAL(clicked(bool)), player, SLOT(next()));
    connect(m_ui->rewindBtn, SIGNAL(clicked(bool)), player, SLOT(prev()));
    connect(player->control(), SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
    connect(player->control(), SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(m_ui->horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));
    connect(m_ui->volumeControl, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));
    m_ui->volumeControl->setValue(50);
    m_ui->horizontalSlider->setRange(0, player->control()->duration()/ 10000);

}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::setVolume(int value) {
    player->control()->setVolume(value);
}

void MainWindow::seek(int value) {
    player->control()->setPosition(value / SLIDER_DIVISOR);
}
void MainWindow::durationChanged(qint64 duration) {
    this->duration = duration/1000;
    m_ui->horizontalSlider->setMaximum(duration);
}

void MainWindow::positionChanged(qint64 progress) {
    if(!m_ui->horizontalSlider->isSliderDown()) {
        m_ui->horizontalSlider->setValue(progress/ SLIDER_DIVISOR);
    }
    updateDurationInfo(progress/1000);
}

void MainWindow::updateDurationInfo(double currentInfoD)
{
    qint64 currentInfo = (qint64)currentInfoD;
    QString rStr;
    QString lStr;
    if (currentInfo || duration) {
        QTime currentTime((currentInfo/3600)%60, // hr
                          (currentInfo/60)%60, // min
                          currentInfo%60, // sec
                          ((qint64)(currentInfoD*1000))%1000 // ms
                          );
        QTime totalTime((duration/3600)%60, (duration/60)%60, duration%60, (duration*1000)%1000);
        QString format = "mm:ss";
        QString format2 = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        rStr = totalTime.toString(format2);
        lStr = currentTime.toString(format);
    }
    m_ui->left->setText(lStr);
    m_ui->right->setText(rStr);
}
