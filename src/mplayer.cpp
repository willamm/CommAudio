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
