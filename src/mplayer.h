/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE: mplayer.h
--
-- PROGRAM: Qtify Audio Player
--
-- FUNCTIONS:
--  public:
--      mPlayer();
--      virtual ~mPlayer();
--      QMediaPlayer* control();
--      bool addToQueue(const QMediaContent &content);
--
--  public slots:
--      void play();
--      void next();
--      void prev();
--      void setVolume();
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
-- Mediaplayer class for the main window. This class acts as an intermediate between the player and user controls.
-- Controls all media controls.
----------------------------------------------------------------------------------------------------------------------*/
#ifndef MPLAYER_H
#define MPLAYER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>

class mPlayer : public QObject
{
    Q_OBJECT

public:
    explicit mPlayer(QObject* parent = nullptr);
    virtual ~mPlayer();
    QMediaPlayer* control();
    QMediaPlaylist* getPlaylist();
    bool addToQueue(const QMediaContent &content);

public slots:
    void play();
    void next();
    void prev();

private:
    QMediaPlayer* player;
    QMediaPlaylist* playlist;
    bool playing;
};

#endif // MPLAYER_H
