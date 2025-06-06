//
// Created by Damien Sprockeels on 24/04/2025.
//

#include "../../headers/diatony/ModulationParameters.hpp"

/**
 * to_string method
 * @return a string representation of the modulation parameters
 */
string ModulationParameters::to_string() const {
    string message;
    message += modulation_type_names[type] + " modulation ";
    message += "from section " + std::to_string(from->get_progression_number()) + " to section " +
        std::to_string(to->get_progression_number()) + "\n";
    message += "starting position: " + std::to_string(start) + "\n";
    message += "ending position: " + std::to_string(end) + "\n";
    return message;
}
