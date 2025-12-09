#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
 DJLibraryService::~DJLibraryService() {
    for (AudioTrack* track : library) {
        delete track;
    }
    library.clear();
}
DJLibraryService::DJLibraryService(DJLibraryService&& other) noexcept 
    : playlist(std::move(other.playlist)),
      library(std::move(other.library)) { }
     
      
DJLibraryService& DJLibraryService::operator=(DJLibraryService&& other) noexcept {
    if (this != &other) {
        for (AudioTrack* track : library) {
            delete track;
        }
        library.clear();
        playlist = std::move(other.playlist);
        library = std::move(other.library);
    }
    return *this;
}
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
     for (size_t i = 0; i < library_tracks.size(); i++) {
        const SessionConfig::TrackInfo& trackInfo = library_tracks[i];
        AudioTrack* track = nullptr;

        if (trackInfo.type == "MP3") {
            track = new MP3Track(trackInfo.title, trackInfo.artists, trackInfo.duration_seconds, trackInfo.bpm, trackInfo.extra_param1);
        }
        else if (trackInfo.type == "WAV") {
            track = new WAVTrack(trackInfo.title, trackInfo.artists, trackInfo.duration_seconds, trackInfo.bpm, trackInfo.extra_param1, trackInfo.extra_param2);
        } 
    library.push_back(track);
    }
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded" << std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, const std::vector<int>& track_indices) {
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    Playlist currPlaylist(playlist_name);
    
    size_t tracksAdded = 0;
    
    for (size_t i = 0; i < track_indices.size(); i++) {
        int trackIndex = track_indices[i];
        size_t index = track_indices[i] - 1; // האינדקס בספרייה הוא 0-מבוסס

        if (index >= library.size()) {
            std::cout << "[WARNING] Invalid track index: " << trackIndex << std::endl;
            continue;
        }

        AudioTrack* originalTrack = library[index];
        PointerWrapper<AudioTrack> clonedWrapper = originalTrack->clone();
        AudioTrack* clonedTrack = clonedWrapper.release();
        if (!clonedTrack) {
            std::cout << "[Error] Failed to clone track: " << originalTrack->get_title() << std::endl;
            continue;
        }

        currPlaylist.add_track(clonedTrack);
        tracksAdded++;
        playlist = std::move(currPlaylist);
    }
    (void)playlist_name; 
    (void)track_indices;  
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> trackTitles;
    std::vector<AudioTrack*> tracks = playlist.getTracks();
    for (size_t i = 0; i < tracks.size(); i++) {
    AudioTrack* track = tracks[i];
    trackTitles.push_back(track->get_title());
    }
    return trackTitles;
}
                                               
                    
