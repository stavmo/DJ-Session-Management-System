#include "MixingEngineService.h"
#include <iostream>
#include <memory>

MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0) {
    decks[0] = nullptr;
    decks[1] = nullptr;


    std::cout << "[MixingEngineService] Initialized with 2 empty decks" << std::endl;
}

MixingEngineService::~MixingEngineService() {

    std::cout << "[MixingEngineService] Cleaning up decks.... " << std::endl;
    
    delete decks[0];
    decks[0] = nullptr;
    delete decks[1];
    decks[1] = nullptr;
}

MixingEngineService::MixingEngineService(const MixingEngineService& other):
    decks{nullptr,nullptr},
    active_deck(other.active_deck),
    auto_sync(other.auto_sync),
    bpm_tolerance(other.bpm_tolerance) {

    for(int i = 0; i < 2; i++) {
        if (other.decks[i]) {
            decks[i] = other.decks[i]->clone().release();
        }
    }
}

MixingEngineService& MixingEngineService::operator=(const MixingEngineService& other) {
    if (this == &other) {
        return *this;
    }
    for (int i =0; i < 2; i++) {
        delete decks[i];
        decks[i] = nullptr;
        if (other.decks[i]) {
             decks[i] = other.decks[i]->clone().release();
        }
    }
    active_deck = other.active_deck;
    auto_sync = other.auto_sync;
    bpm_tolerance = other.bpm_tolerance;
    return *this;
}


/**
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    PointerWrapper<AudioTrack> copy = track.clone();
    if (!copy) {
        std:: cout << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone\n";
        return -1;
    }

    std::cout <<  "\n=== Loading Track to Deck ===" << std::endl;

    size_t target = 1 - active_deck;
    std::cout << "[Deck Switch] Target deck:" << target << std::endl;
    
    //clear-up deck
    if (decks[target]) {
        delete decks[target];
        decks[target] = nullptr;
    }
    //load track to target deck
    copy->load();
    copy->analyze_beatgrid();

    //BPM management
    if (decks[active_deck] && auto_sync) {
        if (!can_mix_tracks(copy)) {
            sync_bpm(copy);
        }
    }

    //instant transition
    decks[target] = copy.release();  
    active_deck = target;
    std::cout << "[Active Deck] Switched to deck " << target << std::endl;
    return target;
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
 * Checks if two tracks can be mixed based on BPM difference.
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    if (!decks[active_deck] || !track) {
        return false;
    }

    int active_bpm = decks[active_deck]->get_bpm();
    int track_bpm = track->get_bpm();

    return std::abs(active_bpm - track_bpm) < bpm_tolerance;

}

/**
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if (decks[active_deck] && track) {
        int active_bpm = decks[active_deck]->get_bpm();
        int track_bpm = track->get_bpm();
        int avg = (track_bpm + active_bpm)/2;
        
        track->set_bpm(avg);
        std::cout << "[Sync BPM] Syncing BPM from " << track_bpm << " to " <<  avg << std::endl;
    }
}
