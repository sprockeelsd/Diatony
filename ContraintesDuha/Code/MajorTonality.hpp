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
};

#endif