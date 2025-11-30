#include "WAVTrack.h"
#include <iostream>

WAVTrack::WAVTrack(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth) {

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void WAVTrack::load() {
    std::cout << "[WAVTrack::load] Loading WAV: \"" << title
              << "\" at " << sample_rate 
              << "Hz/" << bit_depth << "bit (uncompressed)...\n";
              
    long long size = duration_seconds * sample_rate * (bit_depth / 8) * 2;

    std::cout << "  → Estimated file size: " << size << " bytes\n";
    std::cout << "  → Fast loading due to uncompressed format.\n";
}

void WAVTrack::analyze_beatgrid() {
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    float  beats = (duration_seconds / 60.0) * bpm;
    std::cout << "  → Estimated beats: " << (int) beats << "  → Precision factor: 1 (uncompressed audio)\n";

    // TODO: Implement WAV-specific beat detection analysis
    // Requirements:
    // 1. Print analysis message with track title
    // 2. Calculate beats: (duration_seconds / 60.0) * bpm
    // 3. Print number of beats and mention uncompressed precision
    // should print "  → Estimated beats: <beats>  → Precision factor: 1.0 (uncompressed audio)"
}

double WAVTrack::get_quality_score() const {

    const float BASE_SCORE = 70.0;
    const int CD_SAMPLE_RATE = 44100;
    const int HIGH_RES_SAMPLE_RATE = 96000;
    const int CD_BITDEPTH = 16;
    const int PRO_BITDEPTH = 24;

    const int CD_SAMPLE_RATE_BONUS = 10;
    const int HIGH_RES_SAMPLE_RATE_BONUS = 5;
    const int CD_BITDEPTH_BONUS = 10;
    const int PRO_BITDEPTH_BONUS = 5;

    double base_score = BASE_SCORE;
    if (sample_rate >= CD_SAMPLE_RATE){
        base_score += CD_SAMPLE_RATE_BONUS;
    }

    if (sample_rate >= HIGH_RES_SAMPLE_RATE){
        base_score += HIGH_RES_SAMPLE_RATE_BONUS;
    }
    
    if(bit_depth >= CD_BITDEPTH){
        base_score += CD_BITDEPTH_BONUS;
    }
    
    if(bit_depth >= PRO_BITDEPTH){
        base_score += PRO_BITDEPTH_BONUS;
    }

    return base_score;
}

PointerWrapper<AudioTrack> WAVTrack::clone() const {
    WAVTrack* clone = new WAVTrack(*this);
    return PointerWrapper<AudioTrack>(clone); // Replace with your implementation
}