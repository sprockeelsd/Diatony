//
// Created by Damien Sprockeels on 03/07/2024.
//

#ifndef DIATONY_SOLVEPROBLEM_HPP
#define DIATONY_SOLVEPROBLEM_HPP

#include "TonalProgression.hpp"

/**
 * Finds an optimal solution for a four voice texture problem and returns the list of all intermediate solutions found
 * during search
 * @param size the number of chords
 * @param tonality the tonaity of the piece
 * @param chords the chord degrees of the progression
 * @param qualities the qualities of the chords
 * @param states the states of the chords
 * @param print
 * @return A list of FourVoiceTexture* representing all the solutions found during search
 */
vector<TonalProgression *> solve_diatony_problem(int size, Tonality *tonality, vector<int> chords, vector<int> qualities,
                                                 vector<int> states, bool print = false);
/**
 * Finds an optimal solution for a four voice texture problem.
 * @param size the number of chords
 * @param tonality the tonaity of the piece
 * @param chords the chord degrees of the progression
 * @param qualities the qualities of the chords
 * @param states the states of the chords
 * @param print
 * @return A FourVoiceTexture* representing the best solution found. If no solution is found, returns nullptr.
 * If the best solution is not found during search, returns the last solution found (best so far).
 */
TonalProgression* solve_diatony_problem_optimal(int size, Tonality* tonality, vector<int> chords, vector<int> qualities,
                                                vector<int> states, bool print = false);

/**
 * Finds all solutions close to the optimal solution, with a margin percentage of deviation from the cost vector. That
 * margin applies to all costs.
 * @param size the number of chords
 * @param tonality the tonaity of the piece
 * @param chords the chord degrees of the progression
 * @param qualities the qualities of the chords
 * @param states the states of the chords
 * @param costs the cost vector of the best solution found without relaxation
 * @param margin a percentage of "error" that is allowed to find close to optimal solutions
 * @return a vector of the solutions that are close to the best solution
 */
vector<TonalProgression*> find_optimal_solutions_with_margin(int size, Tonality* tonality, vector<int> chords,
                                                             vector<int> qualities, vector<int> states, vector<int> costs,
                                                             double margin = 0.0);

#endif //DIATONY_SOLVEPROBLEM_HPP
