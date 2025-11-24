#include "MP3Track.h"
#include <iostream>
#include <cmath>
#include <algorithm>

MP3Track::MP3Track(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int bitrate, bool has_tags)
    : AudioTrack(title, artists, duration, bpm), bitrate(bitrate), has_id3_tags(has_tags) {

    std::cout << "MP3Track created: " << bitrate << " kbps" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void MP3Track::load() {
    std::cout << "[MP3Track::load] Loading MP3: \"" << title
              << "\" at " << bitrate << " kbps...\n";
    // TODO: Implement MP3 loading with format-specific operations
    // NOTE: Use exactly 2 spaces before the arrow (→) character


    
    //!!!!!! ADDED HERE
    if(has_id3_tags){
        std::cout <<"  → Processing ID3 metadata (artist info, album art, etc.)..."<< std::endl;
    }
    else{
        std::cout <<"  → No ID3 tags found."<< std::endl;
    }

    std::cout <<"  → Decoding MP3 frames..."<< std::endl;
    std::cout <<"  → Load complete."<< std::endl;
    
}

void MP3Track::analyze_beatgrid() {
     std::cout << "[MP3Track::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    // TODO: Implement MP3-specific beat detection analysis
    // NOTE: Use exactly 2 spaces before each arrow (→) character

    //!!!!!! ADDED HERE
    int beats_estimated = (duration_seconds / 60.0) * bpm;
    double precision_factor = bitrate / 320.0;

    std::cout <<"  → Estimated beats:"<< beats_estimated << "  → Compression precision factor:" << precision_factor << std::endl;


}

double MP3Track::get_quality_score() const {
    // TODO: Implement comprehensive quality scoring
    // NOTE: This method does NOT print anything

    //return 0.0; // Replace with your implementation

    //!!!!!! ADDED HERE
    double score = (bitrate / 320.0) * 100.0;
    if(has_id3_tags){
        score += 5;
    }

    if(bitrate < 128){
        score -= 10;
    }

    if (score < 0.0) return 0.0;
    if (score > 100.0) return 100.0;

    return score;
}

PointerWrapper<AudioTrack> MP3Track::clone() const {
    // TODO: Implement polymorphic cloning
    //return PointerWrapper<AudioTrack>(nullptr); // Replace with your implementation

    //!!!!!! ADDED HERE
    return PointerWrapper<AudioTrack>(new MP3Track(*this));
}