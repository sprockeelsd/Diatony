//
// Created by Damien Sprockeels on 03/07/2024.
//

#ifndef DIATONY_SOLVEPROBLEM_HPP
#define DIATONY_SOLVEPROBLEM_HPP

#include "FourVoiceTexture.hpp"

/**
 * Finds an optimal solution for a four voice texture problem and returns the list of all intermediate solutions found
 * during search
 * @param size the number of chords
 * @param tonality the tonaity of the piece
 * @param chords the chord degrees of the progression
 * @param qualities the qualities of the chords
 * @param states the states of the chords
 * @return A list of FourVoiceTexture* representing all the solutions found during search
 */
vector<FourVoiceTexture*> solve_diatony_problem(int size, Tonality* tonality, vector<int> chords, vector<int> qualities,
                                               vector<int> states);
/**
 * Finds an optimal solution for a four voice texture problem.
 * @param size the number of chords
 * @param tonality the tonaity of the piece
 * @param chords the chord degrees of the progression
 * @param qualities the qualities of the chords
 * @param states the states of the chords
 * @return A FourVoiceTexture* representing the best solution found. If no solution is found, returns nullptr.
 * If the best solution is not found during search, returns the last solution found (best so far).
 */
FourVoiceTexture* solve_diatony_problem_optimal(int size, Tonality* tonality, vector<int> chords, vector<int> qualities,
                                               vector<int> states);

#endif //DIATONY_SOLVEPROBLEM_HPP
