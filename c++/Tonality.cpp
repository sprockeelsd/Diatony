#include "headers/Tonality.hpp"


/***********************************************************************************************************************
 *                                                  Tonality class methods                                             *
 ***********************************************************************************************************************/

/**
 * Constructor
 * @param t the tonic of the tonality
 * @param m the mode of the tonality
 * @param s the scale of the tonality
 */
Tonality::Tonality(int t, int m, vector<int> s) {
    tonic = t % PERFECT_OCTAVE + PERFECT_OCTAVE;    // bring it back to [12,21]
    mode = m;
    scale = s;

    // Set the list of degrees which are the notes of the scale
    int curr = tonic;
    for(int i = 0; i < scale.size(); ++i){
        degrees_notes[i] = curr;
        curr += scale[i];
    }

    // Create the dictionary of degrees and all occurrences of that notes
    for (int i = 0; i < degrees_notes.size(); ++i) {
        vector<int> temp = getAllGivenNote(degrees_notes[i]);
        if(i == SEVENTH_DEGREE && mode == MINOR_MODE){
            vector<int> temp2 = getAllGivenNote(degrees_notes[i] - 1); // to also get the flat seventh because it is used in third degree chord
            temp.insert(temp.end(), temp2.begin(), temp2.end()); // merge the 2 vectors together
        }
        IntSet set((const vector<int>) temp); // cast into IntSet
        scale_degrees[i] = set;
    }

    // Set tonal notes and modal notes
    tonal_notes = getAllNotesFromIntervalLoop(tonic, {PERFECT_FOURTH, MAJOR_SECOND,PERFECT_FOURTH}); // 1, 4 and 5 degrees
    // @todo modify so the flat seventh is also in that set
    modal_notes = getAllNotesFromIntervalLoop(get_degrees_notes()[THIRD_DEGREE], {PERFECT_FOURTH, MAJOR_SECOND, PERFECT_FOURTH}); // 3, 6 and 7 degrees

    // chord qualities and scale degrees chords are set in the child classes
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
int Tonality::get_mode(){
    return mode;
}

/**
 * Get the scale of the tonality
 * @return the scale of the tonality
 */
vector<int> Tonality::get_scale() {
    return scale;
}

/**
 * Get the notes corresponding to the degrees of the scale (first elem = tonic, second_elem = second degree, etc.)
 * @return a map containing the notes for each of the scale degrees
 */
map<int,int> Tonality::get_degrees_notes() {
    return degrees_notes;
}

/**
 * Get the note for a given degree
 * @param degree the degree of the scale [0,6]
 * @return the note for the given degree
 */
int Tonality::get_degree_note(int degree){
    return degrees_notes[degree];
}

/**
 * Get all the notes for each scale degree
 * @return a map of [degree, all_notes] for each degree of the scale (0 to 6)
 */
map<int, IntSet> Tonality::get_scale_degrees() {
    return scale_degrees;
}

/**
 * Get all the notes for a given scale degree
 * @param degree a degree of the scale [0,6]
 * @return an IntSet containing all the notes for the given scale degree
 */
IntSet Tonality::get_scale_degree(int degree) {
    return scale_degrees[degree];
}

/**
 * Get the notes that don't change in major or minor scale (1,4,5 degrees)
 * @return an IntSet containing the tonal notes
 */
IntSet Tonality::get_tonal_notes() {
    return tonal_notes;
}

/**
 * Get the notes that change in major or minor scale (3,6,7 degrees)
 * @return an IntSet containing the modal notes
 */
IntSet Tonality::get_modal_notes() {
    return modal_notes;
}