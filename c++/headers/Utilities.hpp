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

const vector<std::string> noteNames = {"C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"};

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

/** Intervals */
// for now, no augmented or diminished intervals
const int unisson = 0;
const int minorSecond = 1;
const int majorSecond = 2;
const int minorThird = 3;
const int majorThird = 4;
const int perfectFourth = 5;
const int tritone = 6;
const int perfectFifth = 7;
const int minorSixth = 8;
const int majorSixth = 9;
const int minorSeventh = 10;
const int majorSeventh = 11;
const int perfectOctave = 12;

/** Chords */
// Types of chords represented by the intervals between their notes in root position up to an octave
const vector<int> MAJOR_CHORD = {4, 3, 5};
const vector<int> MINOR_CHORD = {3, 4, 5};
const vector<int> DIMINISHED_CHORD = {3, 3, 6};
const vector<int> AUGMENTED_CHORD = {4, 4, 4};
const vector<int> DOMINANT_SEVENTH_CHORD = {4, 3, 3, 2};
const vector<int> MAJOR_SEVENTH_CHORD = {4, 3, 4, 1};
const vector<int> MINOR_SEVENTH_CHORD = {3, 4, 3, 2};

/** Modes */
// syntactic sugar for more commonly used modes
const int major_mode = 0;
const int minor_mode = 5;   // to correspond to the enum Mode

enum Mode {
    IONIAN,     // major mode
    DORIAN,
    PHRYGIAN,
    LYDIAN,
    MIXOLYDIAN,
    AEOLIAN,    // natural minor mode
    LOCRIAN
};

/** Scales */
// defined by the intervals between their notes in semi-tones

// turn this into a dictionnary with the name of the scale as key and the vector of intervals as value

const vector<int> MAJOR_SCALE = {2, 2, 1, 2, 2, 2, 1};
const vector<int> NATURAL_MINOR_SCALE = {2, 1, 2, 2, 1, 2, 2};
const vector<int> HARMONIC_MINOR_SCALE = {2, 1, 2, 2, 1, 3, 1};
const vector<int> MELODIC_MINOR_SCALE = {2, 1, 2, 2, 2, 2, 1};

/***********************************************************************************************************************
 *                                                      Functions                                                      *
 ***********************************************************************************************************************/

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
 * Print a dynamic array of integers
 * @param v a dynamic array of integers
 */
void printIntVector(vector<int> vector);

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
