//
// Created by Damien Sprockeels on 03/07/2024.
//

#ifndef DIATONY_SOLVEPROBLEM_HPP
#define DIATONY_SOLVEPROBLEM_HPP

#include "FourVoiceTexture.hpp"
#include "../aux/Utilities.hpp"

/**
 * Returns the best solution to the Four voice texture problem specified by the parameters. If the maximum search time
 * specified in the options is reached, the best solution found so far is returned.
 * @param params the parameters of the problem, containing the tonalities, chord degrees, qualities and states for each chord in each progression
 * @param opts the options for the search, containing the maximum search time, the restart strategy, etc.
 * @param print whether to print the solutions found during the search
 * @return the best solution found, or nullptr if no solution was found
 */
const FourVoiceTexture* solve_diatony(FourVoiceTextureParameters* params, const Options* opts = nullptr, bool print = false);

//todo other functions (returning all sols, finding approximately optimal solutions, etc.)

#endif //DIATONY_SOLVEPROBLEM_HPP