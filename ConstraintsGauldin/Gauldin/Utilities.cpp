/**
 * @file Utilities.cpp
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief
 * @version 0.1
 * @date 2022-12-05
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "Utilities.h"

using namespace Gecode;
using namespace std;

unordered_map<std::string, int> noteNameToValue;

enum chord_type
{
    M,
    m,
    d7,
    M7,
    m7,
    d,
    A
};

/**
 * @brief Initializes the map of (Note name, value). Value are based on the 0 notes (A0, F#0, ....) so they can easily be scaled to the different octaves.
 * Note that the values below A0 are not "realistic notes", as in they are not playable on a piano. Using these values make it easier to compute the value
 * for any octave.
 */
void fillNoteNameToValue()
{
    noteNameToValue["C"] = 12;
    noteNameToValue["C#"] = 13;
    noteNameToValue["Db"] = 13;
    noteNameToValue["D"] = 14;
    noteNameToValue["D#"] = 15;
    noteNameToValue["Eb"] = 15;
    noteNameToValue["E"] = 16;
    noteNameToValue["F"] = 17;
    noteNameToValue["F#"] = 18;
    noteNameToValue["Gb"] = 18;
    noteNameToValue["G"] = 19;
    noteNameToValue["Ab"] = 20;
    noteNameToValue["G#"] = 20;
    noteNameToValue["A"] = 21;
    noteNameToValue["A#"] = 22;
    noteNameToValue["Bb"] = 22;
    noteNameToValue["B"] = 23;
}

/**
 * @brief Takes as an argument a note name(e.g.F#3) and returns its MIDI value
 *
 * @param note the name of a note
 * @return int the MIDI value of that note
 */
int noteToMidi(std::string note)
{
    int octave = (int)(note[note.size() - 1] - 48);
    std::string name = note.substr(0, note.size() - 1);

    return noteNameToValue.at(name) + octave * 12;
}

/**
 * @brief Get all the "note" notes (all Cs, all Bbs,...)
 *
 * @param note a note value in MIDI representation
 * @return std::vector<int> a dynamic array containing all the notes
 */
std::vector<int> getAllNotes(int note)
{
    std::vector<int> accumulator;
    note = note % 12 + 12;
    while (note < 127)
    {
        accumulator.push_back(note);
        note += 12;
    }
    return accumulator;
}

std::vector<int> getAllNotesTonality(int note, std::string tonality){
    std::vector<int> notes;
    if(tonality == majorType){
        int steps[] = {2,2,1,2,2,2,1};
        for(const int i:steps){
            note += i;
            notes.push_back((note%12)+12);
        }
    }
    return notes;
}

/**
 * @brief Get all the possible notes for a given chord
 *
 * @param root a note in MIDI representation
 * @param chordType M, m, 7, m7, M7, d, A, 5, 6, 9, ....
 * @return vector<int> an vector containing all the note values for the chord
 */

/**
 * @brief Print a dynamic array of integers
 *
 * @param v a dynamic array of integers
 */
void printIntVector(std::vector<int> v)
{
    for (int i = 0; i < v.size(); ++i)
    {
        std::cout << v.at(i) << " ";
    }
    std::cout << std::endl;
}