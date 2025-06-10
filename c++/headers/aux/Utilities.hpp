// Author: Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description: A file containing useful functions and constants for the symbolic musical problems
//
#ifndef UTILITIES
#define UTILITIES

#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "gecode/int.hh"
#include "gecode/kernel.hh"
// ReSharper disable once CppUnusedIncludeDirective
#include "gecode/minimodel.hh"
#include "gecode/search.hh"

using namespace std;
using namespace Gecode;

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                  Useful constants                                                   *
 *                                                                                                                     *
 ***********************************************************************************************************************/

// TODO remove useless stuff

/** Files */
const string LOG_FILE = "log.txt";

/** Types of search engines */
enum solver_types{
    DFS_SOLVER, //0
    BAB_SOLVER, //1
    LDS_SOLVER  //2
};

/** Voice ranges */
constexpr int BASS_MIN = 40;
constexpr int BASS_MAX = 60;
constexpr int TENOR_MIN = 48;
constexpr int TENOR_MAX = 69;
constexpr int ALTO_MIN = 55;
constexpr int ALTO_MAX = 75;
constexpr int SOPRANO_MIN = 60;
constexpr int SOPRANO_MAX = 84;

/** Melodic costs */
constexpr int UNISON_COST = 0;
constexpr int SECOND_COST = 1;
constexpr int THIRD_COST = 3;
constexpr int FOURTH_COST = 6;
constexpr int TRITONE_COST = 6;
constexpr int FIFTH_COST = 6;
constexpr int SIXTH_COST = 12;
constexpr int SEVENTH_COST = 18;
constexpr int OCTAVE_COST = 6;

constexpr int MAX_MELODIC_COST = SEVENTH_COST;

/** Notes */
constexpr int B_SHARP = 0;
constexpr int C = 0;
constexpr int C_SHARP = 1;
constexpr int D_FLAT = 1;
constexpr int D = 2;
constexpr int D_SHARP = 3;
constexpr int E_FLAT = 3;
constexpr int E = 4;
constexpr int F_FLAT = 4;
constexpr int E_SHARP = 5;
constexpr int F = 5;
constexpr int F_SHARP = 6;
constexpr int G_FLAT = 6;
constexpr int G = 7;
constexpr int G_SHARP = 8;
constexpr int A_FLAT = 8;
constexpr int A = 9;
constexpr int A_SHARP = 10;
constexpr int B_FLAT = 10;
constexpr int B = 11;
constexpr int C_FLAT = 11;

const vector<std::string> noteNames = {"C", "Csharp", "D", "Eb", "E", "F", "Fsharp", "G", "Ab", "A", "Bb", "B"};

/** Voice positions */
enum voices{
    BASS,       //0
    TENOR,      //1
    ALTO,       //2
    SOPRANO     //3
};

const vector<std::string> voiceNames = {"Bass", "Tenor", "Alto", "Soprano"};

/** scale degrees */ //todo maybe rename the last ones better
enum degrees{
    FIRST_DEGREE,               //0
    SECOND_DEGREE,              //1
    THIRD_DEGREE,               //2
    FOURTH_DEGREE,              //3
    FIFTH_DEGREE,               //4
    SIXTH_DEGREE,               //5
    SEVENTH_DEGREE,             //6
    FIFTH_DEGREE_APPOGIATURA,   //7
    FIVE_OF_TWO,                //8
    FIVE_OF_THREE,              //9
    FIVE_OF_FOUR,               //10
    FIVE_OF_FIVE,               //11
    FIVE_OF_SIX,                //12
    FIVE_OF_SEVEN,              //13
    FLAT_TWO,                   //14
    AUGMENTED_SIXTH,            //15
};

const vector<std::string> degreeNames = {"I", "II", "III", "IV", "V", "VI", "VII",
                                         "Vda",
                                         "V/II", "V/III", "V/IV", "V/V", "V/VI",
                                         "V/VII","bII", "6te_aug"};

enum cadences{
    PERFECT_CADENCE,    //0
    PLAGAL_CADENCE,     //1
    HALF_CADENCE,       //3
    DECEPTIVE_CADENCE   //4
};

