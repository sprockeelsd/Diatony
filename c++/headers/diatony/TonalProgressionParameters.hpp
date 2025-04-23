//
// Created by Damien Sprockeels on 22/04/2025.
//

#ifndef TONALPROGRESSIONPARAMETERS_HPP
#define TONALPROGRESSIONPARAMETERS_HPP

#include <utility>

#include "../aux/Utilities.hpp"
#include "../aux/Tonality.hpp"

/**
 * Class representing all the parameters necessary to create a tonal progression
 */
class TonalProgressionParameters {
protected:
    const int                         size;                                       // The number of chords in the progression
    int                               start;
    int                               end;
    Tonality*                         tonality;                                   // The tonality of the progression
    const vector<int>                 chordDegrees;                               // The degrees of the chords in that tonality
    const vector<int>                 chordQualities;                             // The qualities of the chords
    const vector<int>                 chordStates;                                // The states of the chords

public:
    /**
     * Constructor
     * @param s the number of chords
     * @param start the start position of this progression in the whole piece
     * @param end the end position of this progression in the whole piece
     * @param t the tonality
     * @param chordDegs the chord degrees
     * @param chordQuals the chord qualities
     * @param chordStas the chord states
     */
    TonalProgressionParameters(const int s, int start, int end, Tonality *t,
                                vector<int> chordDegs, vector<int> chordQuals, vector<int> chordStas):
        size(s), start(start), end(end), tonality(t), chordDegrees(std::move(chordDegs)), chordQualities(std::move(chordQuals)),
        chordStates(std::move(chordStas)) {}

    /// ----------------getters----------------------------
    int get_size() const { return size; }

    int get_start() const { return start; }

    int get_end() const { return end; }

    Tonality *get_tonality() const { return tonality; }

    vector<int> get_chordDegrees() const { return chordDegrees; }

    vector<int> get_chordQualities() const { return chordQualities; }

    vector<int> get_chordStates() const { return chordStates; }

    /**
     * to_string method
     * prints the number of chords, tonality, chord degrees, qualities and states
     * @return a string representation of the object
     */
    string to_string() const;
};

#endif //TONALPROGRESSIONPARAMETERS_HPP