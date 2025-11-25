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
    
    PointerWrapper<AudioTrack> clone = track.clone();

    if(!clone) {
        std::cerr << "[ERROR] Track:  " << track.get_title()  << " failed to clone in DJControllerService::loadTrackToCache";
        return 0;
    }

    if(cache.contains(track.get_title())) {
        cache.get(track.get_title());
        return 1;
    }

    AudioTrack* uw_track = clone.release();

    uw_track->load();
    uw_track->analyze_beatgrid();

    PointerWrapper<AudioTrack> prepared_clone(uw_track);
    
    if (cache.put(std::move(prepared_clone))){
        // Eviction happened
        return -1;
    }

    // No eviction
    return 0; 
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
    return cache.get(track_title);
}
