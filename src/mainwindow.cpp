#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mplayer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_server(new MediaServer(this, 5150))
    , m_voiceChat(new VoiceChatController(this))
{
    mPlayer* player = new mPlayer;
    m_ui->setupUi(this);
    m_ui->horizontalSlider->setTickInterval(QSlider::TicksBothSides);


    connect(m_ui->actionStart_session, &QAction::triggered, m_voiceChat, &VoiceChatController::hostSession);
    connect(m_ui->actionJoin_session, &QAction::triggered, m_voiceChat, &VoiceChatController::joinSession);

    // Mediaplayer setup
    connect(m_ui->actionAdd_file, &QAction::triggered, this, [this, player] (){
       fileName = QFileDialog::getOpenFileName(this, tr("Open file"));
       player->addToQueue(QUrl::fromLocalFile(fileName));
    });
    connect(m_ui->playPauseBtn, SIGNAL(clicked(bool)), player, SLOT(play()));
    connect(m_ui->fastForwardBtn, SIGNAL(clicked(bool)), player, SLOT(next()));
    connect(m_ui->rewindBtn, SIGNAL(clicked(bool)), player, SLOT(prev()));
//    connect(player->control(), SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
    connect(player->control(), SIGNAL(positionChanged(qint64)), this, SLOT(durationChanged(qint64)));

}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::durationChanged(qint64 duration) {
    this->duration = duration/1000;
    m_ui->horizontalSlider->setMaximum(duration);
    updateDurationInfo(duration/1000);
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
        QString format = "mm:ss.zzz";
        QString format2 = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        rStr = totalTime.toString(format2);
        lStr = currentTime.toString(format);
    }
    m_ui->left->setText(lStr);
    m_ui->right->setText(rStr);
}
