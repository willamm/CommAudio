#ifndef MPLAYER_H
#define MPLAYER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>

class mPlayer : public QObject
{
    Q_OBJECT

public:
    mPlayer();
    virtual ~mPlayer();
    QMediaPlayer* control();
    bool addToQueue(const QMediaContent &content);

public slots:
    void play();
    void next();
    void prev();
//    void setVolume();

private:
    QMediaPlayer* player;
    QMediaPlaylist* playlist;
    bool playing;
};

#endif // MPLAYER_H
