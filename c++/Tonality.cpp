#include "headers/Utilities.hpp"
#include "headers/Tonality.hpp"

Tonality::Tonality(int t, vector<int> m) {
    tonic = t % 12 + 12;    // bring it back to [12,21]
    mode = m;

    // Set the list of degrees which are the notes of the scale
    int curr = tonic;
    degrees_notes.push_back(curr);
    for (int i = 0; i < mode.size()-1; ++i) {
        curr += mode[i];
        degrees_notes.push_back(curr);
    }

    // Set the list of all the notes in the tonality
    tonality_notes = getAllNotesFromTonality(tonic, mode);

    // Create the dictionnary of degrees and all occurences of that notes
    for (int i = 0; i < degrees_notes.size(); ++i) {
        scale_degrees[i+1] = getAllGivenNote(degrees_notes[i]);
    }

    // Set tonal notes and modal notes
    tonal_notes = getAllNotesFromIntervalLoop(tonic, {perfectFourth, majorSecond,perfectFourth}); // 1, 4 and 5 degrees
    modal_notes = getAllNotesFromIntervalLoop(get_degrees_notes()[2], {perfectFourth, majorSecond, perfectFourth}); // 3, 6 and 7 degrees

    // set the chords domains for each degree
}

int Tonality::get_tonic() {
    return tonic;
}

vector<int> Tonality::get_mode() {
    return mode;
}

vector<int> Tonality::get_degrees_notes() {
    return degrees_notes;
}

IntSet Tonality::get_tonality_notes() {
    return tonality_notes;
}

map<int, IntSet> Tonality::get_scale_degrees() {
    return scale_degrees;
}

IntSet Tonality::get_scale_degree(int degree) {
    return scale_degrees[degree];
}

IntSet Tonality::get_tonal_notes() {
    return tonal_notes;
}

IntSet Tonality::get_modal_notes() {
    return modal_notes;
}

map<int, IntSet> Tonality::get_scale_degrees_chords(){ // @todo maybe move to child classes
    return scale_degrees_chords;
}