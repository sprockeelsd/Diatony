//
// Created by Damien Sprockeels on 23/04/2025.
//

#include "../../headers/diatony/FourVoiceTextureParameters.hpp"

/**
 * to_string method
 * Prints the total number of chords of the piece, the number of sections, the section starts and ends, the tonalities of
 * the sections, and the parameters for each of the sections
 * @return a string representation of the object
 */
string FourVoiceTextureParameters::toString() const {
    string message;
    message += "Global problem Parameters:\n\n";
    message += "Total number of chords: " + std::to_string(totalNumberOfChords) + "\n";
    message += "Number of sections: " + std::to_string(numberOfSections) + "\n";
    message += "\n";
    message += "Section parameters: \n\n";
    for (const auto p : sectionParameters ) {
        message += p->pretty() + "\n";
    }
    message += "\n";
    message += "Modulation parameters: \n\n";
    for (const auto m : modulationParameters ) {
        message += m->to_string() + "\n";
    }
    return message;
}