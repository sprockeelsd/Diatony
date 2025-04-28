// Author: Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description: A file containing useful functions and constants for the symbolic musical problems
//
#ifndef UTILITIES
#define UTILITIES

#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <ctime>
#include <exception>
#include <set>

#include "gecode/kernel.hh"
#include "gecode/int.hh"
#include "gecode/search.hh"
#include "gecode/minimodel.hh"
#include "gecode/set.hh"

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

/** Branching strategies */
enum variable_selection{
    DEGREE_MAX,                     //0
    DOM_SIZE_MIN,                   //1
    RIGHT_TO_LEFT,                  //2
    LEFT_TO_RIGHT_SOPRANO_TO_BASS,  //3
    AFC_MAX,                        //4
};

/// go <-- soprano->bass: 4-3-2-1-8-7-6-5 etc
auto right_to_left = [](const Space& home, const IntVar& x, int i) {
    return i;
};

/// go --> soprano->bass
auto left_to_right_soprano_to_bass = [](const Space& home, const IntVar& x, int i) {
    return (i/4) * 4 + (4 - i%4);
};

const vector<IntVarBranch> variable_selection_heuristics = {INT_VAR_DEGREE_MAX(),
                                                            INT_VAR_SIZE_MIN(),
                                                            INT_VAR_MERIT_MAX(right_to_left),
                                                            INT_VAR_MERIT_MIN(left_to_right_soprano_to_bass)};

const vector<string> variable_selection_heuristics_names = {"Degree max", "Domain size min", "Left to right",
                                                            "Right to left", "AFC max"};

enum value_selection{
    VAL_MIN,            //0
    VAL_MAX,            //1
    VAL_MED,            //2
    VAL_RND,            //3
};

/// value selection heuristic
auto branchVal = [](const Space& home, IntVar x, int i) {
    return x.min();
};

/// commit function (EQ and DIFF)
auto branchCommit = [](Space& home, unsigned int a, IntVar x, int i, int n){
    if (a == 0U){
        rel(home, x, IRT_EQ, n);
    } else {
        rel(home, x, IRT_NQ, n);
    }
};

const vector<IntValBranch> value_selection_heuristics = {INT_VAL_MIN(), INT_VAL_MAX(), INT_VAL_MED(), INT_VAL_RND(1U)};

const vector<string> value_selection_heuristics_names = {"Value min", "Value max", "Median value", "Value random"};


/** Voice ranges */
const int BASS_MIN = 40;
const int BASS_MAX = 60;
const int TENOR_MIN = 48;
const int TENOR_MAX = 69;
const int ALTO_MIN = 55;
const int ALTO_MAX = 75;
const int SOPRANO_MIN = 60;
const int SOPRANO_MAX = 84;

/** Melodic costs */
const int UNISON_COST = 0;
const int SECOND_COST = 1;
const int THIRD_COST = 3;
const int FOURTH_COST = 6;
const int TRITONE_COST = 6;
const int FIFTH_COST = 6;
const int SIXTH_COST = 12;
const int SEVENTH_COST = 18;
const int OCTAVE_COST = 6;

const int MAX_MELODIC_COST = SEVENTH_COST;

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
const int DIMINISHED_THIRD = 2;
const int AUGMENTED_FOURTH = TRITONE;
const int DIMINISHED_FIFTH = TRITONE;

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
    IONIAN,     //0 , major mode
    DORIAN,     //1
    PHRYGIAN,   //2
    LYDIAN,     //3
    MIXOLYDIAN, //4
    AEOLIAN,    //5 , natural minor mode
    LOCRIAN     //6
};

// syntactic sugar for more commonly used modes
const int MAJOR_MODE = IONIAN;
const int MINOR_MODE = AEOLIAN;

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
    PERFECT_CADENCE_MODULATION,         ///0
    PIVOT_CHORD_MODULATION,             ///1
    ALTERATION_MODULATION,              ///2, sudden change of the tonality by using a chord from the new key that contains a note that is not in the previous key
    SECONDARY_DOMINANT_MODULATION,      ///3, introducing the sensitive note of the new tonality
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
 * Prints the Search::Statistics object into a csv format (coma separated)
 * @param stats a Search::Statistics object representing the statistics of a search
 * @return The string representation of the statistics object
 */
string statistics_to_csv_string(Search::Statistics stats);

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

vector<int> intVarArray_to_int_vector(IntVarArray vars);

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
 */
void write_to_log_file(const char *message, const string& filename);

#endif
