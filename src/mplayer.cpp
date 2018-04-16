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
--      QMediaPlaylist* getPlaylist() ;
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
-- PROGRAMMER: Calvin Lai
--
-- NOTES:
-- Mediaplayer class for the main window. This class acts as an intermediate between the player and user controls.
-- Controls all media controls.
----------------------------------------------------------------------------------------------------------------------*/
#include "mplayer.h"


// CONSTRUCTOR/DESTRUCTOR
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: mPlayer
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: mPlayer (QObject* parent)
--                     QObject* parent: the parent object
--
-- RETURNS: void
--
-- NOTES:
-- Constructor for the class. Instantiates the media player and playlist and sets the volume for player.
----------------------------------------------------------------------------------------------------------------------*/
mPlayer::mPlayer(QObject* parent)
{
    player = new QMediaPlayer(this);
    player->setVolume(50);
    playlist = new QMediaPlaylist(this);
    player->setPlaylist(playlist);
    playing = false;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ~mPlayer
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: ~mPlayer ()
--
-- RETURNS: void
--
-- NOTES:
-- Destructor for the class.
----------------------------------------------------------------------------------------------------------------------*/
mPlayer::~mPlayer() {

}

// PUBLIC SLOTS
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: play
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: void play ()
--
-- RETURNS: void
--
-- NOTES:
-- Called when the play button is pressed. If the player is currently playing media, it pauses playback. If the player
-- is not currently playing, it begins to play.
----------------------------------------------------------------------------------------------------------------------*/
void mPlayer::play() {
    if (playlist->isEmpty()) {
        return;
    }
    if (playing) {
        player->pause();
        playing = false;
        emit stateChanged();
    } else {
        player->play();
        playing = true;
        emit stateChanged();
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: next
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: next ()
--
-- RETURNS: void
--
-- NOTES:
-- Skips to the next track in the playlist, does nothing if playlist is empty.
----------------------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: prev
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: prev ()
--
-- RETURNS: void
--
-- NOTES:
-- Restarts the current media if less than 3 seconds have played, else plays the previous media.
----------------------------------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: control
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: QMediaPlayer* control ()
--
-- RETURNS: QMediaPlayer*
--
-- NOTES:
-- Returns the mediaplayer.
----------------------------------------------------------------------------------------------------------------------*/
QMediaPlayer* mPlayer::control() {
    return player;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: getPlaylist
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: QMediaPlaylist* getPlaylist ()
--
-- RETURNS: QMediaPlaylist*
--
-- NOTES:
-- Returns the mediaplaylist.
----------------------------------------------------------------------------------------------------------------------*/
QMediaPlaylist* mPlayer::getPlaylist() {
    return playlist;
}

// WRAPPERS
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: addToQueue
--
-- DATE: April 3, 2018
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Calvin Lai
--
-- PROGRAMMER: Calvin Lai
--
-- INTERFACE: bool addToQueue (const QMediaContent &content)
--                                   QMediaContent &content: the media content to add
--
-- RETURNS: bool, true if successful, else false
--
-- NOTES:
-- Wrapper function that adds the media content to the playback queue.
----------------------------------------------------------------------------------------------------------------------*/
bool mPlayer::addToQueue(const QMediaContent &content) {
    return playlist->addMedia(content);
}
