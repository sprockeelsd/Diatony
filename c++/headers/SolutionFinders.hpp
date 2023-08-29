#ifndef MYPROJECT_SOLUTIONFINDERS_HPP
#define MYPROJECT_SOLUTIONFINDERS_HPP

#include "FourVoiceTexture.hpp"
#include "Utilities.hpp"
#include "Tonality.hpp"
#include "MajorTonality.hpp"
#include "MinorTonality.hpp"

using namespace Gecode;
using namespace std;

/**
 * Returns the best solution for the problem pb. It uses a branch and bound solver with lexico-minimization of the costs
 * @param pb an instance of a FourVoiceTexture problem
 * @return the best solution to the problem
 */
const FourVoiceTexture* find_best_solution(FourVoiceTexture *pb);

/**
 * Returns the first maxNOfSols solutions for the problem pb using the solver solverType.
 * @param pb an instance of a FourVoiceTexture problem
 * @param solverType the type of the solver to use from solver_types
 * @param maxNOfSols the maximum number of solutions we want to find (the default value is MAX_INT)
 * @return the first maxNOfSols solutions to the problem
 */
vector<const FourVoiceTexture*> find_all_solutions(FourVoiceTexture *pb, int solverType,
                                                   int maxNOfSols = std::numeric_limits<int>::max());

#endif
