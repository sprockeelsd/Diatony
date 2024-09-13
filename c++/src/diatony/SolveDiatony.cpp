//
// Created by Damien Sprockeels on 03/07/2024.
//

#include <utility>

#include "../../headers/diatony/SolveDiatony.hpp"

/**
 * Finds an optimal solution for a four voice texture problem and returns the list of all intermediate solutions found
 * during search
 * @param size the number of chords
 * @param tonality the tonality of the piece
 * @param chords the chord degrees of the progression
 * @param qualities the qualities of the chords
 * @param states the states of the chords
 * @return A vector<FourVoiceTexture*> representing all the solutions found during search. If no solutions are found,
 * returns an empty vector. If the best solution is not found within the time limit, returns all solutions found so far.
 */
vector<FourVoiceTexture *>
solve_diatony_problem(int size, Tonality *tonality, vector<int> chords, vector<int> qualities, vector<int> states) {
    /// create a new problem
    auto pb = new FourVoiceTexture(size, tonality, std::move(chords), std::move(qualities), std::move(states));
    /// Search options
    Search::Options opts;
    opts.threads = 1;
    opts.stop = Search::Stop::time(600000); // stop after 120 seconds
    opts.cutoff = Search::Cutoff::merge(
            Search::Cutoff::linear(2*size),
            Search::Cutoff::geometric((4*size)^2, 2));
    opts.nogoods_limit = size * 4 * 4;

    //DFS<FourVoiceTexture> solver(pb, opts);
    /// Restart based solver
    RBS<FourVoiceTexture, BAB> solver(pb, opts);
    delete pb;

    vector<FourVoiceTexture*> solutions;

    int n_sols = 0;
    auto start = std::chrono::high_resolution_clock::now();     /// start time
    /// Find all solutions
    while(FourVoiceTexture* next_sol = solver.next()){
        n_sols++;
        std::cout << "temporary solution found (" << n_sols  << ")" << std::endl;
        solutions.push_back((FourVoiceTexture*) next_sol->copy());
        std::cout << next_sol->to_string() << std::endl;
        std::cout << statistics_to_string(solver.statistics()) << std::endl;
        delete next_sol;
    }
    /// Give info on the search (complete, optimal, etc)
    std::cout << "search over" << std::endl;
    auto currTime = std::chrono::high_resolution_clock::now();     /// current time
    std::chrono::duration<double> duration = currTime - start;
    if(solver.stopped()){
        std::cout << "Best solution not found within the time limit. Current best solution found: " << std::endl;
        std::cout << solutions[n_sols-1]->to_string() << std::endl;
        std::cout << statistics_to_string(solver.statistics()) << std::endl;
    }
    else if(n_sols == 0){
        std::cout << "No solutions" << std::endl;
    }
    else{
        std::cout << "Best solution found" << std::endl;
        std::cout << solutions[n_sols-1]->to_string() << std::endl;
        std::cout << statistics_to_string(solver.statistics()) << std::endl;
    }
    std::cout << "time taken: " << duration.count() << " seconds and " << n_sols << " solutions found.\n" << std::endl;
    return solutions;
}

/**
 * Finds an optimal solution for a four voice texture problem.
 * @param size the number of chords
 * @param tonality the tonality of the piece
 * @param chords the chord degrees of the progression
 * @param qualities the qualities of the chords
 * @param states the states of the chords
 * @return A FourVoiceTexture* representing the best solution found. If no solution is found, returns nullptr.
 * If the best solution is not found during search, returns the last solution found (best so far).
 */
FourVoiceTexture* solve_diatony_problem_optimal(int size, Tonality* tonality, vector<int> chords, vector<int> qualities,
                                                vector<int> states){
    auto sols = solve_diatony_problem(size, tonality, std::move(chords), std::move(qualities), std::move(states));
    if(sols.empty())
        return nullptr;
    return sols.back();
}

/**
 * Finds all solutions close to the optimal solution, with a margin percentage of deviation from the cost vector. That
 * margin applies to all costs.
 * @param size the number of chords
 * @param tonality the tonality of the piece
 * @param chords the chord degrees of the progression
 * @param qualities the qualities of the chords
 * @param states the states of the chords
 * @param costs the cost vector of the best solution found without relaxation
 * @param margin a percentage of "error" that is allowed to find close to optimal solutions
 * @return a vector of the solutions that are close to the best solution
 */
vector<FourVoiceTexture*> find_optimal_solutions_with_margin(int size, Tonality* tonality, vector<int> chords,
                                                             vector<int> qualities, vector<int> states, vector<int> costs,
                                                             double margin){

    auto pb = new FourVoiceTexture(size, tonality, std::move(chords), std::move(qualities),
                                   std::move(states), std::move(costs), margin);

    DFS<FourVoiceTexture> solver(pb);
    delete pb;

    vector<FourVoiceTexture*> solutions;
    int n_sols = 0;
    while(auto next_sol = solver.next()){
        n_sols++;
        std::cout << "Solution found (" << n_sols  << ")" << std::endl;
        solutions.push_back((FourVoiceTexture*) next_sol->copy());
        std::cout << next_sol->to_string() << std::endl;
        std::cout << statistics_to_string(solver.statistics()) << std::endl;
        delete next_sol;
    }
    std::cout << n_sols << " solutions found with a margin on costs of " << to_string(margin) << std::endl;
    return solutions;
}