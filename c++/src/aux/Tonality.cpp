// Author: Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description: A class representing a tonality
//
#include "../../headers/aux/Tonality.hpp"


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
    name = noteNames[tonic] + " " + modeNames[mode];

    int note = tonic;
    degrees_notes[FIRST_DEGREE]                 = note;
    degrees_notes[FIVE_OF_FOUR]                 = note;
    degrees_notes[FIFTH_DEGREE_APPOGIATURA]     = note;
    degrees_notes[FLAT_TWO]                     = (note + MINOR_SECOND)              % PERFECT_OCTAVE;
    degrees_notes[SECOND_DEGREE]                = (note += scale[FIRST_DEGREE])      % PERFECT_OCTAVE;
    degrees_notes[FIVE_OF_FIVE]                 = note;
    degrees_notes[THIRD_DEGREE]                 = (note += scale[SECOND_DEGREE])     % PERFECT_OCTAVE;
    degrees_notes[FIVE_OF_SIX]                  = note;
    degrees_notes[FOURTH_DEGREE]                = (note += scale[THIRD_DEGREE])      % PERFECT_OCTAVE;
    degrees_notes[FIFTH_DEGREE]                 = (note += scale[FOURTH_DEGREE])     % PERFECT_OCTAVE;
    degrees_notes[SIXTH_DEGREE]                 = (note += scale[FIFTH_DEGREE])      % PERFECT_OCTAVE;
    degrees_notes[FIVE_OF_TWO]                  = note;
    degrees_notes[SEVENTH_DEGREE]               = (note += scale[SIXTH_DEGREE])      % PERFECT_OCTAVE;
    degrees_notes[FIVE_OF_THREE]                = note;
    degrees_notes[SEVEN_DIMINISHED]             = note;
    // todo add aug6th


    tonal_notes = {degrees_notes[FIRST_DEGREE], degrees_notes[FOURTH_DEGREE], degrees_notes[FIFTH_DEGREE]};
    /// Modal notes are the notes that change in major or minor scale (3,6,7 degrees). For simplicity for constraints,
    /// All notes that are not tonal are considered modal (useful for doubling)
    for(int i = 0; i < PERFECT_OCTAVE; i++){
        if(tonal_notes.find(i) == tonal_notes.end())
            modal_notes.insert(i);
    }
    /// The rest of the attributes are set by the child classes
}

string Tonality::get_name() const {
    return name;
}

/**
 * Get the tonic of the tonality
 * @return the tonic of the tonality
 */
int Tonality::get_tonic() const {
    return tonic;
}

/**
 * Get the mode of the tonality
 * @return the mode of the tonality
 */
int Tonality::get_mode() const{
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

/**
 * @return a string representing a tonality
 */
string Tonality::to_string(){
    string t;
    t += "Name: " + name + "\n";
    t += "Tonic: " + std::to_string(tonic) + "(" + noteNames[tonic] + ")\n";
    t += "Mode: " + std::to_string(mode) + "(" + modeNames[mode] + ")\n";
    t += "Scale: " + int_vector_to_string(scale) + "\n";

    t+= "Default chord qualities: ";
    for(int i = FIRST_DEGREE; i <= FLAT_TWO; i++)
        t += std::to_string(chord_qualities[i]) + "(" + chordQualityNames[chord_qualities[i]] + ") ";
    t += "\n";

    t+= "Degree notes: ";
    for(int i = FIRST_DEGREE; i <= FLAT_TWO; i++)
        t += std::to_string(degrees_notes[i]) + "(" + noteNames[degrees_notes[i]] + ") ";
    t += "\n";

    t += "Tonal notes: ";
    for (auto elem: tonal_notes)
        t += std::to_string(elem) + " ";

    return t;
}