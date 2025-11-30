#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks{nullptr,nullptr}, active_deck(0), auto_sync(false), bpm_tolerance(0)
{
    std::cout << "[MixingEngineService] Initialized with 2 empty decks. \n";
}

MixingEngineService::MixingEngineService(const MixingEngineService& other)
    : decks{nullptr,nullptr}, active_deck(other.active_deck), auto_sync(other.auto_sync), bpm_tolerance(other.bpm_tolerance)
{
    if(other.decks[0]) { decks[0] = other.decks[0]->clone().release();}

    if(other.decks[1]) { decks[1] = other.decks[1]->clone().release();}
}

MixingEngineService& MixingEngineService::operator=(const MixingEngineService& other) {
    if(this == &other) { return *this; }

    for (AudioTrack*& deck : decks) {
        delete deck;
        deck = nullptr;
    }

    active_deck = other.active_deck;
    auto_sync = other.auto_sync;
    bpm_tolerance = other.bpm_tolerance;

    if(other.decks[0]) { decks[0] = other.decks[0]->clone().release();}

    if(other.decks[1]) { decks[1] = other.decks[1]->clone().release();}

    return *this;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {

    std::cout << "[MixingEngineService] Cleaning up decks.... \n";
    for (AudioTrack* deck : decks){
        delete deck;
        deck = nullptr;
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout << "\n=== Loading Track to Deck ===\n";

    PointerWrapper<AudioTrack> cloned_track = track.clone();
    
    if(!cloned_track) {
        std::cerr << "[ERROR] Track:  " << track.get_title()  << " failed to clone in MixingEngineService :: loadTrackToDeck";
        return -1;
    }

    cloned_track->analyze_beatgrid();
    cloned_track->load();

    if(!decks[0] && !decks[1]){
        decks[0] = cloned_track.release();
        return 0;
    }

    size_t target_deck = 1 - active_deck;
    std::cout << "[Deck Switch] Target deck: "<< target_deck <<"\n";
    if (decks[target_deck]){
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }

    if(auto_sync && !can_mix_tracks(cloned_track)){
        sync_bpm(cloned_track);
    }

    decks[target_deck] = cloned_track.release();
    std::cout << "[Load Complete] ’" << decks[target_deck]->get_title() << "’ is now loaded on deck "<< target_deck <<"\n"; 
    std::cout << "[Unload] Unloading previous deck " << active_deck << " (" << decks[active_deck]->get_title() <<")\n";
    delete decks[active_deck];
    decks[active_deck] = nullptr;
    active_deck = target_deck;
    std::cout << "[Active Deck] Switched to deck "<< target_deck <<"\n";
     
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
    
    if(!decks[active_deck]) { return false; }

    if(!track) { return false; }

    int active_deck_bpm = decks[active_deck]->get_bpm();
    int new_track_bpm = track->get_bpm();

    int bpm_difference = active_deck_bpm - new_track_bpm;

    if(bpm_difference < 0) {
        bpm_difference = -bpm_difference;
    }

    return bpm_difference <= bpm_tolerance;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    
    if(!decks[active_deck]) { return; }

    if(!track) { return; }

    int original_bpm = track->get_bpm();
    int average_bpm = (original_bpm + decks[active_deck]->get_bpm()) / 2 ;
    track->set_bpm(average_bpm);
}
