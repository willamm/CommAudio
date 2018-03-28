#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_server(new MediaServer(this, 5150))
    , m_voiceChat(new VoiceChatController(this))
{
    QMediaPlayer* player = new QMediaPlayer;

    m_ui->setupUi(this);
    // Test for playing audio
    connect(m_ui->actionAdd_file, &QAction::triggered, this, [this, player] (){
       fileName = QFileDialog::getOpenFileName(this, tr("Open file"));
       player->setMedia(QUrl::fromLocalFile(fileName));
       player->setVolume(50);
       player->play();
    });

    connect(m_ui->actionStart_session, &QAction::triggered, m_voiceChat, &VoiceChatController::hostSession);
    connect(m_ui->actionJoin_session, &QAction::triggered, m_voiceChat, &VoiceChatController::joinSession);
    connect(m_ui->playPauseBtn, SIGNAL(pressed()), this, SLOT(player->pause());
}

MainWindow::~MainWindow()
{
    delete m_ui;
}
