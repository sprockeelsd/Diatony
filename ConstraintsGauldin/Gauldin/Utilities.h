/**
 * @file Utilities.h
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief
 * @version 0.1
 * @date 2022-12-05
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef UTILITIES
#define UTILITIES

#include <gecode/int.hh>
#include <gecode/search.hh>
#include <iostream>
#include <vector>
#include <string>
#include <map>

std::string majorType = "Major";
std::string minorType = "minor";

/**
 * @brief Initializes the map of (Note name, value). Value are based on the 0 notes (A0, F#0, ....) so they can easily be scaled to the different octaves.
 * Note that the values below A0 are not "realistic notes", as in they are not playable on a piano. Using these values make it easier to compute the value
 * for any octave.
 */
void fillNoteNameToValue();

/**
 * @brief Takes as an argument a note name(e.g.F#3) and returns its MIDI value
 *
 * @param note the name of a note
 * @return int the MIDI value of that note
 */
int noteToMidi(std::string Note);

/**
 * @brief Get all the "note" notes (all Cs, all Bbs,...)
 *
 * @param note a note value in MIDI representation
 * @return std::vector<int> a dynamic array containing all the notes
 */
std::vector<int> getAllNotes(int note);

std::vector<int> getAllNotesTonality(int note, std::string tonality);

/**
 * @brief Print a dynamic array of integers
 *
 * @param v a dynamic array of integers
 */
void printIntVector(std::vector<int> v);

#endif