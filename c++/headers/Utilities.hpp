#ifndef UTILITIES
#define UTILITIES

#include <gecode/int.hh>
#include <gecode/minimodel.hh>

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;
using namespace Gecode;

/***********************************************************************************************************************
 *                                                  Useful constants                                                   *
 ***********************************************************************************************************************/

/** Types of search engines */
enum solver_types{
    DFS_SOLVER, //0
    BAB_SOLVER, //1
    LDS_SOLVER  //2
};

/** Notes */
const int B_SHARP = 0;
const int C = 0;
const int C_SHARP = 1;
const int D_FLAT = 1;
const int D = 2;
const int D_SHARP = 3;
const int E_FLAT = 3;
const int E = 4;
const int F_FLAT = 4;
const int E_SHARP = 5;
const int F = 5;
const int F_SHARP = 6;
const int G_FLAT = 6;
const int G = 7;
const int G_SHARP = 8;
const int A_FLAT = 8;
const int A = 9;
const int A_SHARP = 10;
const int B_FLAT = 10;
const int B = 11;
const int C_FLAT = 11;

const vector<std::string> noteNames = {"C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"}; // @todo turn into a bi-directional map

enum voices{
    BASS,       //0
    TENOR,      //1
    ALTO,       //2
    SOPRANO     //3
};

enum degrees{
    FIRST_DEGREE,       //0
    SECOND_DEGREE,      //1
    THIRD_DEGREE,       //2
    FOURTH_DEGREE,      //3
    FIFTH_DEGREE,       //4
    SIXTH_DEGREE,       //5
    SEVENTH_DEGREE      //6
};

const vector<std::string> degreeNames = {"First degree", "Second degree", "Third degree", "Fourth degree", "Fifth degree",
                                         "Sixth degree", "Seventh degree"};

/** Intervals */
// "classic" intervals
enum intervals{
    UNISSON,            //0
    MINOR_SECOND,       //1
    MAJOR_SECOND,       //2
    MINOR_THIRD,        //3
    MAJOR_THIRD,        //4
    PERFECT_FOURTH,     //5
    TRITONE,            //6
    PERFECT_FIFTH,      //7
    MINOR_SIXTH,        //8
    MAJOR_SIXTH,        //9
    MINOR_SEVENTH,      //10
    MAJOR_SEVENTH,      //11
    PERFECT_OCTAVE      //12
};

// augmented/diminished intervals
const int AUGMENTED_SECOND = 3;

/** Chords */
// Types of chords represented by the intervals between their notes in root position up to an octave
const vector<int> MAJOR_CHORD = {MAJOR_THIRD, MINOR_THIRD, PERFECT_FOURTH};
const vector<int> MINOR_CHORD = {MINOR_THIRD, MAJOR_THIRD, PERFECT_FOURTH};
const vector<int> DIMINISHED_CHORD = {MINOR_THIRD, MINOR_THIRD, TRITONE};
const vector<int> AUGMENTED_CHORD = {MAJOR_THIRD, MAJOR_THIRD, MAJOR_THIRD};
const vector<int> DOMINANT_SEVENTH_CHORD = {MAJOR_THIRD, MINOR_THIRD, MINOR_THIRD, MAJOR_SECOND};
const vector<int> MAJOR_SEVENTH_CHORD = {MAJOR_THIRD, MINOR_THIRD, MAJOR_THIRD, MINOR_SECOND};
const vector<int> MINOR_SEVENTH_CHORD = {MINOR_THIRD, MAJOR_THIRD, MINOR_THIRD, MAJOR_SECOND};

// Chord states
enum chordStates{
    FUNDAMENTAL_STATE,  //0
    FIRST_INVERSION,    //1
    SECOND_INVERSION,   //2
    THIRD_INVERSION     //3
};

const vector<std::string> stateNames = {"Fundamental state", "First inversion", "Second inversion", "Third inversion"};

/** Modes */
// syntactic sugar for more commonly used modes
const int MAJOR_MODE = 0;
const int MINOR_MODE = 5;   // to correspond to the enum Mode

