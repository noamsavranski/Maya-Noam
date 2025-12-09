#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    if(cache.contains(track.get_title())){
        std::cout << "[INFO] Cache HIT for: " << track.get_title() << std::endl;
        cache.get(track.get_title());
        return 1;
    }
    else{
        PointerWrapper<AudioTrack> cloned = track.clone();
        if(!cloned){
            std::cerr << "[ERROR] Track: \"" << track.get_title()
                  << "\" failed to clone" << std::endl;
            return 0;
        }
        cloned->load();
        cloned->analyze_beatgrid();
        bool insert =cache.put(std::move(cloned));
        if(insert == true){
            return -1;
        }
        else{
            return 0;
        }
    }
    
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    AudioTrack* pointer =cache.get(track_title);
   if(!pointer){
    return nullptr;
   }
    return pointer;
}
