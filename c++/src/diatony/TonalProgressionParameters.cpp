//
// Created by Damien Sprockeels on 22/04/2025.
//

#include "../../headers/diatony/TonalProgressionParameters.hpp"

/**
 * to_string method
 * prints the number of chords, tonality, chord degrees, qualities and states
 * @return a string representation of the object
 */
string TonalProgressionParameters::to_string() const {
    string message;
    message += "Number of chords: " + std::to_string(size) + "\n" +
         "Tonality: " + midi_to_letter(tonality->get_tonic()) + " " + mode_int_to_name(tonality->get_mode()) + "\n" +
         "Chords: " +
         int_vector_to_string(chordDegrees) + "\n" +
            "Qualities: " +
            int_vector_to_string(chordQualities) + "\n" +
            "States: " +
            int_vector_to_string(chordStates) + "\n";
    return message;
}