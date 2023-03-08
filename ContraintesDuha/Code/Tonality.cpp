/**
 * @file Tonality.cpp
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This class represents a tonality based on a root note and a mode
 * @version 0.1
 * @date 2023-03-08
 *
 */

#include "Tonality.h"

/**
 * @brief Construct a new Tonality object
 *
 * @param root an integer representing the root of the tonality.
 * @param tonalityMode a vector of integers representing the steps between consecutive notes of the scale
 */
Tonality::Tonality(int root, vector<int> tonalityMode)
{
    key = root % 12 + 12;
    mode = tonalityMode;

    fundamentals = getAllGivenNote(key);
    dominants = getAllGivenNote(key + perfectFifth);

    tonalityNotes = getAllNotesFromTonality(root, mode);
}

/**
 * @brief Construct a new Tonality object. The default values are C and MAJOR_SCALE
 *
 */
Tonality::Tonality()
{
    key = C;
    mode = MAJOR_SCALE;

    fundamentals = getAllGivenNote(key);
    dominants = getAllGivenNote(key + perfectFifth);

    tonalityNotes = getAllNotesFromTonality(key, mode);
}

// ----------------------------------------------- getters ----------------------------------------------------

/**
 * @brief returns the key of the tonality
 *
 * @return int the key of the tonality [12,23]
 */
int Tonality::getKey()
{
    return key;
}

/**
 * @brief returns the mode of the tonality
 *
 * @return vector<int> the mode of the tonality
 */
vector<int> Tonality::getMode()
{
    return mode;
}

/**
 * @brief returns the set of fundamentals
 *
 * @return IntSet the set of fundamentals
 */
IntSet Tonality::getFundamentals()
{
    return fundamentals;
}

/**
 * @brief returns the set of dominants
 *
 * @return IntSet the set of dominants
 */
IntSet Tonality::getDominants()
{
    return dominants;
}

/**
 * @brief returns the set of the notes in the tonality
 *
 * @return IntSet the set of the notes in the tonality
 */
IntSet Tonality::getTonalityNotes()
{
    return tonalityNotes;
}