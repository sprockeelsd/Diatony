#include "headers/problem_wrapper.hpp"

/**
 * Wraps the FourVoiceTexture constructor.
 * @todo modify this to include any parameters your FourVoiceTexture constructor requires
 * @param size an integer representing the size of the problem
 * @param key the key of the tonality
 * @param mode the mode of the tonality
 * @param chord_degrees an integer array representing the chord degrees
 * @param chord_states an integer array representing the chord states
 * @return A pointer to a FourVoiceTexture object casted as a void*
 */
void* create_new_problem(int size, int key, int mode, int* chord_degrees, int* chord_states){
    Tonality *t;
    if(mode == MAJOR_MODE){
        t = new MinorTonality(key % PERFECT_OCTAVE + PERFECT_OCTAVE);
    }
    else{ // @todo add other modes here if needed
        t = new MinorTonality(key % PERFECT_OCTAVE + PERFECT_OCTAVE);
    }
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
    FourVoiceTexture* pb = static_cast<FourVoiceTexture*>(sp);
    int* sol = pb->return_solution();
    writeToLogFile(pb->toString().c_str());
    return sol;
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