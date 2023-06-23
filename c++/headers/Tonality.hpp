#ifndef TONALITY
#define TONALITY

#include <gecode/int.hh>

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace Gecode;
using namespace std;

/**
 * This class models a tonality
 */
class Tonality { // abstract class
protected:
    int tonic;                  // tonic of the tonality
    vector<int> mode;           // mode of the tonality
    vector<int> degrees_notes;  // notes corresponding to the degrees of the scale (first elem = tonic, second_elem = second degree, etc.) @todo change into a map as well so it is easier to acces
    map<int, vector<int>> chord_qualities; // map of [degree, chord_quality] for each degree of the scale (1 to 7)

    map<int, IntSet> scale_degrees;         // map of [degree, all_notes] for each degree of the scale (1 to 7)
    IntSet tonality_notes;                  // all the notes in the tonality
    IntSet tonal_notes;            // notes that don't change in major or minor mode (1,4,5 degrees)
    IntSet modal_notes;            // notes that change in major or minor mode (3,6,7 degrees)

    map<int, IntSet> scale_degrees_chords; // map of [degree, chord] for each degree of the scale (1 to 7) @todo add in constructor

public:
    // abstract methods are virtual
    /**
     * Constructor
     * @param t the tonic of the tonality
     * @param m the mode of the tonality
     */
    Tonality(int t, vector<int> m);

    /**
     * Get the tonic of the tonality
     * @return the tonic of the tonality
     */
    int get_tonic();

    /**
     * Get the mode of the tonality
     * @return the mode of the tonality
     */
    vector<int> get_mode();

    /**
     * Get the notes corresponding to the degrees of the scale (first elem = tonic, second_elem = second degree, etc.)
     * @return a vector containing the notes for each of the scale degrees
     */
    vector<int> get_degrees_notes();

    /**
     * Get the chord quality for each degree
     * @return a map of [degree, chord_quality] for each degree of the scale (1 to 7)
     */
    map<int, vector<int>> get_chord_qualities();

    /**
     * Get all the notes in the tonality
     * @return an IntSet containing all the notes in the tonality
     */
    IntSet get_tonality_notes();

    /**
     * Get all the notes for each scale degree
     * @return a map of [degree, all_notes] for each degree of the scale (1 to 7)
     */
    map<int, IntSet> get_scale_degrees();

    /**
     * Get all the notes for a given scale degree
     * @param degree a degree of the scale [1,7]
     * @return an IntSet containing all the notes for the given scale degree
     */
    IntSet get_scale_degree(int degree);

    /**
     * Get the notes that don't change in major or minor mode (1,4,5 degrees)
     * @return an IntSet containing the tonal notes
     */
    IntSet get_tonal_notes();

    /**
     * Get the notes that change in major or minor mode (3,6,7 degrees)
     * @return an IntSet containing the modal notes
     */
    IntSet get_modal_notes();

    /**
     * Get the chord notes for each degree
     * @return a map of [degree, chord] for each degree of the scale (1 to 7)
     */
    virtual map<int, IntSet> get_scale_degrees_chords(); // = 0; // @todo maybe make it abstract, otherwise there is currently no interest in having this class

    /**
     * Get the chord notes for a given degree
     * @param degree a degree of the scale [1,7]
     * @return an IntSet containing the chord notes for the given degree
     */
    virtual IntSet get_scale_degree_chord(int degree); // = 0; // @todo maybe make it abstract, otherwise there is currently no interest in having this class

    /**
     * Creates a string representing the tonality object
     * @return a string representing the tonality object
     */
    string to_string() const;

    /**
     * Overload of the << operator for Tonality objects
     */
    // friend std::ostream& operator<<(std::ostream& os, const Tonality& t);
};

#endif