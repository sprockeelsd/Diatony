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
    string message = "Section " + std::to_string(progression_number) + ":\n";
    message += "Number of chords: " + std::to_string(size) + "\n" +
        "Start: " + std::to_string(start) + "\n" +
        "End: " + std::to_string(end) + "\n" +
         "Tonality: " + midi_to_letter(tonality->get_tonic()) + " " + mode_int_to_name(tonality->get_mode()) + "\n" +
         "Chords: " +
         int_vector_to_string(chord_degrees) + "\n" +
            "Qualities: " +
            int_vector_to_string(chord_qualities) + "\n" +
            "States: " +
            int_vector_to_string(chord_states) + "\n";
    return message;
}

/**
 * pretty to_string method, with human-readable format
 * @return a string representation of the object in a more readable format
 */
string TonalProgressionParameters::pretty() const {
    string message = "Section " + std::to_string(progression_number) + ":\n";
    message += "Number of chords: " + std::to_string(size) + "\n" +
        "Start: " + std::to_string(start) + "\n" +
            "End: " + std::to_string(end) + "\n" +
         "Tonality: " + midi_to_letter(tonality->get_tonic()) + " " + mode_int_to_name(tonality->get_mode()) + "\n";
    message += "Chords: \n";
    for (int i = 0; i < chord_degrees.size(); i++) {
        message += degreeNames[chord_degrees[i]] + " (" + chordQualityNames[chord_qualities[i]] + ") in " +
                   stateNames[chord_states[i]];
        if (i != chord_degrees.size() - 1)
            message += " \n";
    }
    return message;
}