//
// Created by Damien Sprockeels on 24/04/2025.
//

#include "../../headers/diatony/ModulationParameters.hpp"

string ModulationParameters::toString() const {
    string message;
    message += modulation_type_names[type] + " modulation ";
    message += "from section " + to_string(from->get_section_number()) + " to section " + to_string(to->get_section_number()) + "\n";
    message += "starting position: " + to_string(start) + "\n";
    message += "ending position: " + to_string(end) + "\n";
    return message;
}
