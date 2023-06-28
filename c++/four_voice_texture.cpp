#include "headers/four_voice_texture.hpp"

/***********************************************************************************************************************
 *                                          FourVoiceTexture class methods                                             *
 ***********************************************************************************************************************/

/**
 * Constructor
 * @param s the size of the array of variables
 * @param *t a pointer to a Tonality object
 * @param chordDegs the degrees of the chord of the chord progression
 * @param chordStas the states of the chord of the chord progression (fundamental, 1st inversion,...)
 */
FourVoiceTexture::FourVoiceTexture(int s, Tonality *t, vector<int> chordDegs, vector<int> chordStas){
    size = s;
    tonality = t;
    chordDegrees = chordDegs;
    chordStates = chordStas;

    /** variable initialization */
    FullChordsVoicing = IntVarArray(*this, 4*size, tonality->get_tonality_notes());

    bassMelodicIntervals = IntVarArray(*this, size-1, -perfectOctave, perfectOctave);
    tenorMelodicIntervals = IntVarArray(*this, size-1, -perfectOctave, perfectOctave);
    altoMelodicIntervals = IntVarArray(*this, size-1, -perfectOctave, perfectOctave);
    sopranoMelodicIntervals = IntVarArray(*this, size-1, -perfectOctave, perfectOctave);

    // variable arrays for harmonic intervals between adjacent voices
    bassTenorHarmonicIntervals = IntVarArray(*this, size, 0, perfectOctave + perfectFifth);
    tenorAltoHarmonicIntervals = IntVarArray(*this, size, 0, perfectOctave);
    altoSopranoHarmonicIntervals = IntVarArray(*this, size, 0, perfectOctave);

    /** constraints */

    // link the arrays together
    link_melodic_arrays(*this, size, FullChordsVoicing, bassMelodicIntervals, tenorMelodicIntervals, altoMelodicIntervals, sopranoMelodicIntervals);
    link_harmonic_arrays(*this, size, FullChordsVoicing, bassTenorHarmonicIntervals, tenorAltoHarmonicIntervals, altoSopranoHarmonicIntervals);

    // restrain the domain of the voices to their range + state that bass <= tenor <= alto <= soprano
    restrain_voices_domains(*this, size, FullChordsVoicing);

    /**------------------------------------------chord related constraints --------------------------------------------*/
    for(int i = 0; i < size; i++){
        IntVarArgs currentChord(FullChordsVoicing.slice(4 * i, 1, 4));

        setToChord(*this, tonality, chordDegrees[i], currentChord);
        setBass(*this, tonality, chordDegrees[i], chordStates[i], currentChord);
        // @todo check why OM doesn't find solutions (probably a MIDIcent problem?)
    }

    //branching
    branch(*this, FullChordsVoicing, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
}

/**
 * Copy constructor
 * @param s an instance of the FourVoiceTexture class
 */
FourVoiceTexture::FourVoiceTexture(FourVoiceTexture& s): Space(s){
    //IntVars update
    size = s.size;
    tonality = s.tonality;
    FullChordsVoicing.update(*this, s.FullChordsVoicing);
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
 * @return an array of integers representing the values of the variables in a solution
 */
int* FourVoiceTexture::return_solution(){
    int* solution = new int[size*4];
    for(int i = 0; i < 4*size; i++){
        solution[i] = FullChordsVoicing[i].val();
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
 * @param _b a solution to the problem from which we wish to add a constraint for the next solutions
 */
void FourVoiceTexture::constrain(const Space& _b) {
    const FourVoiceTexture &b = static_cast<const FourVoiceTexture &>(_b);
}

/**
 * Prints the solution in the console
 */
void FourVoiceTexture::print_solution(){
    for(int i = 0; i < 4*size; i++){
        cout << FullChordsVoicing[i].val() << " ";
    }
    cout << endl;
}

/**
 * toString method
 * @return a string representation of the current instance of the FourVoiceTexture class.
 * Right now, it returns a string "FourVoiceTexture object. size = <size>"
 * If a variable is not assigned when this function is called, it writes <not assigned> instead of the value
 */
string FourVoiceTexture::toString(){
    string message = "FourVoiceTexture object. \n";
    message += "size = " + to_string(size) + "\n.";
    message += "chord degrees = " + int_vector_to_string(chordDegrees) + "\n";
    message += "chord states = " + int_vector_to_string(chordStates) + "\n";
    message += "FullChordsVoicing = {";
    for(int i = 0; i < 4*size; i++){
        if(FullChordsVoicing[i].assigned())
            message += to_string(FullChordsVoicing[i].val());
        else
            message += "<not assigned>";
        if(i != 4*size - 1)
            message += ", ";
    }
    message += "}\n\n";
    return message;
}

/***********************************************************************************************************************
 *                                                Search engine methods                                                *
 ***********************************************************************************************************************/

/**
 * Creates a search engine for the given problem
 * @param pb an instance of the FourVoiceTexture class representing a given problem
 * @param type the type of search engine to create (see enumeration in headers/gecode_problem.hpp)
 * @return a search engine for the given problem
 */
Search::Base<FourVoiceTexture>* make_solver(FourVoiceTexture* pb, int type){

    Gecode::Search::Options opts;

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