#ifndef TONALITY
#define TONALITY

#include <gecode/int.hh>

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace Gecode;
using namespace std;

class Tonality { // abstract class
protected:
    int tonic;
    vector<int> mode;
    // degrees of the scale
    IntSet tonics;                 // 1st degree of the scale
    IntSet supertonics;            // 2nd degree of the scale
    IntSet mediants;               // 3rd degree of the scale
    IntSet subdominants;           // 4th degree of the scale
    IntSet dominants;              // 5th degree of the scale
    IntSet submediants;            // 6th degree of the scale
    IntSet leading_tones;          // 7th degree of the scale

    IntSet tonal_notes;            // notes that don't change in major or minor mode (1,4,5 degrees)
    IntSet modal_notes;            // notes that change in major or minor mode (3,6,7 degrees)

    map<int, IntSet> scale_degrees_chords; // A dictionnary with pairs (degree, chordnotes)

public:
    // abstract methods are virtual
    Tonality(int t, vector<int> m);

    int get_tonic();
    vector<int> get_mode();

    virtual IntSet get_tonics() = 0;
    virtual IntSet get_supertonics() = 0;
    virtual IntSet get_mediants() = 0;
    virtual IntSet get_subdominants() = 0;
    virtual IntSet get_dominants() = 0;
    virtual IntSet get_submediants() = 0;
    virtual IntSet get_leading_tones() = 0;

    virtual map<int, IntSet> get_scale_degrees_chords() = 0;
};

#endif
