/**
 * @file Utilities.h
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This file contains useful constants and functions
 * @version 1.0
 * @date 2023-01-20
 *
 */

#include "Utilities.h"

/**
 * @brief For a given tonality (root + mode), get all the possible notes
 *
 * @param root the root of the tonality (in [12,23])
 * @param scale the set of tones and semitones that define the scale
 * @return vector<int> all the possible notes from that tonality
 */
const vector<int> getAllNotesFromTonality(int root, vector<int> scale)
{
    int note = root % 12 + 12; // bring the root back to [12,23] in case the argument is wrong
    vector<int> notes;

    int i = 0;
    while (note < 127)
    {
        notes.push_back(note);
        note += scale[i % scale.size()];
        ++i;
    }
    return (const vector<int>)notes;
}

/**
 * @brief For a given chord (root + mode), get all the possible notes
 *
 * @param root the root of the chord
 * @param quality the set of tones and semitones that define the chord
 * @return vector<int> all the possible notes from that chord
 */
const vector<int> getAllNotesFromChord(int root, vector<int> quality)
{
    int note = root % 12 + 12; // bring the root back to [12,23] in case the argument is wrong
    vector<int> notes;

    int i = 0;
    while (note < 127)
    {
        notes.push_back(note);
        note += quality[i % quality.size()];
        ++i;
    }
    return (const vector<int>)notes;
}

/**
 * @brief Print a dynamic array of integers
 *
 * @param v a dynamic array of integers
 */
void printIntVector(vector<int> v)
{
    for (int i = 0; i < v.size(); ++i)
    {
        std::cout << v.at(i) << " ";
    }
    std::cout << std::endl;
}
