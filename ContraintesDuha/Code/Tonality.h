/**
 * @file Tonality.h
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This class represents a tonality based on a root note and a mode
 * @version 0.1
 * @date 2023-03-08
 *
 */
#ifndef TONALITY
#define TONALITY

#include <gecode/int.hh>

#include "Utilities.h"

#include <iostream>
#include <vector>
#include <string>

using namespace Gecode;
using namespace std;

class Tonality // faire des classes qui héritent de celle-ci pourl es différents modes
{
protected:
    int key;          // The key of the tonality
    vector<int> mode; // The mode of the tonality. Any mode is supported as long as it is described using a vector containing the intervals between the different notes of the scale

    IntSet fundamentals; // All the fundamentals
    IntSet dominants;    // All the dominants @todo maybe move this to child classes?

    IntSet tonalityNotes; // The set of all notes in the tonality

public:
    /**
     * @brief Construct a new Tonality object
     * 
     * @param root an integer representing the root of the tonality.
     * @param tonalityMode a vector of integers representing the steps between consecutive notes of the scale
     */
    Tonality(int root, vector<int> tonalityMode);

    /**
     * @brief Construct a new Tonality object. The default values are C and MAJOR_SCALE
     * 
     */
    Tonality();

    // ----------------------------------------------- getters ----------------------------------------------------

    /**
     * @brief returns the key of the tonality
     * 
     * @return int the key of the tonality [12,23]
     */
    int getKey();

    /**
     * @brief returns the mode of the tonality
     * 
     * @return vector<int> the mode of the tonality
     */
    vector<int> getMode();

    /**
     * @brief returns the set of fundamentals
     * 
     * @return IntSet the set of fundamentals
     */
    IntSet getFundamentals();
    /**
     * @brief returns the set of dominants
     * 
     * @return IntSet the set of dominants
     */
    IntSet getDominants();

    /**
     * @brief returns the set of the notes in the tonality
     * 
     * @return IntSet the set of the notes in the tonality
     */
    IntSet getTonalityNotes();
};

#endif