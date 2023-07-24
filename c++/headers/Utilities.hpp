#ifndef UTILITIES
#define UTILITIES

#include <gecode/int.hh>
#include <gecode/minimodel.hh>

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;
using namespace Gecode;

/***********************************************************************************************************************
 *                                                  Useful constants                                                   *
 ***********************************************************************************************************************/

/** Types of search engines */
enum solver_types{
    DFS_SOLVER, //0
    BAB_SOLVER, //1
};

const vector<std::string> noteNames = {"C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"}; // @todo turn into a bi-directional map

/** Notes */
const int Bsharp = 0;
const int C = 0;
const int Csharp = 1;
const int Dflat = 1;
const int D = 2;
const int Dsharp = 3;
const int Eflat = 3;
const int E = 4;
const int Fflat = 4;
const int Esharp = 5;
const int F = 5;
const int Fsharp = 6;
const int Gflat = 6;
const int G = 7;
const int Gsharp = 8;
const int Aflat = 8;
const int A = 9;
const int Asharp = 10;
const int Bflat = 10;
const int B = 11;
const int Cflat = 11;

enum voices{
    BASS,
    TENOR,
    ALTO,
    SOPRANO
};

enum degrees{
    FIRST_DEGREE,
    SECOND_DEGREE,
    THIRD_DEGREE,
    FOURTH_DEGREE,
    FIFTH_DEGREE,
    SIXTH_DEGREE,
    SEVENTH_DEGREE
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
const int augmented_second = 3;

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

/** Scales */
/// defined by the intervals between their notes

//@todo turn this into a dictionary with the name of the scale as key and the vector of intervals as value

const vector<int> MAJOR_SCALE = {MAJOR_SECOND, MAJOR_SECOND, MINOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MINOR_SECOND};
const vector<int> NATURAL_MINOR_SCALE = {MAJOR_SECOND, MINOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MINOR_SECOND, MAJOR_SECOND, MAJOR_SECOND};
const vector<int> HARMONIC_MINOR_SCALE = {MAJOR_SECOND, MINOR_SECOND, MAJOR_SECOND, MAJOR_SECOND, MINOR_SECOND, augmented_second, MINOR_SECOND};
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
vector<int> getAllNotesFromIntervalLoop(int note, vector<int> intervals);

/**
 * For a given tonality (root + mode), returns all the possible notes
 * @param root the root of the tonality (in [0,11])
 * @param scale the set of tones and semitones that define the scale
 * @return vector<int> all the possible notes from that tonality
 */
vector<int> getAllNotesFromTonality(int root, vector<int> scale);

/**
 * For a given chord (root + mode), returns all the possible notes
 * @param root the root of the chord
 * @param quality the set of tones and semitones that define the chord
 * @return vector<int> all the possible notes from that chord
 */
vector<int> getAllNotesFromChord(int root, vector<int> quality);

/**
 * Get all values for a given note
 * @param note a note
 * @return vector<int> a vector containing all the given notes
 */
vector<int> getAllGivenNote(int note);

/**
 * Transforms a vector of integers into a string
 * @param vector a vector of integers
 * @return string the string representation of the vector
 */
string int_vector_to_string(vector<int> vector);

/**
 * Transforms an int* into a vector<int>
 * @param ptr an int* pointer
 * @param size the size of the array
 * @return a vector<int> containing the same values as the array
 */
vector<int> int_pointer_to_vector(int* ptr, int size);

/**
 * Prints the Search::Statistics object into a readable format
 * @param stats a Search::Statistics object representing the statistics of a search
 * @return The string representation of the statistics object
 */
string statistics_to_string(Search::Statistics stats);

string intVarArrayToString(IntVarArray vars);

string intVarToString(IntVar var);

/**
 * Prints A note with its name (e.g. 60 = C)
 * @param var an integer variable
 */
void printNoteInLetter(IntVar var);

/**
 * Prints a node in MIDIcent value
 * @param var an integer variable
 */
void printNoteForOM(IntVar var);

#endif
