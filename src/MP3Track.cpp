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
              
    if(has_id3_tags) {
        std::cout << "  → Processing ID3 metadata (artist info, album art, etc.)...\n";
    } else { 
        std::cout << "  → No ID3 tags found.\n";
    }
    
    std::cout << "  → Decoding MP3 frames...\n";
    std::cout << "  → Load complete.\n";
    
}

void MP3Track::analyze_beatgrid() {

    std::cout << "[MP3Track::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    float beats = (duration_seconds / 60.0) * bpm;
    float precision_factor = bitrate / 320.0;
    std::cout << "  → Estimated beats: " << (int) beats << "  → Compression precision factor: " << precision_factor << "\n";
    

}

double MP3Track::get_quality_score() const {

    const double MAX_SCORE = 100.0;
    const double MIN_SCORE = 0.0;
    const float BASE_BITRATE = 320.0;
    const int MINIMUM_BITRATE = 128;
    const int ID3_TAGS_SCORE_BONUS = 5;
    const int BITRATE_SCORE_PENALTY = 10;

    double base_score = (bitrate / BASE_BITRATE) * MAX_SCORE;
    
    if (has_id3_tags){
        base_score += ID3_TAGS_SCORE_BONUS;
    }
    
    if (bitrate < MINIMUM_BITRATE){
        base_score -= BITRATE_SCORE_PENALTY;
    }

    // Clamp score between 0 to 100
    base_score = std::min(MAX_SCORE, std::max(MIN_SCORE, base_score));
    return base_score;
}

PointerWrapper<AudioTrack> MP3Track::clone() const {

    MP3Track* clone = new MP3Track(*this);
    return PointerWrapper<AudioTrack>(clone);
}