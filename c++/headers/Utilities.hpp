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
enum {
    dfs_solver, //0
    bab_solver, //1
};

const vector<std::string> noteNames = {"C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"}; // @todo turn into a map

/** Notes */
const int Bsharp = 12;
const int C = 12;
const int Csharp = 13;
const int Dflat = 13;
const int D = 14;
const int Dsharp = 15;
const int Eflat = 15;
const int E = 16;
const int Fflat = 16;
const int Esharp = 17;
const int F = 17;
const int Fsharp = 18;
const int Gflat = 18;
const int G = 19;
const int Gsharp = 20;
const int Aflat = 20;
const int A = 21;
const int Asharp = 22;
const int Bflat = 22;
const int B = 23;
const int Cflat = 23;

/** scale degrees */ // @todo rename this into 1st to 7th degree, will be easier
const int tonic = 1;
const int supertonic = 2;
const int mediant = 3;
const int subdominant = 4;
const int dominant = 5;
const int submediant = 6;
const int leadingTone = 7;

/** Intervals */
// "classic" intervals
enum{
    unisson,        //0
    minorSecond,    //1
    majorSecond,    //2
    minorThird,     //3
    majorThird,     //4
    perfectFourth,  //5
    tritone,        //6
    perfectFifth,   //7
    minorSixth,     //8
    majorSixth,     //9
    minorSeventh,   //10
    majorSeventh,   //11
    perfectOctave   //12
};

// augmented/diminished intervals
const int augmentedSecond = 3;

/** Chords */
// Types of chords represented by the intervals between their notes in root position up to an octave
const vector<int> MAJOR_CHORD = {majorThird, minorThird, perfectFourth};
const vector<int> MINOR_CHORD = {minorThird, majorThird, perfectFourth};
const vector<int> DIMINISHED_CHORD = {minorThird, minorThird, tritone};
const vector<int> AUGMENTED_CHORD = {majorThird, majorThird, majorThird};
const vector<int> DOMINANT_SEVENTH_CHORD = {majorThird, minorThird, minorThird, majorSecond};
const vector<int> MAJOR_SEVENTH_CHORD = {majorThird, minorThird, majorThird, minorSecond};
const vector<int> MINOR_SEVENTH_CHORD = {minorThird, majorThird, minorThird, majorSecond};

// Chord states
enum{
    fundamental_state,  //0
    first_inversion,    //1
    second_inversion,   //2
    third_inversion     //3
};

/** Modes */
// syntactic sugar for more commonly used modes
const int major_mode = 0;
const int minor_mode = 5;   // to correspond to the enum Mode

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
// defined by the intervals between their notes in semi-tones

// turn this into a dictionary with the name of the scale as key and the vector of intervals as value

const vector<int> MAJOR_SCALE = {majorSecond, majorSecond, minorSecond, majorSecond, majorSecond, majorSecond, minorSecond};
const vector<int> NATURAL_MINOR_SCALE = {majorSecond, minorSecond, majorSecond, majorSecond, minorSecond, majorSecond, majorSecond};
const vector<int> HARMONIC_MINOR_SCALE = {majorSecond, minorSecond, majorSecond, majorSecond, minorSecond, augmentedSecond, minorSecond};
const vector<int> MELODIC_MINOR_SCALE = {majorSecond, minorSecond, majorSecond, majorSecond, majorSecond, majorSecond, minorSecond};

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
IntSet getAllNotesFromIntervalLoop(int root, vector<int> scale);

/**
 * For a given tonality (root + mode), returns all the possible notes
 * @param root the root of the tonality (in [12,23])
 * @param scale the set of tones and semitones that define the scale
 * @return vector<int> all the possible notes from that tonality
 */
IntSet getAllNotesFromTonality(int root, vector<int> scale);

/**
 * For a given chord (root + mode), returns all the possible notes
 * @param root the root of the chord
 * @param quality the set of tones and semitones that define the chord
 * @return vector<int> all the possible notes from that chord
 */
IntSet getAllNotesFromChord(int root, vector<int> quality);

/**
 * Get all values for a given note
 * @param note a note
 * @return const vector<int> a vector containing all the given notes
 */
IntSet getAllGivenNote(int note);

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
 * @return
 */
vector<int> int_pointer_to_vector(int* ptr, int size);

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
