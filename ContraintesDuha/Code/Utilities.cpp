/**
 * @file Utilities.h
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This file contains useful constants and functions
 * @version 1.0
 * @date 2023-01-20
 *
 */

#include "Utilities.h"

/**
 * @brief For a given tonality (root + mode), get all the possible notes
 *
 * @param root the root of the tonality (in [12,23])
 * @param mode the mode of the tonality
 * @return vector<int> all the possible notes from that tonality
 */
const vector<int> getAllNotesFromTonality(int root, int mode)
{
    int note = root % 12 + 12; // bring the root back to [12,23] in case the argument is wrong
    vector<int> notes;

    if (mode == MAJOR)
    {
        int scale[7] = {2, 2, 1, 2, 2, 2, 1};
        int i = 0;
        while (note < 127)
        {
            notes.push_back(note);
            note += scale[i % 7];
            ++i;
        }
    }
    else if (mode == MINOR)
    {
        int scale[7] = {2, 1, 2, 2, 1, 2, 2};
        int i = 0;
        while (note < 127)
        {
            notes.push_back(note);
            note += scale[i % 7];
            ++i;
        }
    }
    return (const vector<int>) notes;
}

/**
 * @brief Print a dynamic array of integers
 *
 * @param v a dynamic array of integers
 */
void printIntVector(vector<int> v)
{
    for (int i = 0; i < v.size(); ++i)
    {
        std::cout << v.at(i) << " ";
    }
    std::cout << std::endl;
}
