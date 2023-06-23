#include "headers/four_voice_texture.hpp"

/***********************************************************************************************************************
 *                                          FourVoiceTexture class methods                                             *
 ***********************************************************************************************************************/

/**
 * Constructor
 * @todo Modify this constructor depending on your problem. This constructor is where the problem is defined
 * @todo (variables, constraints, branching, ...)
 * @param size the size of the array of variables
 * @param l the lower bound of the domain of the variables
 * @param u the upper bound of the domain of the variables
 */
FourVoiceTexture::FourVoiceTexture(int s, int l, int u) {
    size = s;
    lower_bound_domain = l;
    upper_bound_domain = u;

    // variable initialization
    vars = IntVarArray(*this, size, l, u);

    //constraints
    distinct(*this, vars);

    //branching
    branch(*this, vars, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
}

/**
 * Copy constructor
 * @param s an instance of the FourVoiceTexture class
 * @todo modify this copy constructor to also copy any additional attributes you add to the class
 */
FourVoiceTexture::FourVoiceTexture(FourVoiceTexture& s): Space(s){
    //IntVars update
    size = s.size;
    lower_bound_domain = s.lower_bound_domain;
    upper_bound_domain = s.upper_bound_domain;
    vars.update(*this, s.vars);
}

/**
 * Returns the size of the problem
 * @return an integer representing the size of the vars array
 */
int FourVoiceTexture::getSize(){
    return size;
}

/**
 * Returns the values taken by the variables vars in a solution
 * @todo Modify this to return the solution for your problem. This function uses @param size to generate an array of integers
 * @return an array of integers representing the values of the variables in a solution
 */
int* FourVoiceTexture::return_solution(){
    int* solution = new int[size];
    for(int i = 0; i < size; i++){
        solution[i] = vars[i].val();
    }
    return solution;
}

/**
 * Copy method
 * @return a copy of the current instance of the FourVoiceTexture class. Calls the copy constructor
 */
Space* FourVoiceTexture::copy(void) {
    return new FourVoiceTexture(*this);
}

/**
 * Constrain method for bab search
 * @todo modify this function if you want to use branch and bound
 * @param _b a solution to the problem from which we wish to add a constraint for the next solutions
 */
void FourVoiceTexture::constrain(const Space& _b) {
    const FourVoiceTexture &b = static_cast<const FourVoiceTexture &>(_b);
    rel(*this, vars, IRT_GQ, 2);
}

/**
 * Prints the solution in the console
 */
void FourVoiceTexture::print_solution(){
    for(int i = 0; i < size; i++){
        cout << vars[i].val() << " ";
    }
    cout << endl;
}

/**
 * toString method
 * @return a string representation of the current instance of the FourVoiceTexture class.
 * Right now, it returns a string "FourVoiceTexture object. size = <size>"
 * If a variable is not assigned when this function is called, it writes <not assigned> instead of the value
 * @todo modify this method to also print any additional attributes you add to the class
 */
string FourVoiceTexture::toString(){
    string message = "FourVoiceTexture object. \n";
    message += "size = " + to_string(size) + "\n" + "lower bound for the domain : " +
            to_string(lower_bound_domain) + "\n" + "upper bound for the domain : " + to_string(upper_bound_domain)
             + "\n" + "current values for vars: [";
    for(int i = 0; i < size; i++){
        if (vars[i].assigned())
            message += to_string(vars[i].val()) + " ";
        else
            message += "<not assigned> ";
    }
    message += "]\n\n";
    return message;
}

/***********************************************************************************************************************
 *                                                Search engine methods                                                *
 ***********************************************************************************************************************/

/**
 * Creates a search engine for the given problem
 * @todo Modify this function to add search options etc
 * @param pb an instance of the FourVoiceTexture class representing a given problem
 * @param type the type of search engine to create (see enumeration in headers/gecode_problem.hpp)
 * @return a search engine for the given problem
 */
Search::Base<FourVoiceTexture>* make_solver(FourVoiceTexture* pb, int type){

    Gecode::Search::Options opts;
    /**@todo add here any options you want*/

    if (type == bab_solver)
        return new BAB<FourVoiceTexture>(pb, opts);
    else // default case
        return new DFS<FourVoiceTexture>(pb, opts);
}

/**
 * Returns the next solution space for the problem
 * @param solver a solver for the problem
 * @return an instance of the FourVoiceTexture class representing the next solution to the problem
 */
FourVoiceTexture* get_next_solution_space(Search::Base<FourVoiceTexture>* solver){
    FourVoiceTexture* sol_space = solver->next();
    if (sol_space == nullptr) // handle the case of no solution or time out, necessary when sending the data to OM
        return NULL;
    return sol_space;
}

/***********************************************************************************************************************
 *                                                 Auxiliary functions                                                 *
 ***********************************************************************************************************************/

/**
 * Write a text into a log file
 * Useful for debugging in the OM environment
 * @param message the text to write
 */
void writeToLogFile(const char* message){
    std::time_t currentTime = std::time(nullptr); // Get the current time
    std::string timeString = std::asctime(std::localtime(&currentTime)); // Convert to string

    const char* homeDir = std::getenv("HOME"); // Get the user's home directory
    if (homeDir) {
        std::string filePath(homeDir);
        filePath += "/log.txt"; // Specify the desired file path, such as $HOME/log.txt

        std::ofstream myfile(filePath, std::ios::app); // append mode
        if (myfile.is_open()) {
            myfile <<timeString<< endl << message << endl;
            myfile.close();
        }
    }
}