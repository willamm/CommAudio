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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , player(new mPlayer)
    , m_server(new MediaServer(this, 5150))
    , m_client(new MediaClient(this))
    , m_voiceChat(new VoiceChatController(this))
{
    m_ui->setupUi(this);

    connect(this, SIGNAL(addedMedia()), this, SLOT(updatePlayList()));
    connect(m_ui->actionStart_session, &QAction::triggered, m_voiceChat, &VoiceChatController::hostSession);
    connect(m_ui->actionJoin_session, &QAction::triggered, m_voiceChat, &VoiceChatController::joinSession);
    connect(m_ui->actionConnect, &QAction::triggered, m_client, &MediaClient::getReqInfo);
    connect(m_ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(exit(bool)));
    connect(m_ui->actionAdd_file, &QAction::triggered, this, [this] (){
       fileName = QFileDialog::getOpenFileName(this, tr("Open file"));
       player->addToQueue(QUrl::fromLocalFile(fileName));
       emit addedMedia();
    });
    connect(m_ui->playPauseBtn, SIGNAL(clicked(bool)), player, SLOT(play()));
    connect(m_ui->fastForwardBtn, SIGNAL(clicked(bool)), player, SLOT(next()));
    connect(m_ui->rewindBtn, SIGNAL(clicked(bool)), this, SLOT(previous()));
    connect(player->control(), SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
    connect(player->control(), SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(m_ui->horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));
    connect(m_ui->volumeControl, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));
    m_ui->volumeControl->setValue(50);
    m_ui->horizontalSlider->setRange(0, player->control()->duration()/ 10000);
    connect(m_server, SIGNAL(updateMainWindow(QTcpSocket*)), this, SLOT(updateClientList(QTcpSocket*)));

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

    m_server->inputFile.open(fileName.toStdString(), std::ios_base::binary);

}

bool MainWindow::exit(bool clicked) {
    if (clicked) {
        QApplication::quit();
    } else {
        QApplication::quit();
    }
    return true;
}

void MainWindow::previous() {
    if (player->control()->position() <= 3000) {
        player->prev();
    } else {
        player->control()->setPosition(0);
    }
}

void MainWindow::setVolume(int value) {
    player->control()->setVolume(value);
}

void MainWindow::seek(int value) {
    player->control()->setPosition(value);
}
void MainWindow::durationChanged(qint64 duration) {
    this->duration = duration/1000;
    m_ui->horizontalSlider->setMaximum(duration);
}

void MainWindow::positionChanged(qint64 progress) {
    if(!m_ui->horizontalSlider->isSliderDown()) {
        m_ui->horizontalSlider->setValue(progress);
    }
    updateDurationInfo(progress/1000);
}

void MainWindow::updateDurationInfo(double currentInfoD)
{
    qint64 currentInfo = (qint64)currentInfoD;
    QString rStr;
    QString lStr;
    if (currentInfo || duration) {
        QTime currentTime((currentInfo/3600)%60, (currentInfo/60)%60, currentInfo%60,
            ((qint64)(currentInfoD*1000))%1000);
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

void MainWindow::updatePlayList() {
    QTreeWidgetItem *playlistItem = new QTreeWidgetItem(m_ui->playList);
    playlistItem->setText(0, player->control()->metaData(QMediaMetaData::Title).toString());
    playlistItem->setText(1, player->control()->metaData(QMediaMetaData::Author).toString());
}

void MainWindow::updateClientList(QTcpSocket* socket) {
    QTreeWidgetItem *newClient = new QTreeWidgetItem(m_ui->clientList);
//    auto it = m_server->getClients().begin();
//    for (; it != m_server->getClients().end(); ++it) {
//        QTcpSocket* temp = *it;
////        QString clientID = temp->localAddress();
////        char *str=(char *)malloc(1000);
////        QByteArray ba = clientID.toLatin1();
//        qInfo() << clientID;
////        strcpy(str,ba.data());
////        newClient->setText(0, tr(str));
//    }
    /*
    QString clientID = socket->localAddress().scopeId();
    char *str=(char *)malloc(10);
    QByteArray ba = clientID.toLatin1();
    qInfo() << clientID;
    strcpy(str,ba.data());
    */
    newClient->setText(0, tr("NEW CONNECTION"));

}
