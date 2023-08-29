#include "headers/SolutionFinders.hpp"

/**
 * Returns the best solution for the problem pb. It uses a branch and bound solver with lexico-minimization of the costs
 * @param pb an instance of a FourVoiceTexture problem
 * @return the best solution to the problem
 */
const FourVoiceTexture* find_best_solution(FourVoiceTexture *pb){
    // create a new search engine
    auto* solver = new BAB<FourVoiceTexture>(pb);

    std::cout << "\nSearching for the optimal solution based on preferences:\n" << std::endl;

    FourVoiceTexture *bestSol; // keep a pointer to the best solution
    while(FourVoiceTexture *sol = get_next_solution_space(solver)){
        bestSol = sol;
    }
    std::cout << "Best solution found: \n\n" << bestSol->to_string() << std::endl;
    write_to_log_file(bestSol->to_string().c_str());
    return bestSol;
}

/**
 * Returns the first maxNOfSols solutions for the problem pb using the solver solverType.
 * @param pb an instance of a FourVoiceTexture problem
 * @param solverType the type of the solver to use from solver_types
 * @param maxNOfSols the maximum number of solutions we want to find (the default value is 1000)
 * @return the first maxNOfSols solutions to the problem
 */
vector<const FourVoiceTexture*> find_all_solutions(FourVoiceTexture *pb, int solverType, int maxNOfSols){
    vector<const FourVoiceTexture*> sols;
    // create the search engine
    Search::Base<FourVoiceTexture>* solver = make_solver(pb, solverType);
    std::cout << "\n Searching for all solutions to the problem with the given solver type:" << std::endl;

    int nbSol = 0;
    while(FourVoiceTexture *sol= get_next_solution_space(solver)){
        nbSol++;
        sols.push_back(sol);
        std::cout << "Solution " << nbSol << ": \n" << sol->to_string() << std::endl;
        write_to_log_file(to_string(nbSol).c_str());
        write_to_log_file(sol->to_string().c_str());
        if (nbSol >= maxNOfSols)
            break;
    }
    return sols;
}