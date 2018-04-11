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
--      updateWindowTitle (void)
--      playRequest(QString filePath);
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai, Will Murphy, Matthew Shew
--
-- PROGRAMMER: Will Murphy, Calvin Lai, Matthew Shew
--
-- NOTES:
-- The the main window for the media player. This file contains the gui and all user controls such as seek, volume
-- control, playlist display, play/pause, etc. All media control signals and slots are connected in this file.
----------------------------------------------------------------------------------------------------------------------*/

#include "mainwindow.h"

// CONSTRUCTOR/DESTRUCTOR
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: MainWindow
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai, Matthew Shew, William Murphy
--
-- PROGRAMMER: Calvin Lai, Matthew Shew, William Murphy
--
-- INTERFACE: MainWindow (QWidget* parent)
--                        QWidget* parent: the parent widget
--
-- RETURNS: N/A
--
-- NOTES:
-- Constructor for the mainwindow class. Sets up a majority of the signals and slots for the media player.
----------------------------------------------------------------------------------------------------------------------*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , player(new mPlayer(this))
    , m_server(new MediaServer(this, 5150))
    , m_client(new MediaClient(this))
    , m_voiceChat(new VoiceChatController(this))
{
    QDir dir;
    QString filePath = dir.homePath() + "/Documents/";
    m_ui->setupUi(this);

    connect(this, SIGNAL(addedMedia()), this, SLOT(updatePlayList()));
    //chat options
    connect(m_ui->actionStart_session, &QAction::triggered, m_voiceChat, &VoiceChatController::hostSession);
    connect(m_ui->actionJoin_session, &QAction::triggered, m_voiceChat, &VoiceChatController::joinSession);
    //client options
    connect(m_ui->actionConnect, &QAction::triggered, m_client, &MediaClient::connectToServer);
    connect(m_ui->actionListen, &QAction::triggered, m_client, &MediaClient::startStream);
    connect(m_ui->actionJoin_Group, &QAction::triggered, m_client, &MediaClient::joinGroup);
    connect(m_ui->actionRequest_2, &QAction::triggered, m_client, &MediaClient::request);
    //media options
    connect(m_ui->actionAdd_file, &QAction::triggered, this, [this, filePath] (){
       fileName = QFileDialog::getOpenFileName(this, tr("Open file"), filePath, tr("Audio Files (*.mp3 *.wav)") );
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
    //menu options
    connect(m_server, SIGNAL(updateMainWindow(QHostAddress, quint16)), this, SLOT(updateClientList(QHostAddress, quint16)));
    connect(player->control(), SIGNAL(metaDataChanged()), this, SLOT(updateWindowTitle()));
    connect(m_client, SIGNAL(streamMode()), this, SLOT(updateWindowTitle()));
    connect(m_ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(exit(bool)));
    connect(m_client, SIGNAL(mediaLoaded(QString)), this, SLOT(playRequest(QString)));
    connect(m_client, SIGNAL(playStream(QByteArray)), this, SLOT(playStream(QByteArray)));
    connect(m_ui->actionBroadcast, &QAction::triggered, m_server, &MediaServer::broadcast);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ~MainWIndow
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: ~MainWindow (void)
--
-- RETURNS: void
--
-- NOTES:
-- Destructor for the class.
----------------------------------------------------------------------------------------------------------------------*/
MainWindow::~MainWindow()
{
    delete m_ui;
}

// PUBLIC SLOTS
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: getFileInputName
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai, Matthew Shew
--
-- PROGRAMMER: Calvin Lai, Matthew Shew
--
-- INTERFACE: getFileInputName (void)
--
-- RETURNS: void
--
-- NOTES:
-- Prompts user for file to send to client.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::getFileInputName()
{

    //fileName = QFileDialog::getOpenFileName(this, tr("Open file"));
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose File to Send"), "./", tr("Text File (*.mp3)"));
    qInfo() << "new file: \n" + fileName;

    m_server->inputFile.open(fileName.toStdString(), std::ios_base::binary);

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: exit
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: exit (bool clicked)
--                  bool clicked: status of the click action
--
-- RETURNS: void
--
-- NOTES:
-- Quits the application.
----------------------------------------------------------------------------------------------------------------------*/
bool MainWindow::exit(bool clicked) {
    if (clicked) {
        QApplication::quit();
    } else {
        QApplication::quit();
    }
    return true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: previous
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: previous (void)
--
-- RETURNS: void
--
-- NOTES:
-- Sets the current media to the previous media in the playlist if less than 3 seconds have been played. Restarts the
-- current media if more than 3 seconds have played.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::previous() {
    if (player->control()->position() <= 3000) {
        player->prev();
    } else {
        player->control()->setPosition(0);
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: setVolume
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: setVolume (int value)
--                       int value: the value to set the volume to
--
-- RETURNS: void
--
-- NOTES:
-- Sets the volume to the selected value.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::setVolume(int value) {
    player->control()->setVolume(value);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: seek
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: seek (int value)
--                  int value: the position to seek to
--
-- RETURNS: void
--
-- NOTES:
-- Jumps to the selected position of the currently playing media.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::seek(int value) {
    player->control()->setPosition(value);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: durationChanged
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: durationChanged (void)
--
-- RETURNS: void
--
-- NOTES:
-- Sets the maximum value of the horizontal slider when the current audio file is updated.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::durationChanged(qint64 duration) {
    this->duration = duration/1000;
    m_ui->horizontalSlider->setMaximum(duration);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: positionChanged
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: positionChanged (qint64 progress)
--                             quint progress: the value to set the slider position to
--
-- RETURNS: void
--
-- NOTES:
-- Updates the horizontal slider widget as the song progresses.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::positionChanged(qint64 progress) {
    if(!m_ui->horizontalSlider->isSliderDown()) {
        m_ui->horizontalSlider->setValue(progress);
    }
    updateDurationInfo(progress/1000);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: updateDurationInfo
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: updateDurationInfo (double currentInfoD)
--                                double currentInfoD: the current elapsed time
--
-- RETURNS: void
--
-- NOTES:
-- Updates the current progress of the current media.
----------------------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: updatePlayList
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: updatePlayList (void)
--
-- RETURNS: void
--
-- NOTES:
-- Updates the playlist widget with the song title and album artist of thew newly added media file.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::updatePlayList() {
    QTreeWidgetItem *playlistItem = new QTreeWidgetItem(m_ui->playList);
    playlistItem->setText(0, player->control()->metaData(QMediaMetaData::Title).toString());
    playlistItem->setText(1, player->control()->metaData(QMediaMetaData::Author).toString());
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: updateClientList
--
-- DATE: April 10, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: updateClientList (QHostAddress ip, quint16 port)
--                              QHostAddress ip: the ip address of the client
--                              quint16 port: the port of the client
--
-- RETURNS: void
--
-- NOTES:
-- Updates the clientlist widget with the newly connected client.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::updateClientList(QHostAddress ip, quint16 port) {
    QTreeWidgetItem *newClient = new QTreeWidgetItem(m_ui->clientList);

    QString clientport = QString::number(port, 10);
    QString client = ip.toString();
    QByteArray ipBA = client.toLatin1();
    QByteArray portBA = clientport.toLatin1();

    const char *listIP = ipBA.data();
    const char *listPort = portBA.data();

    newClient->setText(0, tr(listIP));
    newClient->setText(1, tr(listPort));

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: updateWindowTitle
--
-- DATE: April 10, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: updateWindowTitle (void)
--
-- RETURNS: void
--
-- NOTES:
-- Updates the window title with the player's current media (if any) and player status.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::updateWindowTitle() {
    if (!m_client->getUdpStatus()) {
        QString trackInfo = player->control()->metaData(QMediaMetaData::Title).toString();
        QMediaPlayer::State state = player->control()->state();
        QString status;
        switch (state) {
            case QMediaPlayer::StoppedState:
                status = "Stopped";
                break;
            case QMediaPlayer::PlayingState:
                status = "Playing";
                break;
            case QMediaPlayer::PausedState:
                status = "Paused";
                break;
            default:
                status = "N/A";
                break;
        }
        setWindowTitle(QString("Qtify Audio Player || %1 || %2").arg(trackInfo).arg(status));
    } else {
        setWindowTitle(QString("Qtify Audio Player || Streaming from %1").arg(m_client->getIpAddress()));
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: playRequest
--
-- DATE: April 10, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: playRequest (QString filePath)
--                         QString filePath: the filepath of the requested song
--
-- RETURNS: void
--
-- NOTES:
-- Plays the requested song from server.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::playRequest(QString filePath) {
    player->getPlaylist()->clear();
    player->addToQueue(QUrl::fromLocalFile(filePath));
    player->next();
    emit addedMedia();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: playStream
--
-- DATE: April 11, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: playStream (QByteArray audioPacket)
--                        QByteArray audioPacket: the packet received from stream
--
-- RETURNS: void
--
-- NOTES:
-- Plays the streamed packet from the server.
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::playStream(QByteArray audioPacket) {
//    QEventLoop loop;
    QAudioFormat format;
    QAudioOutput* audio;

    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
    audio = new QAudioOutput(format, this);
    QBuffer buffer(&audioPacket);
    buffer.open(QBuffer::ReadWrite);
    audio->start(&buffer);

}
