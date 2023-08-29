#include "headers/SolutionFinders.hpp"

FourVoiceTexture* find_best_solution(FourVoiceTexture *pb, int size){
    // create a new search engine
    Search::Base<FourVoiceTexture>* e = make_solver(pb, BAB_SOLVER);

    std::cout << "Finding optimal solution based on preferences:" << std::endl;

    FourVoiceTexture *bestSol; /// keep a pointer to the best solution
    while(FourVoiceTexture * sol = get_next_solution_space(e)){
        bestSol = sol;
    }
    std::cout << "Best Sol: " << bestSol->to_string() << std::endl;
    return bestSol;
}