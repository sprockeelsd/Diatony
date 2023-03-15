/**
 * @file Tonality.h
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This abstract class represents a tonality. It specifies a set of functions that each tonality must satisfy.
 * @version 0.1
 * @date 2023-03-08
 *
 */
#ifndef TONALITY
#define TONALITY

#include <gecode/int.hh>

#include "Utilities.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace Gecode;
using namespace std;

class Tonality // abstract class
{
public:
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
    virtual int getKey() = 0;

    /**
     * @brief returns the mode of the tonality
     *
     * @return vector<int> the mode of the tonality
     */
    virtual vector<int> getMode() = 0;

    /**
     * @brief returns the set of the notes in the tonality
     *
     * @return IntSet the set of the notes in the tonality
     */
    virtual IntSet getTonalityNotes() = 0;

    /**
     * @brief Returns the scale degrees that are present in the scale
     *
     * @return vector<int> a vector of integers representing the degrees in a scale using their interval with the root
     */
    virtual vector<int> getScaleDegrees() =0;

    /**
     * @brief Returns the map giving all notes for all degrees of the scale
     *
     * @return map<int, IntSet> a map containing all the notes for each degree of the scale
     */
    virtual map<int, IntSet> getScaleDegreeNotes() =0;

    /**
     * @brief Get all the notes for a given scale degree
     *
     * @param degree an integer representing the degree of the scale using its interval with the root
     * @return IntSet the set of all notes of that degree
     */
    virtual IntSet getScaleDegree(int degree) =0;

    /**
     * @brief Get all the notes for the chord for each degree of the scale
     *
     * @return map<int, IntSet> a map containing all notes for the chord based on each degree of the scale
     */
    virtual map<int, IntSet> getScaleDegreesChords() =0;

    /**
     * @brief Get all the notes for a given scale degree chord
     *
     * @param degree an integer representing the degree of the scale using its interval with the root
     * @return IntSet The set of all notes making the chord based on that scale degree
     */
    virtual IntSet getScaleDegreeChord(int degree) =0;
};

#endif