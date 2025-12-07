#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}

DJLibraryService::DJLibraryService(const DJLibraryService& other)
    : playlist(other.playlist), library(){
        for (auto track : other.library){
            PointerWrapper<AudioTrack> clone  = track->clone();
            library.push_back(clone.release());
        }
    }
    
DJLibraryService::~DJLibraryService() {
    for(auto track : library) {
        delete track;
    }
}

DJLibraryService& DJLibraryService::operator=(const DJLibraryService& other){
    if(this != &other){
        playlist = other.playlist;
        for (auto track : library){
            delete track;
        }
        library.clear();
        for (auto track : other.library){
            PointerWrapper<AudioTrack> clone  = track->clone();
            library.push_back(clone.release());
        }
        
    }
    return *this;
}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    
    for (auto info : library_tracks){
        AudioTrack* new_track;
        if (info.type == "MP3"){
            new_track = new MP3Track(info.title,info.artists,info.duration_seconds,info.bpm,info.extra_param1,info.extra_param2);
            // std::cout << "MP3Track created:  "<< info.extra_param1 << "kbps.\n";
        }
        else {
            new_track = new WAVTrack(info.title,info.artists,info.duration_seconds,info.bpm,info.extra_param1,info.extra_param2);
            // std::cout << "WAVTrack created:  "<< info.extra_param1 << "Hz/" << info.extra_param2 << "bit\n" ;
        }
        library.push_back(new_track);
        
    }

    std::cout << "[INFO] Track library built: "<< library.size() << " tracks loaded\n" ;
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
    
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    std::cout << "[INFO] Loading playlist: " << playlist_name << "\n";
    Playlist new_playlist(playlist_name);
    playlist = new_playlist;
    
    for (int index : track_indices){
        if (index <= 0 || index > (int) library.size()){
            std::cout << "[WARNING] Invalid track index: " << index << "\n";
            continue;
        }
        PointerWrapper<AudioTrack> cloned_track = library[index-1]->clone();
        std::string title = library[index-1]->get_title();

        if(!cloned_track) {
            std::cerr << "[ERROR] Track:  " << title  << " failed to clone in DJLibraryService::loadPlaylistFromIndices";
            continue;
        }
        
        cloned_track->load();
        cloned_track->analyze_beatgrid();

        playlist.add_track(cloned_track.release());

        // std::cout << "Added "<< title << " to playlist  " << playlist_name << "\n";
    }
    std::cout << "[INFO] Playlist loaded: "<< playlist_name << " (" << playlist.get_track_count() << " tracks)\n";
    
}


/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {

    std::vector<std::string> titles_vector;
    auto playlist_tracks = playlist.getTracks();
    for (auto track : playlist_tracks){
        titles_vector.push_back(track->get_title());
    }
    
    return titles_vector;
}