/** Intervals */
// "classic" intervals
enum intervals{
    UNISON,            //0
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
constexpr int AUGMENTED_SECOND = 3;
constexpr int DIMINISHED_THIRD = 2;
constexpr int AUGMENTED_FOURTH = TRITONE;
constexpr int DIMINISHED_FIFTH = TRITONE;

/** Chords */
enum chordTypes{
    MAJOR_CHORD,                //0
    MINOR_CHORD,                //1
    DIMINISHED_CHORD,           //2
    AUGMENTED_CHORD,            //3
    AUGMENTED_SIXTH_CHORD,      //4
    DOMINANT_SEVENTH_CHORD,     //5
    MAJOR_SEVENTH_CHORD,        //6
    MINOR_SEVENTH_CHORD,        //7
    DIMINISHED_SEVENTH_CHORD,   //8
    HALF_DIMINISHED_CHORD,      //9
    MINOR_MAJOR_SEVENTH_CHORD,  //10
    MAJOR_NINTH_DOMINANT_CHORD,//11
    MINOR_NINTH_DOMINANT_CHORD //12
};

const vector<std::string> chordQualityNames = {"Major", "Minor", "Diminished", "Augmented","Augmented sixth",
                                                    "Dominant seventh", "Major seventh", "Minor seventh",
                                                    "Diminished seventh", "Half diminished chord",
                                                    "Minor major seventh"};

/// Types of chords represented by the intervals between their notes in root position up to an octave
const vector<int> MAJOR_CHORD_INTERVALS                         = {MAJOR_THIRD, MINOR_THIRD};
const vector<int> MINOR_CHORD_INTERVALS                         = {MINOR_THIRD, MAJOR_THIRD};
const vector<int> DIMINISHED_CHORD_INTERVALS                    = {MINOR_THIRD, MINOR_THIRD};
const vector<int> AUGMENTED_CHORD_INTERVALS                     = {MAJOR_THIRD, MAJOR_THIRD};
const vector<int> AUGMENTED_SIXTH_CHORD_INTERVALS               = {MAJOR_THIRD, AUGMENTED_FOURTH};
const vector<int> DOMINANT_SEVENTH_CHORD_INTERVALS              = {MAJOR_THIRD, MINOR_THIRD, MINOR_THIRD};
const vector<int> MAJOR_SEVENTH_CHORD_INTERVALS                 = {MAJOR_THIRD, MINOR_THIRD, MAJOR_THIRD};
const vector<int> MINOR_SEVENTH_CHORD_INTERVALS                 = {MINOR_THIRD, MAJOR_THIRD, MINOR_THIRD};
const vector<int> DIMINISHED_SEVENTH_CHORD_INTERVALS            = {MINOR_THIRD, MINOR_THIRD, MINOR_THIRD};
const vector<int> HALF_DIMINISHED_CHORD_INTERVALS               = {MINOR_THIRD, MINOR_THIRD, MAJOR_THIRD};
const vector<int> MINOR_MAJOR_SEVENTH_CHORD_INTERVALS           = {MINOR_THIRD, MAJOR_THIRD, MAJOR_THIRD};
/// todo There are three types of augmented sixth chords: Italian, French and German. For now, only italian is implemented
const vector<int> MAJOR_NINTH_DOMINANT_CHORD_INTERVALS         = {MAJOR_THIRD, MINOR_THIRD, MINOR_THIRD, MAJOR_THIRD};
const vector<int> MINOR_NINTH_DOMINANT_CHORD_INTERVALS         = {MAJOR_THIRD, MINOR_THIRD, MINOR_THIRD, MINOR_THIRD};

const map<int, vector<int>> chordQualitiesIntervals = {
        {MAJOR_CHORD,                   MAJOR_CHORD_INTERVALS},
        {MINOR_CHORD,                   MINOR_CHORD_INTERVALS},
        {DIMINISHED_CHORD,              DIMINISHED_CHORD_INTERVALS},
        {AUGMENTED_CHORD,               AUGMENTED_CHORD_INTERVALS},
        {AUGMENTED_SIXTH_CHORD,         AUGMENTED_SIXTH_CHORD_INTERVALS},
        {DOMINANT_SEVENTH_CHORD,        DOMINANT_SEVENTH_CHORD_INTERVALS},
        {MAJOR_SEVENTH_CHORD,           MAJOR_SEVENTH_CHORD_INTERVALS},
        {MINOR_SEVENTH_CHORD,           MINOR_SEVENTH_CHORD_INTERVALS},
        {DIMINISHED_SEVENTH_CHORD,      DIMINISHED_SEVENTH_CHORD_INTERVALS},
        {HALF_DIMINISHED_CHORD,         HALF_DIMINISHED_CHORD_INTERVALS},
        {MINOR_MAJOR_SEVENTH_CHORD,     MINOR_MAJOR_SEVENTH_CHORD_INTERVALS},
        {MAJOR_NINTH_DOMINANT_CHORD,   MAJOR_NINTH_DOMINANT_CHORD_INTERVALS},
        {MINOR_NINTH_DOMINANT_CHORD,   MINOR_NINTH_DOMINANT_CHORD_INTERVALS}
};

// Chord states
enum chordStates{
    FUNDAMENTAL_STATE,  //0
    FIRST_INVERSION,    //1
    SECOND_INVERSION,   //2
    THIRD_INVERSION,    //3
    FOURTH_INVERSION    //4
};

const vector<std::string> stateNames = {"Fund", "1st inv", "2nd inv", "3rd inv"};

enum chordNotes{
    ROOT,       //0
    THIRD,      //1
    FIFTH,      //2
    SEVENTH,    //3
    NINTH      //4
};

/** Modes */
enum Mode {
    IONIAN,     //0, major mode
    DORIAN,     //1
    PHRYGIAN,   //2
    LYDIAN,     //3
    MIXOLYDIAN, //4
    AEOLIAN,    //5 , natural minor mode
    LOCRIAN     //6
};

// syntactic sugar for more commonly used modes
constexpr int MAJOR_MODE = IONIAN;
constexpr int MINOR_MODE = AEOLIAN;

const vector<std::string> modeNames = {"Major", "Dorian", "Phrygian", "Lydian", "Mixolydian", "Minor", "Locrian"};

/** Scales */
/// defined by the intervals between their notes

//@todo turn this into a dictionary with the name of the scale as key and the vector of intervals as value

const vector<int> MAJOR_SCALE = {MAJOR_SECOND, MAJOR_SECOND, MINOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MINOR_SECOND};
const vector<int> NATURAL_MINOR_SCALE = {MAJOR_SECOND, MINOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MINOR_SECOND, MAJOR_SECOND, MAJOR_SECOND};
const vector<int> HARMONIC_MINOR_SCALE = {MAJOR_SECOND, MINOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MINOR_SECOND, AUGMENTED_SECOND, MINOR_SECOND};
const vector<int> MELODIC_MINOR_SCALE = {MAJOR_SECOND, MINOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MINOR_SECOND};

/** Modulations */
enum modulations{
    PERFECT_CADENCE_MODULATION,         ///0.
    PIVOT_CHORD_MODULATION,             ///1.
    ALTERATION_MODULATION,              ///2. sudden change of the tonality by using a chord from the new key that contains a note that is not in the previous key
    CHROMATIC_MODULATION,               ///3. introducing the leading tone of the new tonality
};

const vector<string> modulation_type_names = {
    "Perfect Cadence",
    "Pivot Chord",
    "Alteration",
    "Secondary Dominant",
};

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                      Functions                                                      *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Returns a vector containing all the notes in a chord
 * @param root the root of the chord
 * @param quality the quality of the chord
 * @return
 */
vector<int> get_all_notes_in_chord(int root, int quality);

/**
 * Get all values for a given note
 * @param note a note
 * @return vector<int> a vector containing all the given notes
 */
vector<int> get_all_given_note(int note);

/**
 * Get the interval in semitones between the root and
 * @param quality the quality of the chord
 * @param chordNote the rank of the note in the chord (fundamental, third, fifth, etc.)
 * @return the interval in semitones between the root and the note
 */
int get_interval_from_root(int quality, int chordNote);

/**
 * Transforms an int* into a vector<int>
 * @param ptr an int* pointer
 * @param size the size of the array
 * @return a vector<int> containing the same values as the array
 */
vector<int> int_pointer_to_vector(const int* ptr, int size);

/**
 * Transforms a vector of integers into a string
 * @param vector a vector of integers
 * @return string the string representation of the vector
 */
string int_vector_to_string(const vector<int> &vector);

/**
 * Prints the Search::Statistics object into a readable format
 * @param stats a Search::Statistics object representing the statistics of a search
 * @return The string representation of the statistics object
 */
string statistics_to_string(const Search::Statistics &stats);

/**
 * Prints the Search::Statistics object into a csv format (coma separated)
 * @param stats a Search::Statistics object representing the statistics of a search
 * @return The string representation of the statistics object
 */
string statistics_to_csv_string(const Search::Statistics &stats);

/**
 * Returns the value of a variable as a string
 * @param var an integer variable
 * @param absolute a boolean indicating if the value should be returned as an absolute value (default is false)
 * @return a string representing the value of the variable
 */
string intVar_to_string(const IntVar &var, bool absolute = false);

/**
 * Returns the values of an array of variables as a string. Calls the intVar_to_string function
 * @param vars an array of integer variables
 * @return a string representing the values of the variables
 */
string intVarArray_to_string(IntVarArray vars);

vector<int> intVarArray_to_int_vector(const IntVarArray& vars);

/**
 * Returns the values of an IntVarArgs as a string
 * @param args an IntVarArgs
 * @return a string representing the values
 */
string intVarArgs_to_string(IntVarArgs args);

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
 * @param filename the name of the file to write
 */
void write_to_log_file(const char *message, const string& filename);

#endif
