#include "VLCPlayer.h"

VLCPlayer::VLCPlayer() : vlcInstance_(nullptr), mediaListPlayer_(nullptr), playbackFinished_(false) {}

VLCPlayer::~VLCPlayer() {
    if (vlcInstance_) {
        libvlc_release(vlcInstance_);
    }
}

bool VLCPlayer::initialize() {
    vlcInstance_ = libvlc_new(0, NULL);
    if (!vlcInstance_) {
        std::cerr << "Failed to initialize VLC instance." << std::endl;
        return false;
    }
    mediaListPlayer_ = libvlc_media_list_player_new(vlcInstance_);
    if (!mediaListPlayer_) {
        std::cerr << "Failed to create media list player." << std::endl;
        return false;
    }
    libvlc_event_manager_t *eventManager = libvlc_media_list_player_event_manager(mediaListPlayer_);
    // Attach the media list player event handler
    libvlc_event_attach(eventManager, libvlc_MediaListPlayerPlayed, handleMediaListPlayerPlayed, this);
    // Attach the media changed event handler
    libvlc_event_attach(eventManager, libvlc_MediaPlayerMediaChanged, handleMediaPlayerMediaChanged, this);
    return true;
}

bool VLCPlayer::playPlaylist(const std::vector<std::string>& playlist, const std::string& localDirectory) {
    libvlc_media_list_t *mediaList = libvlc_media_list_new(vlcInstance_);
    libvlc_media_list_lock(mediaList);
    for (const std::string& file : playlist) {
        std::string filePath = localDirectory + file;
        libvlc_media_t *media = libvlc_media_new_path(vlcInstance_, filePath.c_str());
        if (media) {
            libvlc_media_list_add_media(mediaList, media);
            libvlc_media_release(media);
        }
    }
    libvlc_media_list_unlock(mediaList);
    libvlc_media_list_player_set_media_list(mediaListPlayer_, mediaList);

    libvlc_media_list_player_set_media_player(mediaListPlayer_, libvlc_media_list_player_get_media_player(mediaListPlayer_));
   

    libvlc_media_list_player_play(mediaListPlayer_);
    return true;
}

void VLCPlayer::waitUntilPlaybackFinished() {
    while (!playbackFinished_) {
        usleep(100000); // Wait for 100 milliseconds
    }
}

std::string VLCPlayer::getCurrentMusicInfo() {
    std::string musicInfo;
    libvlc_media_player_t *mediaPlayer = libvlc_media_list_player_get_media_player(mediaListPlayer_);

        if (mediaPlayer) {
            
            libvlc_media_t *media = libvlc_media_player_get_media(mediaPlayer);
            if (media) {
                const char *mediaName = libvlc_media_get_meta(media, libvlc_meta_Title);
                if (mediaName) {
                    musicInfo += "Name: ";
                    musicInfo += mediaName;
                    musicInfo += "\n";
                }
            }
            libvlc_time_t currentTime = libvlc_media_player_get_time(mediaPlayer);
            int minutes = currentTime / 60000;
            int seconds = (currentTime % 60000) / 1000;

            char timeStr[10];
            snprintf(timeStr, sizeof(timeStr), "%02d:%02d", minutes, seconds);

            musicInfo += "Playback time: ";
            musicInfo += timeStr;
            musicInfo += "\n";
        }
    return musicInfo;
}


void VLCPlayer::handleMediaListPlayerPlayed(const libvlc_event_t *event, void *userData) {
    VLCPlayer *player = static_cast<VLCPlayer *>(userData);
    player->playbackFinished_ = true;
}

void VLCPlayer::handleMediaPlayerMediaChanged(const libvlc_event_t *event, void *userData) {
    VLCPlayer *player = static_cast<VLCPlayer *>(userData);
    std::cout << "event : MediaChanged" <<std::endl;
    libvlc_media_list_player_t *mediaListPlayer = player->mediaListPlayer_;
    libvlc_media_player_t *mediaPlayer = libvlc_media_list_player_get_media_player(mediaListPlayer);
    libvlc_media_t *media = libvlc_media_player_get_media(mediaPlayer);
    if (media) {
        const char *mediaName = libvlc_media_get_meta(media, libvlc_meta_Title);
        if (mediaName)
            std::cout << "Now playing: " << mediaName << std::endl;
    }
}

bool VLCPlayer::isPlaybackFinished() const {
    return playbackFinished_;
}
