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
    //return 0; // Placeholder

    //ADDED HERE!!!!!!

    //HIT
    if(cache.contains(track.get_title())){
        cache.get(track.get_title());
        return 1;
    }

    //MISS
    PointerWrapper<AudioTrack> clonedPtr = track.clone();

    if(!clonedPtr){
        std::cerr << "[ERROR] Track: " << track.get_title() << " failed to clone\n";
        return 0; //what to return?
    }

    AudioTrack* clonedRawPtr = clonedPtr.release();
    clonedRawPtr->load();
    clonedRawPtr->analyze_beatgrid();

    PointerWrapper<AudioTrack> newPtr(clonedRawPtr);
    bool evicted = cache.put(std::move(newPtr));

    if(evicted){
        return -1;
    }
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
    // Your implementation here
    //return nullptr; // Placeholder

    //ADDED HERE!!!!!!
    AudioTrack* trackPtr = cache.get(track_title);
    if(!trackPtr){
        return nullptr;
    }

    return trackPtr;

}
