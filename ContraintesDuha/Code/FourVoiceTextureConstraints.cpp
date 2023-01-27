/**
 * @file FourVoiceTextureConstraints.cpp
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This class contains all the musical constraints
 * @version 1.0
 * @date 2023-01-25
 *
 */

#include "FourVoiceTextureConstraints.h"

/**
 * @brief Posts the constraint that the domain of vars is equal to domain
 * 
 * @param space An instance of a 4 voice texture problem
 * @param vars The array of variables that we want to apply the constraint to
 * @param domain The set of notes representing the tonality
 */