//
// Created by Damien Sprockeels on 23/04/2025.
//

#include "../../headers/diatony/FourVoiceTextureParameters.hpp"

/**
 * toString method
 * Prints the total number of chords of the piece, the number of sections, the section starts and ends, the tonalities of
 * the sections, and the parameters for each of the sections
 * @return a string representation of the object
 */
string FourVoiceTextureParameters::toString() const {
    string message;
    message += "Four Voice Texture Parameters:\n";
    message += "Total number of chords: " + std::to_string(totalNumberOfChords) + "\n";
    message += "Number of sections: " + std::to_string(numberOfSections) + "\n";
    message += "Section starts: " + int_vector_to_string(sectionStarts) + "\n";
    message += "Section ends: " + int_vector_to_string(sectionEnds) + "\n";
    message += "Section tonalities: ";
    for (const auto& tonality : sectionTonalities) {
        message += midi_to_letter(tonality->get_tonic()) + " " + mode_int_to_name(tonality->get_mode()) + ", ";
    }
    message += "\n";
    message += "Section parameters: ";
    for ( auto p : sectionParameters ) {
        message += p->to_string() + "\n";
    }
    message += "\n";
    return message;
}