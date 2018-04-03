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
--  private:
--      QMediaPlayer* player;
--      QMediaPlaylist* playlist;
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
-- Mediaplayer class for the main window. This class acts as an intermediate between the player and user controls.
-- Controls all media controls.
----------------------------------------------------------------------------------------------------------------------*/
#include "mplayer.h"


// CONSTRUCTOR/DESTRUCTOR
mPlayer::mPlayer()
{
    player = new QMediaPlayer();
    player->setVolume(50);
    playlist = new QMediaPlaylist();
    player->setPlaylist(playlist);
    playing = false;
}

mPlayer::~mPlayer() {

}

// PUBLIC SLOTS
void mPlayer::play() {
    if (playlist->isEmpty()) {
        return;
    }
    if (playing) {
        player->pause();
        playing = false;
    } else {
        player->play();
        playing = true;
    }
}

void mPlayer::next() {
    if (playlist->isEmpty()) {
        return;
    }
    playing = false;
    player->stop();
    playlist->next();
    player->play();
    playing = true;
}

void mPlayer::prev() {
    if (playlist->isEmpty()) {
        return;
    }
    playing = false;
    player->stop();
    playlist->previous();
    player->play();
    playing = true;
}

// GETTER/SETTERS
QMediaPlayer* mPlayer::control() {
    return player;
}

// WRAPPERS
bool mPlayer::addToQueue(const QMediaContent &content) {
    return playlist->addMedia(content);
}