enum Mode {
    IONIAN,     //0 , major mode
    DORIAN,     //1
    PHRYGIAN,   //2
    LYDIAN,     //3
    MIXOLYDIAN, //4
    AEOLIAN,    //5 , natural minor mode
    LOCRIAN     //6
};

const vector<std::string> modeNames = {"Major", "Dorian", "Phrygian", "Lydian", "Mixolydian", "Minor", "Locrian"};

/** Scales */
/// defined by the intervals between their notes

//@todo turn this into a dictionary with the name of the scale as key and the vector of intervals as value

const vector<int> MAJOR_SCALE = {MAJOR_SECOND, MAJOR_SECOND, MINOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MINOR_SECOND};
const vector<int> NATURAL_MINOR_SCALE = {MAJOR_SECOND, MINOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MINOR_SECOND, MAJOR_SECOND, MAJOR_SECOND};
const vector<int> HARMONIC_MINOR_SCALE = {MAJOR_SECOND, MINOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MINOR_SECOND, AUGMENTED_SECOND, MINOR_SECOND};
const vector<int> MELODIC_MINOR_SCALE = {MAJOR_SECOND, MINOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MINOR_SECOND};

/***********************************************************************************************************************
 *                                                      Functions                                                      *
 ***********************************************************************************************************************/

/**
 * For a given set of intervals between notes that loops and a starting note, returns all the possible notes
 * @param note the starting note
 * @param intervals the set of intervals between notes. It must make a loop. For example, to get all notes from a major
 * scale from note, use {2, 2, 1, 2, 2, 2, 1}. To get all notes from a minor chord, use {3, 4, 5}.
 * @return vector<int> all the notes
 */
vector<int> get_all_notes_from_interval_loop(int n, vector<int> intervals);

/**
 * For a given tonality (root + mode), returns all the possible notes
 * @param root the root of the tonality (in [0,11])
 * @param scale the set of tones and semitones that define the scale
 * @return vector<int> all the possible notes from that tonality
 */
vector<int> get_all_notes_from_scale(int root, vector<int> scale);

/**
 * For a given chord (root + mode), returns all the possible notes
 * @param root the root of the chord
 * @param quality the set of tones and semitones that define the chord
 * @return vector<int> all the possible notes from that chord
 */
vector<int> get_all_notes_from_chord(int root, vector<int> quality);

/**
 * Get all values for a given note
 * @param note a note
 * @return vector<int> a vector containing all the given notes
 */
vector<int> get_all_given_note(int note);

/**
 * Transforms an int* into a vector<int>
 * @param ptr an int* pointer
 * @param size the size of the array
 * @return a vector<int> containing the same values as the array
 */
vector<int> int_pointer_to_vector(int* ptr, int size);



/**
 * Transforms a vector of integers into a string
 * @param vector a vector of integers
 * @return string the string representation of the vector
 */
string int_vector_to_string(vector<int> vector);

/**
 * Prints the Search::Statistics object into a readable format
 * @param stats a Search::Statistics object representing the statistics of a search
 * @return The string representation of the statistics object
 */
string statistics_to_string(Search::Statistics stats);

/**
 * Returns the value of a variable as a string. If the variable is unassigned, returns "<unassigned>"
 * @param var an integer variable
 * @return a string representing the value of the variable
 */
string intVar_to_string(IntVar var);

/**
 * Returns the values of an array of variables as a string. Calls the intVar_to_string function
 * @param vars an array of integer variables
 * @return a string representing the values of the variables
 */
string intVarArray_to_string(IntVarArray vars);

/**
 * Returns the name of a note based on its MIDI value
 * @param note an integer
 */
string midi_to_letter(int note);

/**
 * Returns the name of a mode based on its integer value
 * @param mode an integer
 * @return a string representing the name of the mode
 */
string mode_int_to_name(int mode);

/**
 * returns a string with the time
 * @return a string with the time
 */
string time();

/**
 * Write a text into a log file
 * @param message the text to write
 */
void write_to_log_file(const char* message);

#endif
