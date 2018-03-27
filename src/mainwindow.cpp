#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_server(new MediaServer(this, 5150))
    , m_voiceChat(new VoiceChatController(this))
{
    m_ui->setupUi(this);
    // Test for playing audio
    connect(m_ui->actionAdd_file, &QAction::triggered, this, [this] (){
       fileName = QFileDialog::getOpenFileName(this, tr("Open file"));
       QMediaPlayer* player = new QMediaPlayer;
       player->setMedia(QUrl::fromLocalFile(fileName));
       player->setVolume(50);
       player->play();
    });

    connect(m_ui->actionStart_session, &QAction::triggered, m_voiceChat, &VoiceChatController::hostSession);
    connect(m_ui->actionJoin_session, &QAction::triggered, m_voiceChat, &VoiceChatController::joinSession);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}
