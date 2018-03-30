//#include "mediaplayer.h"

//MediaPlayer::MediaPlayer(QWidget *parent):
//    QWidget(parent)
//  , m_server(new MediaServer(this, 5150))
//  , m_voiceChat(new VoiceChatController(this))

//{
//    player = new QMediaPlayer(this);
//    playlist = new QMediaPlaylist();
//    player->setPlaylist(playlist);

//    connect(player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
//    connect(player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
//    connect(player, SIGNAL(metaDataChanged()), SLOT(metaDataChanged()));
//    connect(playlist, SIGNAL(currentIndexChanged(int)), SLOT(playlistPositionChanged(int)));
//    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
//            this, SLOT(statusChanged(QMediaPlayer::MediaStatus)));
//    connect(player, SIGNAL(bufferStatusChanged(int)), this, SLOT(bufferingProgress(int)));
//    connect(player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(displayErrorMessage()));

//    slider = new QSlider(Qt::Horizontal, this);
////    slider->setRange(0, player->duration() / SLIDER_DIVISOR);

//    labelDuration = new QLabel(this);
//    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));

//}


