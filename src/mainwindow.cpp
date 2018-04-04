#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_client(new Client(this))
    , m_server(new Server(this))
    //, m_server(new MediaServer(this, 5150))
    //, m_voiceChat(new VoiceChatController(this))
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

//    connect(m_ui->actionStart_session, &QAction::triggered, m_voiceChat, &VoiceChatController::hostSession);
//    connect(m_ui->actionJoin_session, &QAction::triggered, m_voiceChat, &VoiceChatController::joinSession);

    //connect(m_ui->actionStart, &QAction::triggered, m_server, &Server::openServer);
    connect(m_ui->actionStart, &QAction::triggered, this, &MainWindow::getFileOutputName);

    //connect(m_ui->actionStart_2, &QAction::triggered, m_client, &Client::connectClient);
    connect(m_ui->actionStart_2, &QAction::triggered, this, &MainWindow::getFileInputName);
}

MainWindow::~MainWindow()

{
    delete m_ui;
}


void MainWindow::getFileInputName()
{

    //fileName = QFileDialog::getOpenFileName(this, tr("Open file"));
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose File to Send"), "./", tr("Text File (*.mp3)"));
    qInfo() << "new file: \n" + fileName;

    m_client->inputFile.open(fileName.toStdString(), std::ios_base::binary);

    m_client->connectClient();

}



void MainWindow::getFileOutputName()
{

    //fileName = QFileDialog::getOpenFileName(this, tr("Open file"));
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose File to Save to"), "./", tr("Text File (*.mp3)"));
    qInfo() << "new file: \n" + fileName;

    m_server->outputFile.open(fileName.toStdString(), std::ios_base::binary);

    m_server->openServer();

}
