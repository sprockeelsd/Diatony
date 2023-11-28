#include "../headers/Tonality.hpp"


/***********************************************************************************************************************
 *                                                  Tonality class methods                                             *
 ***********************************************************************************************************************/

/**
 * Constructor
 * Creates a tonality object based on the tonality (tonic and mode)
 * @param t the tonic of the tonality
 * @param m the mode of the tonality
 * @param s the scale of the tonality
 */
Tonality::Tonality(int t, int m, vector<int> s) {
    tonic = t % PERFECT_OCTAVE;    // bring it back to [0,11]
    mode = m;
    scale = s;

    /// Set the list of degrees on which chords are built, based on the tonic and the scale
    int curr = tonic;
    for(int i = 0; i < scale.size(); ++i){
        degrees_notes[i] = curr % PERFECT_OCTAVE;
        curr += scale[i];
    }

    /// Create the dictionary of degrees and all possible values of that (those) note(s)
    for (int i = 0; i < degrees_notes.size(); ++i) {
        vector<int> notes = get_all_given_note(degrees_notes[i]);          //vector to store all notes for a given degree
        if(i == SEVENTH_DEGREE && mode == MINOR_MODE){ // @todo move this to minor tonality somehow
            // @todo maybe add sharp sixth as well? for now not necessary
            vector<int> additional_notes = get_all_given_note(degrees_notes[i] - 1); // to also get the flat seventh because it is used in third degree chord
            notes.insert(notes.end(), additional_notes.begin(), additional_notes.end()); // merge the 2 vectors together
        }
        IntSet set((const vector<int>) notes); // cast into IntSet
        scale_degrees[i] = set;
    }

    /// Set tonal notes and modal notes
    tonal_notes = {degrees_notes[FIRST_DEGREE], degrees_notes[FOURTH_DEGREE], degrees_notes[FIFTH_DEGREE]}; // 1, 4 and 5 degrees
    modal_notes = {degrees_notes[THIRD_DEGREE], degrees_notes[SIXTH_DEGREE], degrees_notes[SEVENTH_DEGREE]}; // 3, 6 and 7 degrees
    if (mode == MINOR_MODE)
        modal_notes.insert(degrees_notes[SEVENTH_DEGREE] - 1); // add the minor seventh if minor mode


    /// chord qualities and scale degrees chords are set in the child classes
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
 * Get the notes corresponding to the degrees of the scale on which chords are built
 * @return a map containing the notes for each of the scale degrees
 */
map<int,int> Tonality::get_degrees_notes() {
    return degrees_notes;
}

/**
 * Get the note on which the chord is built for a given degree
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
set<int> Tonality::get_tonal_notes() {
    return tonal_notes;
}

/**
 * Get the notes that change in major or minor scale (3,6,7 degrees)
 * @return an IntSet containing the modal notes
 */
set<int> Tonality::get_modal_notes() {
    return modal_notes;
}