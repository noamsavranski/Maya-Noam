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
     for (size_t i = 0; i < library_tracks.size(); ++i) {
        const SessionConfig::TrackInfo& track_info = library_tracks[i];

        AudioTrack* new_track = nullptr; 

        if (track_info.type == "MP3") { 
            new_track = new MP3Track( track_info.title,track_info.artists,track_info.duration_seconds,track_info.bpm, track_info.extra_param1, track_info.extra_param2);
        } 
        else if (track_info.type == "WAV") { 
            new_track = new WAVTrack(track_info.title,track_info.artists,track_info.duration_seconds,track_info.bpm,track_info.extra_param1,track_info.extra_param2);
            
        }
        if (new_track) {
            library.push_back(new_track);
        }
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

    for (size_t i = 0; i < track_indices.size(); ++i) {
        int trackIndex = track_indices[i]; 
        if (trackIndex < 1 || trackIndex > static_cast<int>(library.size())) {
            std::cout << "[WARNING] Invalid track index: " << trackIndex << std::endl;
            continue;
        }
        size_t index = static_cast<size_t>(trackIndex - 1);

        AudioTrack* originalTrack = library[index];

        PointerWrapper<AudioTrack> clonedWrapper = originalTrack->clone();
        if (!clonedWrapper) {
            std::cout << "[Error] Failed to clone track: "
                      << originalTrack->get_title() << std::endl;
            continue;
        }
        clonedWrapper->load();
        clonedWrapper->analyze_beatgrid();

        AudioTrack* clonedTrack = clonedWrapper.release();
        currPlaylist.add_track(clonedTrack); 
    }

    std::cout << "[INFO] Playlist loaded: " << playlist_name
              << " (" << currPlaylist.get_track_count() << " tracks)" << std::endl;
    playlist = std::move(currPlaylist);

}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> trackTitles;
    std::vector<AudioTrack*> tracks = playlist.getTracks();
    for (const auto& track : tracks) {
        if (track) {
            trackTitles.push_back(track->get_title());
        }
    }

    return trackTitles;
}
                                               
                    
