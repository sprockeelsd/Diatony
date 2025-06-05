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
    string message = "Section " + std::to_string(sectionNumber) + ":\n";
    message += "Number of chords: " + std::to_string(size) + "\n" +
        "Start: " + std::to_string(start) + "\n" +
            "End: " + std::to_string(end) + "\n" +
         "Tonality: " + midi_to_letter(tonality->get_tonic()) + " " + mode_int_to_name(tonality->get_mode()) + "\n" +
         "Chords: " +
         int_vector_to_string(chordDegrees) + "\n" +
            "Qualities: " +
            int_vector_to_string(chordQualities) + "\n" +
            "States: " +
            int_vector_to_string(chordStates) + "\n";
    return message;
}

string TonalProgressionParameters::pretty() const {
    string message = "Section " + std::to_string(sectionNumber) + ":\n";
    message += "Number of chords: " + std::to_string(size) + "\n" +
        "Start: " + std::to_string(start) + "\n" +
            "End: " + std::to_string(end) + "\n" +
         "Tonality: " + midi_to_letter(tonality->get_tonic()) + " " + mode_int_to_name(tonality->get_mode()) + "\n";
    //todo print the rest in a readable way
    message += "Chords: \n";
    for (int i = 0; i < chordDegrees.size(); i++) {
        message += degreeNames[chordDegrees[i]] + " (" + chordQualityNames[chordQualities[i]] + ") in " +
                   stateNames[chordStates[i]];
        if (i != chordDegrees.size() - 1)
            message += " \n";
    }
    return message;
}