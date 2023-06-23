#include "headers/Utilities.hpp"
#include "headers/Tonality.hpp"


/***********************************************************************************************************************
 *                                                  Tonality class methods                                             *
 ***********************************************************************************************************************/

/**
 * Constructor
 * @param t the tonic of the tonality
 * @param m the mode of the tonality
 */
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

    // Create the dictionary of degrees and all occurrences of that notes
    for (int i = 0; i < degrees_notes.size(); ++i) {
        scale_degrees[i+1] = getAllGivenNote(degrees_notes[i]);
    }

    // Set tonal notes and modal notes
    tonal_notes = getAllNotesFromIntervalLoop(tonic, {perfectFourth, majorSecond,perfectFourth}); // 1, 4 and 5 degrees
    modal_notes = getAllNotesFromIntervalLoop(get_degrees_notes()[2], {perfectFourth, majorSecond, perfectFourth}); // 3, 6 and 7 degrees
}

/**
 * Get the tonic of the tonality
 * @return the tonic of the tonality
 */
int Tonality::get_tonic() {
    return tonic;
}

/**
 * Get the mode of the tonality
 * @return the mode of the tonality
 */
vector<int> Tonality::get_mode() {
    return mode;
}

/**
 * Get the notes corresponding to the degrees of the scale (first elem = tonic, second_elem = second degree, etc.)
 * @return a vector containing the notes for each of the scale degrees
 */
vector<int> Tonality::get_degrees_notes() {
    return degrees_notes;
}

/**
 * Get the chord quality for each degree
 * @return a map of [degree, chord_quality] for each degree of the scale (1 to 7)
 */
map<int, vector<int>> Tonality::get_chord_qualities(){
    return chord_qualities;
}

/**
 * Get all the notes in the tonality
 * @return an IntSet containing all the notes in the tonality
 */
IntSet Tonality::get_tonality_notes() {
    return tonality_notes;
}

/**
 * Get all the notes for each scale degree
 * @return a map of [degree, all_notes] for each degree of the scale (1 to 7)
 */
map<int, IntSet> Tonality::get_scale_degrees() {
    return scale_degrees;
}

/**
 * Get all the notes for a given scale degree
 * @param degree a degree of the scale [1,7]
 * @return an IntSet containing all the notes for the given scale degree
 */
IntSet Tonality::get_scale_degree(int degree) {
    return scale_degrees[degree];
}

/**
 * Get the notes that don't change in major or minor mode (1,4,5 degrees)
 * @return an IntSet containing the tonal notes
 */
IntSet Tonality::get_tonal_notes() {
    return tonal_notes;
}

/**
 * Get the notes that change in major or minor mode (3,6,7 degrees)
 * @return an IntSet containing the modal notes
 */
IntSet Tonality::get_modal_notes() {
    return modal_notes;
}

/**
 * Get the chord notes for each degree
 * @return a map of [degree, chord] for each degree of the scale (1 to 7)
 */
map<int, IntSet> Tonality::get_scale_degrees_chords(){ // @todo maybe move to child classes
    return scale_degrees_chords;
}

/**
 * Get the chord notes for a given degree
 * @param degree a degree of the scale [1,7]
 * @return an IntSet containing the chord notes for the given degree
 */
IntSet Tonality::get_scale_degree_chord(int degree){
    return scale_degrees_chords[degree];
}

/**
 * Creates a string representing the tonality object
 * @return a string representing the tonality object
 */
string Tonality::to_string() const{
    return "";
}

/**
 * Overload of the << operator for Tonality objects
 */
//std::ostream& operator<<(std::ostream& os, const Tonality& t){
//    os << t.to_string();
//    return os;
//}