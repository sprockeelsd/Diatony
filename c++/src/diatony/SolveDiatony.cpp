//
// Created by Damien Sprockeels on 03/07/2024.
//

#include <utility>

#include "../../headers/diatony/SolveDiatony.hpp"

/**
 * Returns the best solution to the Four voice texture problem specified by the parameters. If the maximum search time
 * specified in the options is reached, the best solution found so far is returned.
 * @param params the parameters of the problem, containing the tonalities, chord degrees, qualities and states for each chord in each progression
 * @param opts the options for the search, containing the maximum search time, the restart strategy, etc.
 * @param print whether to print the solutions found during the search
 * @return the best solution found, or nullptr if no solution was found
 */
const FourVoiceTexture* solve_diatony(FourVoiceTextureParameters* params, const Options* opts, bool print) {
    // create an instance of the FVT problem
    const auto pb = new FourVoiceTexture(params);
    /// create the restart based solver
    RBS<FourVoiceTexture, BAB> solver(pb, *opts);
    delete pb;

    int n_sols = 0;
    FourVoiceTexture* lastSol = nullptr;
    const auto start = std::chrono::high_resolution_clock::now();     /// start time
    // Search for solutions
    while (FourVoiceTexture* sol_fvt = solver.next()) {
        n_sols += 1;
        lastSol = dynamic_cast<FourVoiceTexture *>(sol_fvt->copy());
        if (print) {
            std::cout << sol_fvt->to_string() << std::endl;
            std::cout << statistics_to_string(solver.statistics()) << std::endl;
        }
        //todo improve branching and search (see notes)
        //if (n_sols >= 1) break;
    }
    const auto currTime = std::chrono::high_resolution_clock::now();     /// current time
    const std::chrono::duration<double> duration = currTime - start;     /// elapsed time

    if (print) {
        std::cout << "search over" << std::endl;
        if(solver.stopped()){
            std::cout << "Best solution not found within the time limit." << std::endl;
        }
        else if(n_sols == 0){
            std::cout << "No solutions" << std::endl;
        }
        else{
            std::cout << "Best solution found." << std::endl;
        }
        std::cout << "time taken: " << duration.count() << " seconds and " << n_sols << " solutions found.\n" << std::endl;
    }
    return lastSol;
}
//todo other functions (returning all sols, finding approximately optimal solutions, etc.)