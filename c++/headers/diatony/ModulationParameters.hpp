//
// Created by Damien Sprockeels on 24/04/2025.
//

#ifndef MODULATIONPARAMETERS_HPP
#define MODULATIONPARAMETERS_HPP

#include "../aux/Utilities.hpp"
#include "TonalProgressionParameters.hpp"

/**
 * Class representing the parameters of a modulation between two progressions in different tonalities.
 */
class ModulationParameters {
protected:
    const int                           type;       // The type of the modulation (e.g., perfect cadence, pivot chord, alteration, chromatic)
    const int                           start;      // The starting position of the modulation in the piece
    const int                           end;        // The ending position of the modulation in the piece
    const TonalProgressionParameters*   from;       // The parameters of the progression from which the modulation starts
    const TonalProgressionParameters*   to;         // The parameters of the progression to which the modulation goes

public:
    /**
     * Constructor
     * @param type      The type of modulation (e.g., perfect cadence, pivot chord, alteration, chromatic)
     * @param start     The starting position of the modulation in the piece
     * @param end       The ending position of the modulation in the piece
     * @param from      The parameters of the progression from which the modulation starts
     * @param to        The parameters of the progression to which the modulation goes
     */
    ModulationParameters(const int type, const int start, const int end,
                         const TonalProgressionParameters* from, const TonalProgressionParameters* to) :
            type(type), start(start), end(end), from(from), to(to) {}

    /**
     * Copy constructor
     * @param params A pointer to a ModulationParameters object
     */
    explicit ModulationParameters(const ModulationParameters* params) :
            type(params->type), start(params->start), end(params->end),
            from(new TonalProgressionParameters(params->from)),
            to(new TonalProgressionParameters(params->to)) {}

    /**                 getters                     **/

    int                                 get_type()      const { return type; }

    int                                 get_start()     const { return start; }

    int                                 get_end()       const { return end; }

    const TonalProgressionParameters *  get_from()      const { return from; }

    const TonalProgressionParameters *  get_to()        const { return to; }

    /**
     * to_string method
     * @return a string representation of the modulation parameters
     */
    string to_string() const;

};

/**
 * << operator overload
 * @param os the output stream
 * @param params the TonalProgressionParameters object to print
 * @return the output stream with the parameters of the TonalProgressionParameters object
 */
inline std::ostream& operator<<(std::ostream& os, const ModulationParameters& params) {
    os << params.to_string();
    return os;
}



#endif //MODULATIONPARAMETERS_HPP
