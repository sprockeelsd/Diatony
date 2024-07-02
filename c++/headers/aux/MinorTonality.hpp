// Author: Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description: A class representing a minor tonality
//
#ifndef MINORTONALITY
#define MINORTONALITY

#include "Tonality.hpp"

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
     * @param degree a degree of the scale [0,6]
     * @return the default chord quality for the given degree
     */
    int get_chord_quality(int degree);

    /**
     * Get the chord notes for a given degree
     * @param degree a degree of the scale [1,7]
     * @return an IntSet containing the chord notes for the given degree
     */
    IntSet get_scale_degree_chord(int degree);
};


#endif
