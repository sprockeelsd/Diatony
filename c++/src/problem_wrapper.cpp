#include "../headers/problem_wrapper.hpp"

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
void *create_new_problem(int size, int key, int mode, int *chord_degrees, int *chord_qualities, int *chord_states) {
    /// date and time for logs
    write_to_log_file(time().c_str());

    Tonality *t;
    if(mode == MAJOR_MODE) /// major mode
        t = new MajorTonality(key % PERFECT_OCTAVE);
    else if(mode == MINOR_MODE) /// minor mode
        t = new MinorTonality(key % PERFECT_OCTAVE);
    // @todo add other modes here if needed
    else /// default = major
        t = new MajorTonality(key % PERFECT_OCTAVE);

    vector<int> degrees(int_pointer_to_vector(chord_degrees, size));
    vector<int> qualities(int_pointer_to_vector(chord_qualities, size));
    vector<int> states(int_pointer_to_vector(chord_states, size));
    auto* pb = new FourVoiceTexture(size, t, degrees, qualities, states);
    return (void*) pb;
}

/**
 * returns the size of the problem
 * @param sp a void* pointer to a FourVoiceTexture object
 * @return an integer representing the size of the problem
 */
int get_size(void* sp){
    return static_cast<FourVoiceTexture*>(sp)->get_size();
}

/**
 * returns the values of the variables for a solution
 * @param sp a void* pointer to a FourVoiceTexture object
 * @return an int* pointer representing the values of the variables
 */
int* return_solution(void* sp){
    auto* pb = static_cast<FourVoiceTexture*>(sp);
    int* sol = pb->return_solution();
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
 * returns the best solution space, it should be bound. If not, it will return NULL.
 * @param solver a void* pointer to a BAB<FourVoiceTexture> for the search engine of the problem
 * @return a void* cast of a FourVoiceTexture* pointer
 */
void* return_best_solution_space(void* solver){
    FourVoiceTexture *bestSol; // keep a pointer to the best solution
    while(FourVoiceTexture *sol = get_next_solution_space(static_cast<BAB<FourVoiceTexture>*>(solver))){
        bestSol = sol;
    }
    std::cout << "Best solution found: \n\n" << bestSol->to_string() << std::endl;
    write_to_log_file(bestSol->to_string().c_str());
    return (void*) bestSol;
}

/**
 * returns the next solution space, it should be bound. If not, it will return NULL.
 * @param solver a void* pointer to a Base<FourVoiceTexture>* pointer for the search engine of the problem
 * @return a void* cast of a FourVoiceTexture* pointer
 */
void* return_next_solution_space(void* solver){
    return (void*) get_next_solution_space(static_cast<DFS<FourVoiceTexture>*>(solver));
}