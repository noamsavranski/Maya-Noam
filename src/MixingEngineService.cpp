#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks..." << std::endl;
    for(size_t i=0; i<2; i++){
        if(decks[i]){
            delete decks[i];
            decks[i] = nullptr;
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout << "[=== Loading Track to Deck ===]" << std::endl;

    PointerWrapper<AudioTrack> clone = track.clone();

    if (!clone) { 
        std::cout<<"[ERROR] Track: " << track.get_title() << "failed to clone" << std::endl;
        return -1; 
    }
     int target_deck = 1 - active_deck;
     std::cout << "[Deck Switch] Target deck: " << target_deck<< std::endl;

     if (decks[target_deck]){
        delete decks[target_deck];
        decks[target_deck]=nullptr;
     }

     clone->load();
     clone->analyze_beatgrid();

     if (decks[active_deck] && auto_sync) {
        if(!can_mix_tracks(clone)) {
            sync_bpm(clone);
        }
     }
     decks[target_deck] = clone.release();

     std::cout << "[Load Complete] " << decks[target_deck]->get_title() << " is now loaded on deck " << target_deck << std::endl;

     if (decks[active_deck]) {
        std::cout << "[Unload] Unloading previous deck " << active_deck << decks[active_deck]->get_title() << std::endl;
        delete decks[active_deck];
        decks[active_deck] = nullptr;
    }
    active_deck = target_deck;

    std::cout << "[Active Deck] Switched to deck " << target_deck << std::endl;

    std::cout << "\n=== Deck Status ===" << std::endl;

    std::cout << "Deck 0: ";
    if (decks[0]) std::cout << decks[0]->get_title();
    else std::cout << "[EMPTY]";
    std::cout << std::endl;

    std::cout << "Deck 1: ";
    if (decks[1]) std::cout << decks[1]->get_title();
    else std::cout << "[EMPTY]";
    std::cout << std::endl;

    std::cout << "Active Deck: " << active_deck << std::endl;
    std::cout << "===================" << std::endl;
     
    return target_deck;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    if(!decks[active_deck]){
        return false;
    }
    if(!track){
        return false;
    }
    int active = decks[active_deck]->get_bpm();
    int new_bpm = track->get_bpm();
    int dist =std::abs(active - new_bpm);
    if(dist > bpm_tolerance){
        return false;
    }
    return true;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if(decks[active_deck]){
        if(track){
            int new_bpm = track->get_bpm();
            int active = decks[active_deck]->get_bpm();
            int average= (active+new_bpm) / 2;
            track->set_bpm(average);
            std::cout << "[Sync BPM] Syncing BPM from " << new_bpm << " to " <<average <<std::endl;
        }
    }
}
