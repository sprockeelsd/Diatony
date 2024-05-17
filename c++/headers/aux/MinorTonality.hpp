// Author: Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description: A class representing a minor tonality
//
#ifndef MINORTONALITY
#define MINORTONALITY

#include <gecode/int.hh>

#include "Utilities.hpp"
#include "Tonality.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace Gecode;
using namespace std;
class MinorTonality : public Tonality{
protected:
public:
    /**
     * Constructor
     * @param t the tonic of the tonality
     */
    MinorTonality(int t);

    /** Abstract methods from Tonality class */

    /**
     * Get the chord quality for each degree
     * @return a map of [degree, chord_quality] for each degree of the scale (1 to 7)
     */
    map<int, vector<int>> get_chord_qualities();

    /**
     * Get the chord notes for each degree
     * @return a map of [degree, chord] for each degree of the scale (1 to 7)
     */
    map<int, IntSet> get_scale_degrees_chords();

    /**
     * Get the chord notes for a given degree
     * @param degree a degree of the scale [1,7]
     * @return an IntSet containing the chord notes for the given degree
     */
    IntSet get_scale_degree_chord(int degree);
};


#endif
