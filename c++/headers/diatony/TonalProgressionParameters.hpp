//
// Created by Damien Sprockeels on 22/04/2025.
//

#ifndef TONALPROGRESSIONPARAMETERS_HPP
#define TONALPROGRESSIONPARAMETERS_HPP

#include <utility>

#include "../aux/Utilities.hpp"
#include "../aux/Tonality.hpp"

/**
 * Class representing all the parameters necessary to create a tonal progression.
 */
class TonalProgressionParameters {
protected:
    const int                       progression_number;     // The position of the progression in the piece (0 for the first progression, 1 for the second, etc.)
    const int                       size;                   // The number of chords in the progression
    const int                       start;                  // The position in the global piece at which the progression starts
    const int                       end;                    // The position in the global piece at which the progression ends
    Tonality*                       tonality;               // The tonality of the progression
    const vector<int>               chord_degrees;          // The degrees of the chords in the progression
    const vector<int>               chord_qualities;        // The qualities of the chords
    const vector<int>               chord_states;           // The states of the chords

public:
    /**
     * Constructor
     * @param prog_n        The position of the progression in the piece (0 for the first progression, 1 for the second, etc.)
     * @param s             The number of chords
     * @param start         The start position of this progression in the whole piece
     * @param end           The end position of this progression in the whole piece
     * @param t             The tonality
     * @param chord_degs    The chord degrees
     * @param chord_quals   The chord qualities
     * @param chord_stas    The chord states
     */
    TonalProgressionParameters(const int prog_n, const int s, int start, int end, Tonality *t,
                                vector<int> chord_degs, vector<int> chord_quals, vector<int> chord_stas):
        progression_number(prog_n), size(s), start(start), end(end), tonality(t),
        chord_degrees(std::move(chord_degs)), chord_qualities(std::move(chord_quals)),
        chord_states(std::move(chord_stas)) {}

    /**
     * Copy constructor
     * @param params A pointer to a TonalProgressionParameters object
     */
    explicit TonalProgressionParameters(const TonalProgressionParameters * params):
        progression_number(params->progression_number), size(params->size), start(params->start), end(params->end),
        tonality(params->tonality),
        chord_degrees(params->chord_degrees), chord_qualities(params->chord_qualities),
        chord_states(params->chord_states) {}

    /**                     getters                     **/

    int get_progression_number() const { return progression_number; }

    int get_size() const { return size; }

    int get_start() const { return start; }

    int get_end() const { return end; }

    Tonality *get_tonality() const { return tonality; }

    vector<int> get_chordDegrees() const { return chord_degrees; }

    vector<int> get_chordQualities() const { return chord_qualities; }

    vector<int> get_chordStates() const { return chord_states; }

    /**
     * to_string method
     * prints the number of chords, tonality, chord degrees, qualities and states
     * @return a string representation of the object
     */
    string to_string() const;

    /**
     * pretty to_string method, with human-readable format
     * @return a string representation of the object in a more readable format
     */
    string pretty() const;
};

#endif //TONALPROGRESSIONPARAMETERS_HPP