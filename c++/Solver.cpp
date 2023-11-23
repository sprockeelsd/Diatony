#include "headers/Solver.hpp"

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                                                                                     *
 *                                                Search engine methods                                                *
 *                                                                                                                     *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Creates a search engine for the given problem
 * @param pb an instance of the FourVoiceTexture class representing a given problem
 * @param type the type of search engine to create (see enumeration in headers/gecode_problem.hpp)
 * @return a search engine for the given problem
 */
Search::Base<FourVoiceTexture>* make_solver(FourVoiceTexture* pb, int type){
    Search::Stop *stop = Search::Stop::time(60000); // stop after 60 seconds
    Search::Options opts; //@todo add options when necessary
    opts.threads = 0;
    opts.stop = stop;

    if (type == BAB_SOLVER){
        write_to_log_file("Solver type: BAB\n");
        return new BAB<FourVoiceTexture>(pb, opts);
    }
    else{
        write_to_log_file("Solver type: DFS\n");
        return new DFS<FourVoiceTexture>(pb, opts);
    }
}

/**
 * Returns the next solution space for the problem
 * @param solver a solver for the problem
 * @return an instance of the FourVoiceTexture class representing the next solution to the problem
 */
FourVoiceTexture* get_next_solution_space(Search::Base<FourVoiceTexture>* solver){
    FourVoiceTexture* sol_space = solver->next();
    if (sol_space == nullptr) // handle the case of no solution or time out, necessary when sending the data to OM
        return nullptr;
    return sol_space;
}

/**
 * Returns the best solution for the problem pb. It uses a branch and bound solver with lexico-minimization of the costs
 * @param pb an instance of a FourVoiceTexture problem
 * @return the best solution to the problem
 */
const FourVoiceTexture* find_best_solution(FourVoiceTexture *pb){
    // create a new search engine
    auto* solver = make_solver(pb, BAB_SOLVER);

    write_to_log_file("Searching for the optimal solution based on preferences:\n");

    FourVoiceTexture *bestSol; // keep a pointer to the best solution
    while(FourVoiceTexture *sol = get_next_solution_space(solver)){
        bestSol = sol;
    }
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
    auto* solver = make_solver(pb, solverType);
    write_to_log_file("\nSearching for solutions:\n");

    int nbSol = 0;
    while(FourVoiceTexture *sol= get_next_solution_space(solver)){
        nbSol++;
        sols.push_back(sol);
        string message = "Solution found: \nSolution" + to_string(nbSol) + ": \n" + sol->to_string() + "\n";
        write_to_log_file(message.c_str());
        std::cout << message << std::endl  << statistics_to_string(solver->statistics()) << std::endl;
        write_to_log_file(statistics_to_string(solver->statistics()).c_str());
        if (nbSol >= maxNOfSols)
            break;
    }
    if(nbSol == 0)
        write_to_log_file("No solutions found.");
    return sols;
}