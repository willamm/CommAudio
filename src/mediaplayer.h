//#ifndef MEDIAPLAYER_H
//#define MEDIAPLAYER_H

//#include <QWidget>
//#include <QtWidgets>
//#include <QMediaPlayer>
//#include <QMediaPlaylist>
//#include <QMediaService>
//#include <QMediaMetaData>
//#include <QSettings>

//#include "mediaserver.h"
//#include "voicechatcontroller.h"

//class MediaPlayer : public QWidget
//{
//    Q_OBJECT

//public:
//    MediaPlayer(QWidget *parent = 0);
//    ~MediaPlayer();

//private slots:
//    void open();
//    void durationChanged(qint64 duration);
//    void positionChanged(qint64 progress);
//    void metaDataChanged();

//    void previousClicked();

//    void seek(int seconds);
//    void jump(const QModelIndex &index);
//    void playlistPositionChanged(int);

//    void statusChanged(QMediaPlayer::MediaStatus status);
//    void bufferingProgress(int progress);
//    void displayErrorMessage();


//private:

//    void setTrackInfo(const QString &info);
//    void setStatusInfo(const QString &info);
//    void handleCursor(QMediaPlayer::MediaStatus status);
//    void updateDurationInfo(double currentInfo);

//    QMediaPlayer* player;
//    QMediaPlaylist* playlist;

//    QLabel *coverLabel;
//    QSlider *slider;
//    QLabel *labelDuration;

//    QString trackInfo;
//    QString statusInfo;
//    qint64 duration;

//    MediaServer* m_server;
//    VoiceChatController* m_voiceChat;
//    QString fileName;
//};


//#endif // MEDIAPLAYER_H
