/**
 * @file Tonality.h
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This class models a tonality
 * @version 0.1
 * @date 2022-12-07
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef TONALITY
#define TONALITY

#include <iostream>
#include <vector>
#include <string>

/**
 * @brief This class models a tonality
 *
 */
class Tonality
{
    enum chord_type
    {
        M,  // major
        m,  // minor
        d7, // dominant 7
        M7, // major 7
        m7, // minor 7
        d,  // diminished
        A   // augmented
    };

    enum mode
    {
        Major,
        natural_minor,
        harmonic_minor,
        melodic_minor
    };

protected:
    int root;
    std::string mode;
    std::vector<std::vector<int>> chords;
    std::vector<int> notes;

public:
    Tonality(int root, std::string mode);
};

#endif