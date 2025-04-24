//
// Created by Damien Sprockeels on 24/04/2025.
//

#ifndef MODULATIONPARAMETERS_HPP
#define MODULATIONPARAMETERS_HPP

#include "../aux/Utilities.hpp"
#include "TonalProgressionParameters.hpp"

class ModulationParameters {
protected:
    const int type;
    const int start;
    const int end;
    TonalProgressionParameters* from;
    TonalProgressionParameters* to;

public:
    ModulationParameters(const int type, const int start, const int end,
                         TonalProgressionParameters* from, TonalProgressionParameters* to) :
            type(type), start(start), end(end), from(from), to(to) {}

    /**                 getters                     **/
    int get_type() const { return type; }

    int get_start() const { return start; }

    int get_end() const { return end; }

    TonalProgressionParameters* get_from() const { return from; }

    TonalProgressionParameters* get_to() const { return to; }

    string toString() const;

};



#endif //MODULATIONPARAMETERS_HPP
