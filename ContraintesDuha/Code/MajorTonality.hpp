#ifndef MAJOR_TONALITY
#define MAJOR_TONALITY

#include <gecode/int.hh>

#include "Utilities.hpp"
#include "Tonality.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace Gecode;
using namespace std;

class MajorTonality : public Tonality
{
protected:
    int key;                                        // The key of the tonality
    vector<int> mode;                               // The mode of the tonality. Any mode is supported as long as it is described using a vector containing the intervals between the different notes of the scale
    IntSet tonalityNotes;                           // The set of all notes in the tonality
    vector<int> scaleDegrees;                       // An array of integer representing the degrees present in that scale using their interval with respect to the root
    vector<vector<int>> scaleDegreesChordQualities; // An array representing the quality of chords with respect to their degrees

    map<int, IntSet> scaleDegreeNotes;  // A dictionnary containing the corresponding notes for each scale degree represented as an integer
    map<int, IntSet> scaleDegreeChords; // A dictionnary containing the corresponding notes for each scale degree chord represented as an integer

public:
    /**
     * @brief Construct a new Major Tonality object
     *
     * @param root an integer representing the root of the tonality.
     */
    MajorTonality(int root);

    /**
     * @brief returns the key of the tonality
     *
     * @return int the key of the tonality [12,23]
     */
    int getKey() override { return key; };

    /**
     * @brief returns the mode of the tonality
     *
     * @return vector<int> the mode of the tonality
     */
    vector<int> getMode() override { return mode; };

    /**
     * @brief returns the set of the notes in the tonality
     *
     * @return IntSet the set of the notes in the tonality
     */
    IntSet getTonalityNotes() override { return tonalityNotes; };

    /**
     * @brief Returns the scale degrees that are present in the scale
     *
     * @return vector<int> a vector of integers representing the degrees in a scale using their interval with the root
     */
    vector<int> getScaleDegrees() override { return scaleDegrees; };

    /**
     * @brief Returns the map giving all notes for all degrees of the scale
     *
     * @return map<int, IntSet> a map containing all the notes for each degree of the scale
     */
    map<int, IntSet> getScaleDegreeNotes() override { return scaleDegreeNotes; };

    /**
     * @brief Get all the notes for a given scale degree
     *
     * @param degree an integer representing the degree of the scale using its interval with the root
     * @return IntSet the set of all notes of that degree
     */
    IntSet getScaleDegree(int degree) override { return scaleDegreeNotes[degree]; };

    /**
     * @brief Get all the notes for the chord for each degree of the scale
     *
     * @return map<int, IntSet> a map containing all notes for the chord based on each degree of the scale
     */
    map<int, IntSet> getScaleDegreesChords() override { return scaleDegreeChords; };

    /**
     * @brief Get all the notes for a given scale degree chord
     *
     * @param degree an integer representing the degree of the scale using its interval with the root
     * @return IntSet The set of all notes making the chord based on that scale degree
     */
    IntSet getScaleDegreeChord(int degree) override { return scaleDegreeChords[degree]; };
};

#endif