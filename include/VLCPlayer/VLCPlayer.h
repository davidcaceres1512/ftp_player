#ifndef VLC_PLAYER_H
#define VLC_PLAYER_H

#include <iostream>
#include <cstdio>
#include <vector>
#include <unistd.h>
#include <vlc/vlc.h>

class VLCPlayer {
public:
    VLCPlayer();
    ~VLCPlayer();

    bool initialize();
    bool playPlaylist(const std::vector<std::string>& playlist, const std::string& localDirectory);
    void waitUntilPlaybackFinished();
    bool isPlaybackFinished() const;
    std::string getCurrentMusicInfo();

private:
    libvlc_instance_t *vlcInstance_;
    libvlc_media_list_t *mediaList_;
    libvlc_media_list_player_t *mediaListPlayer_;
    bool playbackFinished_;

    static void handleMediaListPlayerPlayed(const libvlc_event_t *event, void *userData);
    static void handleMediaPlayerMediaChanged(const libvlc_event_t *event, void *userData);
    static void handleMediaMetaChanged(const libvlc_event_t *event, void *userData);
};

#endif // VLC_PLAYER_H
