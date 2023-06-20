#include "headers/Utilities.hpp"
#include "headers/Tonality.hpp"

Tonality::Tonality(int t, vector<int> m) {
    tonic = t % 12 + 12;    // bring it back to [12,21]
    mode = m;
    tonality_notes = getAllNotesFromTonality(tonic, mode);
}

int Tonality::get_tonic() {
    return tonic;
}

vector<int> Tonality::get_mode() {
    return mode;
}

IntSet Tonality::get_tonics() {
    return tonics;
}

IntSet Tonality::get_supertonics() {
    return supertonics;
}

IntSet Tonality::get_mediants() {
    return mediants;
}

IntSet Tonality::get_subdominants() {
    return subdominants;
}

IntSet Tonality::get_dominants() {
    return dominants;
}

IntSet Tonality::get_submediants() {
    return submediants;
}

IntSet Tonality::get_leading_tones() {
    return leading_tones;
}