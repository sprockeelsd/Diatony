/**
 * @file Utilities.h
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This file contains useful constants and functions
 * @version 1.0
 * @date 2023-01-20
 *
 */
#ifndef UTILITIES
#define UTILITIES

#include <iostream>
#include <vector>
#include <string>

using namespace std;

//--------------------Useful constants---------------------

// Modes and types of chords
const int MAJOR = 0;
const int MINOR = 1;
const int DIMINISHED = 2;
const int AUGMENTED = 3;

// Notes
const int Bsharp = 12;
const int C = 12;
const int Csharp = 13;
const int Dflat = 13;
const int D = 14;
const int Dsharp = 15;
const int Eflat = 15;
const int E = 16;
const int Fflat = 16;
const int Esharp = 17;
const int F = 17;
const int Fsharp = 18;
const int Gflat = 18;
const int G = 19;
const int Gsharp = 20;
const int Aflat = 20;
const int A = 21;
const int Asharp = 22;
const int Bflat = 22;
const int B = 23;
const int Cflat = 23;

// Scales
const int majorScale[] = {2, 2, 1, 2, 2, 2, 1};
const int minorScale[] = {2, 1, 2, 2, 1, 2, 2};

/**
 * @brief For a given tonality (root + mode), get all the possible notes
 * 
 * @param root the root of the tonality
 * @param mode the mode of the tonality
 * @return vector<int> all the possible notes from that tonality
 */
const vector<int> getAllNotesFromTonality(int root, int mode);

/**
 * @brief Print a dynamic array of integers
 *
 * @param v a dynamic array of integers
 */
void printIntVector(vector<int> vector);

#endif