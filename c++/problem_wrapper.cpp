#include "headers/problem_wrapper.hpp"

/**
 * Wraps the FourVoiceTexture constructor.
 * @todo modify this to include any parameters your FourVoiceTexture constructor requires
 * @param size an integer representing the size of the problem
 * @param lower_bound_domain an integer representing the lower bound of the domain of the variables
 * @param upper_bound_domain an integer representing the upper bound of the domain of the variables
 * @return A pointer to a FourVoiceTexture object casted as a void*
 */
void* create_new_problem(int size, int* chord_degrees, int* chord_states){
    Tonality* t = new MajorTonality(C);
    vector<int> degrees(int_pointer_to_vector(chord_degrees, size));
    vector<int> states(int_pointer_to_vector(chord_states, size));
    FourVoiceTexture* pb = new FourVoiceTexture(size, t, degrees, states);
    writeToLogFile(pb->toString().c_str());
    return (void*) pb;
}

/**
 * returns the size of the problem
 * @param sp a void* pointer to a FourVoiceTexture object
 * @return an integer representing the size of the problem
 */
int get_size(void* sp){
    return static_cast<FourVoiceTexture*>(sp)->getSize();
}

/**
 * returns the values of the variables for a solution
 * @param sp a void* pointer to a FourVoiceTexture object
 * @return an int* pointer representing the values of the variables
 */
int* return_solution(void* sp){
    return static_cast<FourVoiceTexture*>(sp)->return_solution();
}

/**
 * creates a search engine for FourVoiceTexture objects
 * @param sp a void* pointer to a FourVoiceTexture object
 * @return a void* cast of a Base<FourVoiceTexture>* pointer
 */
void* create_solver(void* sp, int type){
    return (void*) make_solver(static_cast<FourVoiceTexture*>(sp), type);
}

/**
 * returns the next solution space, it should be bound. If not, it will return NULL.
 * @param solver a void* pointer to a Base<FourVoiceTexture>* pointer for the search engine of the problem
 * @return a void* cast of a FourVoiceTexture* pointer
 */
void* return_next_solution_space(void* solver){
    return (void*) get_next_solution_space(static_cast<DFS<FourVoiceTexture>*>(solver));
}